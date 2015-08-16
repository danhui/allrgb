#ifndef __DISTRIBUTOR_H__
#define __DISTRIBUTOR_H__

#include "color.h"
#include "point.h"

class Distributor {
  public:
    virtual void Init(Color *c, Point *p) = 0;
    virtual void Query(Color *c, Point *p) = 0;
    virtual bool Done() = 0;
};

#endif
