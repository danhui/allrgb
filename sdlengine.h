#include <SDL2/SDL.h>

#include "color.h"
#include "graphicsengine.h"

#ifndef __SDLENGINE_H__
#define __SDLENGINE_H__

class SDLEngine : public GraphicsEngine {
  public:
    SDLEngine () {
      window_ = NULL;
      renderer_ = NULL;
    }
    ~SDLEngine {
      SDL_DestroyWindow(window_);
      SDL_DestroyRenderer(renderer_);
      SDL_Quit();
    }

    virtual void Init(int height, int width);
    virtual void DrawRectangle(int x1, int y1, int x2, int y2, Color c) = 0;
    virtual void DrawPoint(int x, int y, Color c);
    virtual int EventPoll();

  private:
    SDL_window* window_;
    SDL_renderer* renderer_;
};

#endif
