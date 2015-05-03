#ifndef __DISTRIBUTOR_H__
#define __DISTRIBUTOR_H__

#include "color.h"
#include "point.h"

class Distributor {
  public:
    virtual void Init(Color *c, Point *p) = 0;
    virtual Color GetColor(Color prev_color) = 0;
    virtual void UpdateColor(Color cur_color) = 0;
    virtual Point GetPoint(Point prev_point) = 0;
    virtual void UpdatePoint(Point cur_point) = 0;
};

#endif
