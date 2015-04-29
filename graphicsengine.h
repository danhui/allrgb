#include <map>
#include <utility>

#include "color.h"
#include "event.h"

#ifndef __GRAPHICSENGINE_H__
#define __GRAPHICSENGINE_H__

// Possible Event types.
const int kNoEvent = 0;
const int kKeyDown = 1;
const int kKeyUp = 2;

// Modified a few Event values since they were arbitrary and large.
const int kArrowUp = -1;
const int kArrowDown = -2;
const int kArrowLeft = -3;
const int kArrowRight = -4;
const int kEscapeCode = 27;

// 4096^2 == 256^3 (for all rgb colours to be displayed exactly once).
// Might be changed in the future for other applications.
const int kMapHeight = 4096;
const int kMapWidth = 4096;

// Max pixel speed for both directions.
const int kMaxSpeed = 25;

// How often the keyboard state should be processed (seconds).
const double kKeyProcess = 0.05;

class GraphicsEngine {
  public:
    virtual void Init(int height, int width) = 0;
    virtual void DrawRectangle(int x, int y, int w, int h, Color c) = 0;
    virtual void DrawPoint(int x, int y, Color c) = 0;
    virtual Event EventPoll() = 0;
    virtual void HandleKeys(std::map<int, int> *key_status) = 0;
    virtual void Display() = 0;
};

#endif
