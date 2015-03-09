#include "sdlengine.h"

#include <SDL2/SDL.h>

#include "color.h"

void SDLEngine::Init(int height, int width) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(width, height, 0, &window_,
      &renderer_);
}

void SDLEngine::DrawRectangle(int x1, int y1, int x2, int y2, Color c) {
  SDL_SetRenderDrawColor(renderer_, c.r_, c.g_, c.b_, c.a_);
  SDL_Rect rectangle;
  rectangle.x = x1;
  rectangle.y = y1;
  rectangle.w = x2-x1+1;
  rectangle.h = y2-y1+1;
  SDL_RenderFillRect(renderer_, &rectangle);
}

void SDLEngine::DrawPoint(int x, int y, Color c) {
  SDL_SetRenderDrawColor(renderer_, c.r_, c.g_, c.b_, c.a_);
  SDL_RenderDrawPoint(renderer_, x, y);
}

int SDLEngine::EventPoll() {
  SDL_Event e;
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
