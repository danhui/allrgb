#include "random_distributor.h"

#include <algorithm>
#include <chrono>
#include <random>

void RandomDistributor::init(Color *c, Point *p) {
  for (int i = 0; i <= kMaxColor ; i++) {
    for (int j = 0; j <= kMaxColor; j++) {
      for (int k = 0; k <= kMaxColor; k++) {
        color_remain_.push_back(Color(i, j, k));
      }
    }
  }
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(color_remain_.begin(), color_remain_.end(),
      std::default_random_engine(seed));
  for (int i = 0; i < kMapWidth; i++) {
    for (int j = 0; j < kMapHeight; j++) {
      point_remain_.push_back(Point(i, j));
    }
  }
  seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(point_remain_.begin(), point_remain_.end(),
      std::default_random_engine(seed));
  query(c, p);
}

void RandomDistributor::query(Color *c, Point *p) {
  *c = color_remain_.back();
  color_remain_.pop_back();
  *p = point_remain_.back();
  point_remain_.pop_back();
}

bool RandomDistributor::done() {
  return (color_remain_.size() == 0) && (point_remain_.size() == 0);
}
