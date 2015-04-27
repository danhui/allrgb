#include "sdlengine.h"

#include <cstdio>
#include <iostream>
#include <SDL2/SDL.h>

#include "color.h"

void SDLEngine::Init(int height, int width) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);
  if (height < 0 || width < 0 || height > dm.h || width > dm.w) {
    height = dm.h;
    width = dm.w;
  }
  SDL_CreateWindowAndRenderer(width, height, 0, &window_, &renderer_);
  if (dm.refresh_rate > 0) {
    refresh_rate_ = dm.refresh_rate;
  } else {
    refresh_rate_ = 30;
  }
#ifdef DEBUG
  refresh_rate_ = 30;
  fprintf(stderr, "SDL Window %d by %d, %d Hz\n", width, height, refresh_rate_);
  //SDL_Delay(1000);
#endif
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

std::pair<int, int> SDLEngine::EventPoll() {
  SDL_Event e;
  if (SDL_PollEvent(&e)) {
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_LEFT:
          return std::make_pair(kKeyDown, kArrowLeft);
        case SDLK_RIGHT:
          return std::make_pair(kKeyDown, kArrowRight);
        case SDLK_UP:
          return std::make_pair(kKeyDown, kArrowUp);
        case SDLK_DOWN:
          return std::make_pair(kKeyDown, kArrowDown);
        default:
          return std::make_pair(kKeyDown, e.key.keysym.sym);
      }
    }
  }
  return std::make_pair(kNoEvent, 0);
}

void SDLEngine::Display() {
  time_t now = time(0);
  if (now - last_render_ >= 1.0 / refresh_rate_) {
    last_render_ = now;
    SDL_RenderClear(renderer_);
    SDL_RenderPresent(renderer_);
  }
}
