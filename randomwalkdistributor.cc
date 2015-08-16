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
}

void RandomWalkDistributor::Query(Color *c, Point *p) {
  prev_color_ = *c;
  prev_point_ = *p;
}

bool RandomWalkDistributor::Done() {
  return true;
}
