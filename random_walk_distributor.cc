#include "random_walk_distributor.h"

#include <cstring>
#include <functional>
#include <random>
#include <chrono>
#include <tuple>
#include <algorithm>

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
  std::uniform_int_distribution<int> distribution(0, kMaxColor);
  auto channel_value = std::bind(distribution, generator);
  *c = Color(channel_value(), channel_value(), channel_value());
  *p = Point(kMapWidth / 2, kMapHeight / 2);
  prev_color_ = *c;
  colorUpdate(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor);
}

void RandomWalkDistributor::Query(Color *c, Point *p) {
  color_range_ = 2;
  candidates_.clear();
  while (candidates_.empty()) {
    colorSearch(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor);
    color_range_ *= 2;
  }
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(
    candidates_.begin(), candidates_.end(), std::default_random_engine(seed));
  *c = candidates_[0];
  prev_color_ = *c;
  colorUpdate(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor);
}

void RandomWalkDistributor::colorSearch(
  int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi
) {
  if (xlo > prev_color_.GetR() + color_range_ ||
      xhi < prev_color_.GetR() - color_range_ ||
      ylo > prev_color_.GetG() + color_range_ ||
      yhi < prev_color_.GetG() - color_range_ ||
      zlo > prev_color_.GetB() + color_range_ ||
      zhi < prev_color_.GetB() - color_range_ ||
      colors_used_[px][py][pz] == (xhi - xlo + 1) * (yhi - ylo + 1) * (zhi - zlo + 1)) {
    return;
  }
  if (xlo == xhi && ylo == yhi && zlo == zhi) {
    candidates_.push_back(Color(xlo, ylo, zlo));
    return;
  }
  for (auto x :
       {std::make_tuple(px * 2 + 1, xlo, (xlo + xhi) / 2),
        std::make_tuple(px * 2 + 2, (xlo + xhi) / 2 + 1, xhi)}) {
    for (auto y :
         {std::make_tuple(py * 2 + 1, ylo, (ylo + yhi) / 2),
          std::make_tuple(py * 2 + 2, (ylo + yhi) / 2 + 1, yhi)}) {
      for (auto z :
           {std::make_tuple(pz * 2 + 1, zlo, (zlo + zhi) / 2),
            std::make_tuple(pz * 2 + 2, (zlo + zhi) / 2 + 1, zhi)}) {
        colorSearch(
          std::get<0>(x), std::get<0>(y), std::get<0>(z),
          std::get<1>(x), std::get<2>(x),
          std::get<1>(y), std::get<2>(y),
          std::get<1>(z), std::get<2>(z));
      }
    }
  }
}

void RandomWalkDistributor::colorUpdate(
  int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi
) {
  if (xlo > prev_color_.GetR() ||
      xhi < prev_color_.GetR() ||
      ylo > prev_color_.GetG() ||
      yhi < prev_color_.GetG() ||
      zlo > prev_color_.GetB() ||
      zhi < prev_color_.GetB()) {
    return;
  }
  colors_used_[px][py][pz] ++;
  if (xlo == xhi && ylo == yhi && zlo == zhi) {
    return;
  }
  for (auto x :
       {std::make_tuple(px * 2 + 1, xlo, (xlo + xhi) / 2),
        std::make_tuple(px * 2 + 2, (xlo + xhi) / 2 + 1, xhi)}) {
    for (auto y :
         {std::make_tuple(py * 2 + 1, ylo, (ylo + yhi) / 2),
          std::make_tuple(py * 2 + 2, (ylo + yhi) / 2 + 1, yhi)}) {
      for (auto z :
           {std::make_tuple(pz * 2 + 1, zlo, (zlo + zhi) / 2),
            std::make_tuple(pz * 2 + 2, (zlo + zhi) / 2 + 1, zhi)}) {
        colorUpdate(
          std::get<0>(x), std::get<0>(y), std::get<0>(z),
          std::get<1>(x), std::get<2>(x),
          std::get<1>(y), std::get<2>(y),
          std::get<1>(z), std::get<2>(z));
      }
    }
  }
}

bool RandomWalkDistributor::Done() {
  return colors_used_[0][0][0] == (kMaxColor + 1) * (kMaxColor + 1) * (kMaxColor + 1);
}
