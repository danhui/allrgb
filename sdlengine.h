#include <ctime>
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
    ~SDLEngine () {
      SDL_DestroyWindow(window_);
      SDL_DestroyRenderer(renderer_);
      SDL_Quit();
    }

    virtual void Init(int height, int width);
    virtual void DrawRectangle(int x1, int y1, int x2, int y2, Color c);
    virtual void DrawPoint(int x, int y, Color c);
    virtual int EventPoll();
    virtual void Display();

  private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    SDL_Texture* texture_;
    int refresh_rate_;
    time_t last_render_;
};

#endif
