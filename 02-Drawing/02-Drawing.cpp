#include <iostream>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h>

#include "SDL.h"
#include <SDL_opengl.h>

void logEvent(SDL_Event event) {
  std::cout << "Logging event with type: " << event.type << std::endl;
}

void computeVertices() {
  /**
   * OpenGL expects all vertices as a single array
   * so we must pack all the attributes for a single
   * vertex together, and then understand the format
   * of our array. In the following array we have only
   * x & y attributes for our vertices, so we can format
   * our array as follows, noting what exactly this means.
   */
  float vertices[] = {
     0.0f,   0.5f, // Vertex 1
     0.5f,  -0.5f, // Vertex 2
    -0.5f,  -0.5f  // Vertex 3
  };

  /**
   * We want to upload the vertex data we've created, to our
   * graphics card. GPU memory is super fast and we don't want
   * to have to keep sending data to it so we'll send it right
   * away so it can handle it.
   */

  // Create Vertex Buffer Object (VBO)
  // GLuint is a platform-independent `unsigned int`
  // OpenGL handles the buffer memory for us, so we get
  // an int representing the buffer, not a pointer
  GLuint vbo;
  glGenBuffers(1, &vbo); // Generates our buffer

  // Make the VBO be the active object so we can upload data to it
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  /**
   * Now with our VBO as the active array buffer, we can copy vertex data to it
   * Final param depends on usage of vertex data. This determines what kind of memory
   * the data will be stored in on the GPU. Can help us make certain tradeoffs or optimizations
   */
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  /**
   * The vertex shader is a small program that runs on the graphics card
   * and explains to the GPU the format in which we've defined our vertex
   * array. A change in or vertex array attribute format will require a change
   * in the vertex (and probably fragment) shader. The vertex shader is responsible
   * for taking in the vertices in the vertex array and outputting their final position
   * in device coordinates, and spitting out any data the fragment shader will require.
   */
  const char* vertexSource = R"glsl(
    #version 150 core

    // This tell us there is one attribute to each
    // vertex. The number after vec (2) tells us that
    // this attribute has two values (as we've formatted array)
    in vec2 position;

    void main() {
      // Final position is assigned to gl_Position for built in stuff
      // Last value must be 1.0 for some reason, not sure about last one?
      gl_Position = vec4(position, 0.0, 1.0);
    }
  )glsl";

  /**
   * Now that we have the vertex shader source written in GLSL
   * we can create the shader itself similarly to how we created
   * our vertex buffer object by telling OpenGL to create and manage
   * the underlying object, maintaining a reference to it, and loading
   * data into it.
   */
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  // Check the status of the vertex shader to ensure that it compiled properly
  GLint status;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    std::cerr << "Uh oh, the vertex shader failed to compile!!" << std::endl;
    return;
  }

  /**
   * The fragment shader takes the information output from the
   * vertex shader, called fragments. Like vertex shader, it has
   * a single mandatory output = final color of fragment. This shader
   * will just output a single color every time.
   */
  const char* fragmentSource = R"glsl(
    #version 150 core

    out vec4 outColor;
    void main() {
      outColor  = vec4(0.8, 0.8, 0.8, 0.8);
    }
  )glsl";
  
  /**
   * Now that we have the fragment shader source we can create the
   * shader and compile it just like we did with the vertex shader.
   * The process is almost identical, with different constants used.
   */
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  // Reuse our old status variable to see if the fragment shader compiled successfully
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

  if (status != GL_TRUE) {
    std::cerr << "Uh oh, the fragment shader failed to compile!!" << std::endl;
    return;
  }

  /**
   * As the Drawing polygons section of the open.gl tutorial indicates, the
   * shaders have only been "programmed" to work together, but have no real
   * connection to each other. In order to to use them together in the graphics
   * pipeline, we have to create a shader program consisting of these two shaders.
   */
   GLuint shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);

   /**
    * The line of code below is used to specify which output is written to
    * which buffer. This is set to 0 by default and since there is only one
    * output right now, the following line of code isn't necessary
    */
   //glBindFragDataLocation(shaderProgram, 0, "outColor");

   glLinkProgram(shaderProgram);
   glUseProgram(shaderProgram); // like a VBO, only one program can be active at a time

  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posAttrib);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
  SDL_Window *window;
  SDL_Init(SDL_INIT_EVERYTHING);

  /**
   * Tell SDL we want a forward compatible OpenGL 3.2 context
   * and to create a stencil buffer (which will be used later).
   */
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  // Create our window that will be centered with dimensions 900x560
  window = SDL_CreateWindow("Dom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 900, 560, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  // Create OpenGL context
  SDL_GLContext context = SDL_GL_CreateContext(window);

  // This forces GLEW to use a modern OpenGL method for checking if a function is available
  // This must come after the creation of the SDL window and OpenGL context
  glewExperimental = GL_TRUE;
  glewInit();

  if (!window) {
    std::cout << "Could not open the window: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Enter the event loop and handle a few key events
  SDL_Event event;
  int numEvents = 0;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
        logEvent(event);
        numEvents++;
      }

      if (numEvents > 2) running = false;
    }

    // This swaps the back and front buffers on the screen for when we start to draw
    SDL_GL_SwapWindow(window);
  }

  GLuint vertexBuffer;
  glGenBuffers(1, &vertexBuffer);

  printf("%u\n", vertexBuffer);

  // Main body where stuff can go down
  computeVertices();

  // Delete the OpenGL context, destrow the window, and let SDL know it can clean up
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
