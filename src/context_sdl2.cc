#include "context.h"
#include <cstdlib>
#include <iostream>

#ifdef USE_API_SDL2

#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>

static SDL_Window *   sdlWindow = nullptr;
static SDL_GLContext  glContext = nullptr;

void setupContext(int width, int height)
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,          1);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,              8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,            8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,             8);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,    1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,    8);

  sdlWindow = SDL_CreateWindow(
    "stlthumb",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    width, height,
    SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN
  );

  if (sdlWindow == nullptr) {
    std::cerr << "Could not create window" << std::endl;
    exit(EXIT_FAILURE);
  }

  glContext = SDL_GL_CreateContext(sdlWindow);
  
  if (glContext == nullptr) {
    SDL_DestroyWindow(sdlWindow);
    std::cerr << "Could not create context" << std::endl;
    exit(EXIT_FAILURE);
  }

  SDL_GL_MakeCurrent(sdlWindow, glContext);
}

void teardownContext()
{
  if (sdlWindow) SDL_GL_MakeCurrent(sdlWindow, nullptr);
  if (glContext) SDL_GL_DeleteContext(glContext);
  if (sdlWindow) SDL_DestroyWindow(sdlWindow);

  glContext = nullptr;
  sdlWindow = nullptr;
}

#endif
