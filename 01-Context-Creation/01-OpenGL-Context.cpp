#include <iostream>

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

  // Delete the OpenGL context, destrow the window, and let SDL know it can clean up
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
