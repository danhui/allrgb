#include <ctime>
#include <SDL2/SDL.h>

#include "color.h"
#include "event.h"
#include "graphicsengine.h"

#ifndef __SDLENGINE_H__
#define __SDLENGINE_H__

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

    virtual void Init(int height, int width);
    virtual void DrawRectangle(int x, int y, int w, int h, Color c);
    virtual void DrawPoint(int x, int y, Color c);
    virtual Event EventPoll();
    virtual void HandleEvent(Event event);
    virtual void Display();

  private:
    SDL_Window* window_;
    int refresh_rate_;
    clock_t last_render_;
    SDL_Renderer* renderer_;

    SDL_Texture* texture_;
    int x_, y_, width_, height_;
};

#endif
