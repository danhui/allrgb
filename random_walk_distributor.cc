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

void RandomWalkDistributor::init(Color *c, Point *p) {
  memset(color_at_, 0, sizeof(color_at_));
  memset(map_used_, 0, sizeof(map_used_));
  memset(color_location_, 0, sizeof(color_location_));
  memset(neighbour_spots_, 0, sizeof(neighbour_spots_));
  memset(colors_used_, 0, sizeof(colors_used_));

  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, kMaxColor);
  auto channel_value = std::bind(distribution, generator);
  *c = Color(channel_value(), channel_value(), channel_value());
  *p = Point(kMapWidth / 2, kMapHeight / 2);
  prev_color_ = *c;

  colorUpdate(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor);
  color_at_[p->getX()][p->getY()] = *c;
  map_used_[p->getX()][p->getY()] = true;
  color_location_[c->getR()][c->getG()][c->getB()] = *p;
  updateNeighbour(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor, 4);
}

bool RandomWalkDistributor::isOccupied(int x, int y) {
  if (!inBounds(x, y)) {
    return false;
  }
  return map_used_[x][y];
}

bool RandomWalkDistributor::inBounds(int x, int y) {
  if (x < 0 || x >= kMapWidth || y < 0 || y >= kMapHeight) {
    return false;
  }
  return true;
}

void RandomWalkDistributor::query(Color *c, Point *p) {
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

  candidates_.clear();
  color_range_ = 1;
  while (candidates_.empty()) {
    findNeighbour(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor);
    color_range_ *= 2;
  }
  seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(
    candidates_.begin(), candidates_.end(), std::default_random_engine(seed));
  Color tc = candidates_[0];
  Point from = color_location_[tc.getR()][tc.getG()][tc.getB()];
  int dx[] = {0, 0, -1 ,1};
  int dy[] = {-1, 1, 0, 0};
  std::vector<Point> new_points;
  for (int i = 0; i < 4; i++) {
    if (!isOccupied(dx[i] + from.getX(), dy[i] + from.getY()) &&
        inBounds(dx[i] + from.getX(), dy[i] + from.getY())) {
      new_points.push_back(Point(dx[i] + from.getX(), dy[i] + from.getY()));
    }
  }
  seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(
    new_points.begin(), new_points.end(), std::default_random_engine(seed));
  *p = new_points[0];
  int val = 0;
  for (int i = 0; i < 4; i++) {
    if (isOccupied(p->getX() + dx[i], p->getY() + dy[i]) &&
        inBounds(p->getX() + dx[i], p->getY() + dy[i])) {
      prev_color_ = color_at_[p->getX() + dx[i]][p->getY() + dy[i]];
      updateNeighbour(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor, -1);
    } else if (inBounds(p->getX() + dx[i], p->getY() + dy[i])) {
      val ++;
    }
  }
  color_at_[p->getX()][p->getY()] = *c;
  map_used_[p->getX()][p->getY()] = true;
  color_location_[c->getR()][c->getG()][c->getB()] = *p;
  prev_color_ = *c;
  updateNeighbour(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor, val);
}

void RandomWalkDistributor::findNeighbour(
  int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi
) {
  if (xlo > prev_color_.getR() + color_range_ ||
      xhi < prev_color_.getR() - color_range_ ||
      ylo > prev_color_.getG() + color_range_ ||
      yhi < prev_color_.getG() - color_range_ ||
      zlo > prev_color_.getB() + color_range_ ||
      zhi < prev_color_.getB() - color_range_ ||
      neighbour_spots_[px][py][pz] == 0) {
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
        findNeighbour(
          std::get<0>(x), std::get<0>(y), std::get<0>(z),
          std::get<1>(x), std::get<2>(x),
          std::get<1>(y), std::get<2>(y),
          std::get<1>(z), std::get<2>(z));
      }
    }
  }
}

void RandomWalkDistributor::updateNeighbour(
  int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi, int val
) {
  if (xlo > prev_color_.getR() ||
      xhi < prev_color_.getR() ||
      ylo > prev_color_.getG() ||
      yhi < prev_color_.getG() ||
      zlo > prev_color_.getB() ||
      zhi < prev_color_.getB()) {
    return;
  }
  neighbour_spots_[px][py][pz] += val;
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
        updateNeighbour(
          std::get<0>(x), std::get<0>(y), std::get<0>(z),
          std::get<1>(x), std::get<2>(x),
          std::get<1>(y), std::get<2>(y),
          std::get<1>(z), std::get<2>(z), val);
      }
    }
  }
}

void RandomWalkDistributor::colorSearch(
  int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi
) {
  if (xlo > prev_color_.getR() + color_range_ * 0.5 ||
      xhi < prev_color_.getR() - color_range_ * 0.5 ||
      ylo > prev_color_.getG() + color_range_ * 0.5 ||
      yhi < prev_color_.getG() - color_range_ * 0.5 ||
      zlo > prev_color_.getB() + color_range_ * 0.5 ||
      zhi < prev_color_.getB() - color_range_ * 0.5 ||
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
  if (xlo > prev_color_.getR() ||
      xhi < prev_color_.getR() ||
      ylo > prev_color_.getG() ||
      yhi < prev_color_.getG() ||
      zlo > prev_color_.getB() ||
      zhi < prev_color_.getB()) {
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

bool RandomWalkDistributor::done() {
  return colors_used_[0][0][0] == (kMaxColor + 1) * (kMaxColor + 1) * (kMaxColor + 1);
}
