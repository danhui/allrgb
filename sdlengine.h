#ifndef __SDLENGINE_H__
#define __SDLENGINE_H__

#include <ctime>
#include <map>
#include <SDL2/SDL.h>

#include "color.h"
#include "event.h"
#include "graphicsengine.h"

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
    virtual void HandleKeys(const std::map<int, int> &key_status);
    virtual void Display();

  private:
    void AdjustSpeed(const std::map<int,int> &key_status, int dir, int mult,
                     int *v);

    SDL_Window* window_;
    int refresh_rate_;
    clock_t last_render_;
    SDL_Renderer* renderer_;

    SDL_Texture* texture_;
    int x_, y_, width_, height_;
    int vx_, vy_;
};

#endif
