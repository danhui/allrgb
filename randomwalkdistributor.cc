#include "randomwalkdistributor.h"

#include <cstring>

#include "color.h"
#include "distributor.h"
#include "point.h"

void RandomWalkDistributor::Init() {
  memset(map_used_, 0, sizeof(map_used_));
  memset(points_open_, 0, sizeof(points_open_));
  memset(colors_used_, 0, sizeof(colors_used_));
}

Color RandomWalkDistributor::GetColor() {
  return Color(-1, -1, -1);
}

Color RandomWalkDistributor::GetColor(
    int p, int lr, int hr, int lg, int hg, int lb, int hb) {
  return Color(-1, -1, -1);
}

Point RandomWalkDistributor::GetPoint() {
  return Point(-1, -1);
}
