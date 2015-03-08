#include "color.h"

#ifndef __GRAPHICSENGINE_H__
#define __GRAPHICSENGINE_H__

const int kEscapeCode = 27;
const int kKeyPressed = 1;
const int kNoEvent = 0;

class GraphicsEngine {
  public:
    virtual void Init(int height, int width) = 0;
    virtual void DrawRectangle(int x1, int y1, int x2, int y2, Color c) = 0;
    virtual void DrawPoint(int x, int y, Color c) = 0;
    virtual int EventPoll() = 0;
};

#endif
