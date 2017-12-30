#pragma once

#include <ctime>
#include <map>
#include <SDL2/SDL.h>

#include "color.h"
#include "event.h"
#include "graphics_engine.h"

class SDLEngine : public GraphicsEngine {
  public:
    SDLEngine () {
      window_ = NULL;
      renderer_ = NULL;
      texture_ = NULL;
    }
    ~SDLEngine () {
      SDL_DestroyTexture(texture_);
      SDL_DestroyRenderer(renderer_);
      SDL_DestroyWindow(window_);
      SDL_Quit();
    }

    virtual void init(int height, int width);
    virtual void drawRectangle(int x, int y, int w, int h, Color c);
    virtual void drawPoint(int x, int y, Color c);
    virtual Event eventPoll();
    virtual void handleKeys(const std::map<int, int> &key_status);
    virtual void display();

  private:
    void adjustSpeed(
      const std::map<int,int> &key_status, int dir, int mult, int *v);

    SDL_Window* window_;
    int refresh_rate_;
    clock_t last_render_;
    clock_t last_key_handle_;
    SDL_Renderer* renderer_;

    SDL_Texture* texture_;
    int x_, y_, width_, height_;
    int vx_, vy_;
};
