#ifndef __RANDOMWALKDISTRIBUTOR_H__
#define __RANDOMWALKDISTRIBUTOR_H__

#include "color.h"
#include "distributor.h"
#include "graphicsengine.h"
#include "point.h"

class RandomWalkDistributor : public Distributor {
  public:
    RandomWalkDistributor() { }
    ~RandomWalkDistributor() { }

    virtual void Init();
    virtual Color GetColor();
    virtual Point GetPoint();
  private:
    Color GetColor(int p, int lr, int hr, int lg, int hg, int lb, int hb);

    Color map_state_[kMapWidth][kMapHeight];
    bool map_used_[kMapWidth][kMapHeight];
    int points_open_[9000][9000];
    int colors_used_[520][520][520];
};

#endif
