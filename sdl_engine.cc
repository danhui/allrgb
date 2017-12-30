#include "sdl_engine.h"

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <map>
#include <SDL2/SDL.h>

#include "color.h"
#include "debug.h"
#include "event.h"
#include "graphics_engine.h"

void SDLEngine::init(int height, int width) {
  width_ = width;
  height_ = height;
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);
  // Make sure the desired dimensions don't exceed those of the screen.
  if (height_ < 0 || width_ < 0 || height_ > dm.h || width_ > dm.w) {
    height_ = dm.h / 2;
    width_ = dm.w / 2;
  }
  SDL_CreateWindowAndRenderer(width_, height_, 0, &window_, &renderer_);
  // Determine refresh rate or default to 30 Hz.
  if (dm.refresh_rate > 0) {
    refresh_rate_ = dm.refresh_rate;
  } else {
    refresh_rate_ = 30;
  }
  debug(1, "SDL Window %d by %d, %d Hz\n", width_, height_,
      refresh_rate_);
  // Texture is full size, but only parts of it will be shown.
  texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGB888,
      SDL_TEXTUREACCESS_STREAMING, kMapWidth, kMapHeight);
  // Start near the center.
  // Note that x_ and y_ here represents the top-left corner of the viewing area
  // aka the renderer + window pair.
  x_ = kMapWidth / 2 - width_ / 2;
  y_ = kMapHeight / 2 - height_ / 2;
  vx_ = 0;
  vy_ = 0;
  last_render_ = clock();
  last_key_handle_ = clock();
}

void SDLEngine::drawRectangle(int x, int y, int w, int h, Color c) {
  int *pixels = NULL;
  int pitch;
  SDL_Rect rect = {x, y, w, h};
  SDL_LockTexture(texture_, &rect, reinterpret_cast<void **>(&pixels), &pitch);
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      pixels[i * kMapWidth + j] = (c.getR() << 16) + (c.getG() << 8) + c.getB();
    }
  }
  SDL_UnlockTexture(texture_);
}

void SDLEngine::drawPoint(int x, int y, Color c) {
  int *pixels = NULL;
  int pitch;
  SDL_Rect rect = {x, y, 1, 1};
  SDL_LockTexture(texture_, &rect, reinterpret_cast<void **>(&pixels), &pitch);
  pixels[0] = (c.getR() << 16) + (c.getG() << 8) + c.getB();
  SDL_UnlockTexture(texture_);
}

Event SDLEngine::eventPoll() {
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

void SDLEngine::adjustSpeed(const std::map<int,int> &key_status, int dir,
                            int mult, int *v) {
  if (key_status.find(dir)->second == kKeyDown) {
    // Key down, so increase velocity.
    *v += mult;
  } else if (key_status.find(dir)->second == kKeyUp) {
    // Set velocity to 0 on release if going in correct direction.
    if (*v * mult > 0) {
      *v = 0;
    }
  }
}

void SDLEngine::handleKeys(const std::map<int, int> &key_status) {
  double diff = (double) (clock() - last_key_handle_) / CLOCKS_PER_SEC;
  if (diff < kKeyProcess) {
    return;
  }
  debug(10, "Last key process %.5f sec. ago\n", diff);
  adjustSpeed(key_status, kArrowUp, -1, &vy_);
  adjustSpeed(key_status, kArrowDown, 1, &vy_);
  adjustSpeed(key_status, kArrowLeft, -1, &vx_);
  adjustSpeed(key_status, kArrowRight, 1, &vx_);
  // Keep speeds within speed limit.
  vx_ = std::min(std::max(-kMaxSpeed, vx_), kMaxSpeed);
  vy_ = std::min(std::max(-kMaxSpeed, vy_), kMaxSpeed);
  x_ += vx_;
  y_ += vy_;
  // Stay on texture for vieweing.
  x_ = std::min(std::max(0, x_), kMapWidth - width_);
  y_ = std::min(std::max(0, y_), kMapHeight - height_);
  if (vx_ != 0 || vy_ != 0) {
    debug(5, "%d %d %d %d\n", x_, y_, vx_, vy_);
    display();
  }
  last_key_handle_ = clock();
}

void SDLEngine::display() {
  double diff = (double) (clock() - last_render_) / CLOCKS_PER_SEC;
  // Check refresh rate before redisplaying.
  if (diff < 1.0 / refresh_rate_) {
    return;
  }
  debug(10, "Last render %.5f sec. ago\n", diff);
  SDL_RenderClear(renderer_);
  // Display a part of the texture_.
  SDL_Rect srcrect = {x_, y_, width_, height_};
  SDL_RenderCopy(renderer_, texture_, &srcrect, NULL);
  SDL_RenderPresent(renderer_);
  last_render_ = clock();
}
