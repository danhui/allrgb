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
#endif
  texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGB888,
      SDL_TEXTUREACCESS_STREAMING, kMapWidth, kMapHeight);
}

void SDLEngine::DrawRectangle(int x, int y, int w, int h, Color c) {
  void *pixels = NULL;
  int pitch;
  SDL_Rect rect = {x, y, w, h};
  SDL_LockTexture(texture_, &rect, &pixels, &pitch);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      ((uint32_t *)pixels)[i * kMapWidth + j] =
        (c.r_ << 16) + (c.g_ << 8) + c.b_;
    }
  }
  SDL_UnlockTexture(texture_);
}

void SDLEngine::DrawPoint(int x, int y, Color c) {
  void *pixels = NULL;
  int pitch;
  SDL_Rect rect = {x, y, 1, 1};
  SDL_LockTexture(texture_, &rect, &pixels, &pitch);
  ((uint32_t *)pixels)[0] = (c.r_ << 16) + (c.g_ << 8) + c.b_;
  SDL_UnlockTexture(texture_);
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
    SDL_Rect srcrect;
    srcrect.x = 0;
    srcrect.y = 0;
    SDL_GetRendererOutputSize(renderer_, &srcrect.w, &srcrect.h);
    SDL_RenderCopy(renderer_, texture_, &srcrect, NULL);
    SDL_RenderPresent(renderer_);
  }
}
