#include "sdlengine.h"

#include <cstdio>
#include <ctime>
#include <SDL2/SDL.h>

#include "color.h"
#include "event.h"

void SDLEngine::Init(int height, int width) {
  width_ = width;
  height_ = height;
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);
  if (height_ < 0 || width_ < 0 || height_ > dm.h || width_ > dm.w) {
    height_ = dm.h / 2;
    width_ = dm.w / 2;
  }
  SDL_CreateWindowAndRenderer(width_, height_, 0, &window_, &renderer_);
  if (dm.refresh_rate > 0) {
    refresh_rate_ = dm.refresh_rate;
  } else {
    refresh_rate_ = 30;
  }
#ifdef DEBUG
  fprintf(stderr, "SDL Window %d by %d, %d Hz\n",
      width_, height_, refresh_rate_);
#endif
  texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGB888,
      SDL_TEXTUREACCESS_STREAMING, kMapWidth, kMapHeight);
  x_ = kMapWidth / 2 - width_ / 2;
  y_ = kMapHeight / 2 - height_ / 2;
  last_render_ = clock();
}

void SDLEngine::DrawRectangle(int x, int y, int w, int h, Color c) {
  int *pixels = NULL;
  int pitch;
  SDL_Rect rect = {x, y, w, h};
  SDL_LockTexture(texture_, &rect, reinterpret_cast<void **>(&pixels), &pitch);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      pixels[i * kMapWidth + j] = (c.r_ << 16) + (c.g_ << 8) + c.b_;
    }
  }
  SDL_UnlockTexture(texture_);
}

void SDLEngine::DrawPoint(int x, int y, Color c) {
  int *pixels = NULL;
  int pitch;
  SDL_Rect rect = {x, y, 1, 1};
  SDL_LockTexture(texture_, &rect, reinterpret_cast<void **>(&pixels), &pitch);
  pixels[0] = (c.r_ << 16) + (c.g_ << 8) + c.b_;
  SDL_UnlockTexture(texture_);
}

Event SDLEngine::EventPoll() {
  SDL_Event e;
  if (SDL_PollEvent(&e)) {
    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
      int type = e.type == SDL_KEYDOWN ? kKeyDown : kKeyUp;
      switch (e.key.keysym.sym) {
        case SDLK_LEFT:
          return Event(type, kArrowLeft);
        case SDLK_RIGHT:
          return Event(type, kArrowRight);
        case SDLK_UP:
          return Event(type, kArrowUp);
        case SDLK_DOWN:
          return Event(type, kArrowDown);
        default:
          return Event(type, e.key.keysym.sym);
      }
    }
  }
  return Event(kNoEvent, 0);
}

void SDLEngine::HandleEvent(Event event) {
}

void SDLEngine::Display() {
  clock_t now = clock();
  double diff = (double) (now - last_render_) / CLOCKS_PER_SEC;
  if (diff >= 1.0 / refresh_rate_) {
    SDL_RenderClear(renderer_);
    SDL_Rect srcrect = {x_, y_, width_, height_};
    SDL_RenderCopy(renderer_, texture_, &srcrect, NULL);
    SDL_RenderPresent(renderer_);
    last_render_ = now;
  }
}
