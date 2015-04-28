#include <utility>

#include "color.h"
#include "event.h"

#ifndef __GRAPHICSENGINE_H__
#define __GRAPHICSENGINE_H__

const int kNoEvent = 0;
const int kKeyDown = 1;
const int kKeyUp = 2;

const int kArrowUp = -1;
const int kArrowDown = -2;
const int kArrowLeft = -3;
const int kArrowRight = -4;

const int kEscapeCode = 27;

const int kMapHeight = 4096;
const int kMapWidth = 4096;

class GraphicsEngine {
  public:
    virtual void Init(int height, int width) = 0;
    virtual void DrawRectangle(int x, int y, int w, int h, Color c) = 0;
    virtual void DrawPoint(int x, int y, Color c) = 0;
    virtual Event EventPoll() = 0;
    virtual void HandleEvent(Event event) = 0;
    virtual void Display() = 0;
};

#endif
