#pragma once
#include <vector>

#include "color.h"
#include "distributor.h"
#include "graphics_engine.h"
#include "point.h"

class RandomDistributor : public Distributor {
  public:
    virtual void init(Color *c, Point *p);
    virtual void query(Color *c, Point *p);
    virtual bool done();

  private:
    std::vector<Color> color_remain_;
    std::vector<Point> point_remain_;
};
