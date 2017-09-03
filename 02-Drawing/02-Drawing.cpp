#include <iostream>
#include <string>
//#include <cstdlib>
//#include <ctime>

#define GLEW_STATIC
#include <GL/glew.h>

#include "SDL.h"
#include <SDL_opengl.h>

void logEvent(SDL_Event event) {
  std::cout << "Logging event with type: " << event.type << std::endl;
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

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  if (!window) {
    std::cout << "Could not open the window: " << SDL_GetError() << std::endl;
    return 1;
  }

  /**
   * OpenGL expects all vertices as a single array
   * so we must pack all the attributes for a single
   * vertex together, and then understand the format
   * of our array. In the following array we've made
   * our vertices more complex, carrying not only x & y
   * device-coordinate positions with them, but we're adding
   * an RGB aspect to each vertex as well. As a result, we'll
   * see the code that explains to OpenGL the format of each
   * vertex accommodate this.
   *
   * We can write our vertex array out in the exact order
   * in which we'll draw vertices to the screen, but this can
   * be wasteful. For example, to draw a rectangle from two triangles
   * (our GL primitive), we'll be duplicating two vertices. We can instead
   * just write out the unique vertices we'll need, and use an element buffer
   * object to refer to these unique vertices when drawing. This allows us to
   * reuse vertices instead of duplicate their memory, which can be really expensive
   * for complex 3D drawings
   */
  /*float vertices[] = {
    // Triangle 1
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1 Top left
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2 Top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.1f, // Vertex 3 Bottom right

    // Triangle 2
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1 Top left (duplicate vertex for second triangle)
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, // Vertex 2 Bottom left
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3 Bottom right (duplicate vertex for second triangle)
  };*/
  float vertices[] = {
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top Left
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top Right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom right
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom left
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
   * We can use an element buffer object to store indices of vertices that are
   * bound to an active vertex buffer object (GL_ARRAY_BUFFER). We could simply
   * refer to drawing these indices in the natural order we've defined them in our
   * vertex array, or we can draw complex shapes out of the vertices we've defined
   * in our vertex array. The element buffer object allows us to reuse these defined
   * vertices making our application more memory efficient
   */
  GLuint elements[] = {0, 1, 2, 2, 3, 0}; // reuse already defined vertices instead of duplicate memory

  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

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

    // This vector accommodates the position (x & y)
    // data for each vertex. The number after the vector
    // (2) tells us that that attribute has two values.
    in vec2 position;

    // This vector accommodates the color (r, g, b)
    // data for each vertex.
    in vec3 color;

    // The vertex shader can take in data and simply act
    // as a pass-through to get this data to the fragment
    // shader. That's what we're doing here with the color
    // data that comes in from the vertex data. We want to
    // give it to the fragment shader, so we'll output it.
    out vec3 Color;

    void main() {
      Color = color; // set output for fragment shader

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

  // We can also get the compile log which may contain useful information if not errors
  char shaderCompileBuffer[512];
  glGetShaderInfoLog(vertexShader, 512, NULL, shaderCompileBuffer);

  std::cout << "Vertex shader log: " << shaderCompileBuffer << std::endl;

  // Check the status
  if (status != GL_TRUE) {
    std::cerr << "Uh oh, the vertex shader failed to compile!!" << std::endl;
    return 1;
  }
 
  /**
   * The fragment shader takes the information output from the
   * vertex shader, called fragments. Like vertex shader, it has
   * a single mandatory output = final color of fragment. This shader
   * will just output a single color every time.
   */
  const char* fragmentSource = R"glsl(
    #version 150 core

    // Get ready to accept the color data
    // passed in by the vertex shader. The
    // name of this input MUST be the same as
    // the name of the vertex shader output.
    in vec3 Color;

    // uniform vec3 triangleColor; // if we decide to use uniform to change color
    out vec4 outColor;
    void main() {
      outColor = vec4(Color, 1);
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

  // Reuse our old status variable and log buffer to see if the fragment shader
  // compiled successfully, and with any messages
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  glGetShaderInfoLog(fragmentShader, 512, NULL, shaderCompileBuffer);

  std::cout << "Fragment shader log: " << shaderCompileBuffer << std::endl;

  if (status != GL_TRUE) {
    std::cerr << "Uh oh, the fragment shader failed to compile!!" << std::endl;
    return 1;
  }

  /**
   * The shaders we've made have only been designed to work together, but have no
   * real connection defined in our application. In order to to use them together
   * in the graphics pipeline, we have to create a shader program consisting of these
   * two shaders.
   */
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  /**
   * The line of code below is used to specify which output is written to
   * which buffer. This is set to 0 by default and since there is only one
   * output right now, the following line of code isn't necessary.
   */
  glBindFragDataLocation(shaderProgram, 0, "outColor");

  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram); // like a VBO, only one program can be active at a time

  /**
   * OpenGL still doesn't know the format of our vertex attributes, even though the
   * shaders now have a programatic link to each other given the `shaderProgram` they
   * are a part of. First we can grab a reference to the position input vector we defined
   * in our vertex shader. The GLint returned from glGetAttrubuteLocation will be an integer
   * and the first (and only) input definition will have the number 0.
   */
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position"); // 0
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
  glEnableVertexAttribArray(posAttrib);

  GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
  glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
  glEnableVertexAttribArray(colorAttrib);

  srand(time(NULL));
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

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Draw using element buffer object

    /**
     * Here if we want, we can grab a reference to the fragment shader's uniform
     * (if we were coloring this way) and set its color with our application code
     */
    // GLint fragUniformReference = glGetUniformLocation(shaderProgram, "triangleColor");
    // glUniform3f(fragUniformReference, ((double)rand()) / RAND_MAX, ((double)rand()) / RAND_MAX, ((double)rand()) / RAND_MAX);

    // This swaps the back and front buffers on the screen for when we start to draw
    SDL_GL_SwapWindow(window);
  }

  // Delete the OpenGL context, destrow the window, and let SDL know it can clean up
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
