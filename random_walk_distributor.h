#ifndef __RANDOM_WALK_DISTRIBUTOR_H__
#define __RANDOM_WALK_DISTRIBUTOR_H__

#include <vector>

#include "color.h"
#include "distributor.h"
#include "graphics_engine.h"
#include "point.h"

class RandomWalkDistributor : public Distributor {
  public:
    virtual void Init(Color *c, Point *p);
    virtual void Query(Color *c, Point *p);
    virtual bool Done();

  private:
    Color map_state_[kMapWidth][kMapHeight];
    bool map_used_[kMapWidth][kMapHeight];
    int points_open_[9000][9000];

    int colors_used_[520][520][520];
    void colorSearch(
      int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi);
    void colorUpdate(
      int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi);

    Color prev_color_;
    std::vector<Color> candidates_;
    int color_range_;
};

#endif
