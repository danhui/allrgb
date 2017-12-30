#ifndef __RANDOM_WALK_DISTRIBUTOR_H__
#define __RANDOM_WALK_DISTRIBUTOR_H__

#include <vector>
#include <set>

#include "color.h"
#include "distributor.h"
#include "graphics_engine.h"
#include "point.h"

const double kRadiusRatio = 0.3;

const int kCandidate = 1;
const int kOccupied = 2;

class RandomWalkDistributor : public Distributor {
  public:
    virtual void init(Color *c, Point *p);
    virtual void query(Color *c, Point *p);
    virtual bool done();

  private:
    Color color_at_[kMapWidth + 1][kMapHeight + 1];
    int counter_[kMapWidth + 1][kMapHeight + 1];
    int status_[kMapWidth + 1][kMapHeight + 1];
    bool isOccupied(int x, int y);
    bool inBounds(int x, int y);

    std::set<Point> color_location_[kMaxColor + 1][kMaxColor + 1][kMaxColor + 1];

    int neighbour_spots_[520][520][520];
    int best_dist_;
    std::set<Point> new_points_;
    void findNeighbour(
      int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi);
    void updateNeighbour(
      int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi, int val);

    int colors_used_[520][520][520];
    void colorSearch(
      int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi);
    void colorUpdate(
      int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi);

    Color prev_color_;
    Point point_;
    std::vector<Color> candidates_;
    int color_range_;
};

#endif
