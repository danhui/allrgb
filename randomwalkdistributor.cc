#include "randomwalkdistributor.h"

#include <cstring>
#include <functional>
#include <random>

#include "color.h"
#include "debug.h"
#include "distributor.h"
#include "point.h"

void RandomWalkDistributor::Init(Color *c, Point *p) {
  memset(map_state_, 0, sizeof(map_state_));
  memset(map_used_, 0, sizeof(map_used_));
  memset(points_open_, 0, sizeof(points_open_));
  memset(colors_used_, 0, sizeof(colors_used_));
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 255);
  auto channel_value = std::bind(distribution, generator);
  *c = Color(channel_value(), channel_value(), channel_value());
  *p = Point(kMapWidth / 2, kMapHeight / 2);
  debug(1, "Initial color (%d,%d,%d) at point (%d,%d)",
      c->GetR(), c->GetG(), c->GetB(), p->GetX(), p->GetY());
}

Color RandomWalkDistributor::GetColor(Color prev_color) {
  return Color(-1, -1, -1);
}

void RandomWalkDistributor::UpdateColor(Color cur_color) {
}

Point RandomWalkDistributor::GetPoint(Point prev_point) {
  return Point(-1, -1);
}

void RandomWalkDistributor::UpdatePoint(Point cur_point) {
}
