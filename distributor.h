#pragma once

#include "color.h"
#include "point.h"

class Distributor {
  public:
    virtual void init(Color *c, Point *p) = 0;
    virtual void query(Color *c, Point *p) = 0;
    virtual bool done() = 0;
};
