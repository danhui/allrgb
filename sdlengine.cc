#include "sdlengine.h"

#include <SDL2/SDL.h>

#include "color.h"

void SDLEngine::Init(int height, int width) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(width, height, 0, &window_,
      &renderer_);
}

void SDLEngine::DrawRectangle(int x1, int y1, int x2, int y2, Color c) {
}

void SDLEngine::DrawPoint(int x, int y, Color c) {
}

int SDLEngine::EventPoll() {
  SDL_event e;
  if (SDL_PollEvent(&e)) {
    if (e.type == SDL_KEYDOWN) {
      if (e.key.keysym.sym == 27) {
        return kEscapeCode;
      }
      else {
        return kKeyPressed;
      }
    }
  }
  return kNoEvent;
}
