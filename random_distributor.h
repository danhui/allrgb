#ifndef __RANDOM_DISTRIBUTOR_H__
#define __RANDOM_DISTRIBUTOR_H__

#include <vector>

#include "color.h"
#include "distributor.h"
#include "graphics_engine.h"
#include "point.h"

class RandomDistributor : public Distributor {
  public:
    virtual void Init(Color *c, Point *p);
    virtual void Query(Color *c, Point *p);
    virtual bool Done();

  private:
    std::vector<Color> color_remain_;
    std::vector<Point> point_remain_;
};

#endif
