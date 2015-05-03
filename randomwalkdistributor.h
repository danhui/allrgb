#ifndef __RANDOMWALKDISTRIBUTOR_H__
#define __RANDOMWALKDISTRIBUTOR_H__

#include "color.h"
#include "distributor.h"
#include "graphicsengine.h"
#include "point.h"

class RandomWalkDistributor : public Distributor {
  public:
    virtual void Init(Color *c, Point *p);
    virtual Color GetColor(Color prev_color);
    virtual void UpdateColor(Color cur_color);
    virtual Point GetPoint(Point prev_point);
    virtual void UpdatePoint(Point cur_point);

  private:

    Color map_state_[kMapWidth][kMapHeight];
    bool map_used_[kMapWidth][kMapHeight];
    int points_open_[9000][9000];
    int colors_used_[520][520][520];
};

#endif
