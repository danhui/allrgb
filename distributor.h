#ifndef __DISTRIBUTOR_H__
#define __DISTRIBUTOR_H__

#include "color.h"
#include "point.h"

class Distributor {
  public:
    virtual void Init() = 0;
    virtual Color GetColor() = 0;
    virtual Point GetPoint() = 0;
};

#endif
