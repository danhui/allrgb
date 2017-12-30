#include "random_walk_distributor.h"

#include <cstring>
#include <functional>
#include <random>
#include <chrono>
#include <tuple>
#include <algorithm>
#include <limits>
#include <cmath>

#include "color.h"
#include "debug.h"
#include "distributor.h"
#include "point.h"

void RandomWalkDistributor::init(Color *c, Point *p) {
  memset(color_at_, 0, sizeof(color_at_));
  memset(counter_, 0, sizeof(counter_));
  memset(status_, 0, sizeof(status_));
  memset(neighbour_spots_, 0, sizeof(neighbour_spots_));
  memset(colors_used_, 0, sizeof(colors_used_));
  for (int i = 0; i <= kMaxColor; i++) {
    for (int j = 0; j <= kMaxColor; j++) {
      for (int k = 0; k <= kMaxColor; k++) {
        color_location_[i][j][k].clear();
      }
    }
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(0, kMaxColor);
  auto channel_value = std::bind(distribution, generator);
  *c = Color(channel_value(), channel_value(), channel_value());
  *p = Point(kMapWidth / 2, kMapHeight / 2);
  prev_color_ = *c;

  colorUpdate(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor);
  color_at_[p->getX()][p->getY()] = *c;
  status_[p->getX()][p->getY()] = kOccupied;
  int dx[] = {0, 0, -1 ,1};
  int dy[] = {-1, 1, 0, 0};
  for (int i = 0; i < 4; i++) {
    int tx = p->getX() + dx[i];
    int ty = p->getY() + dy[i];
    if (!inBounds(tx, ty)) {
      continue;
    }
    counter_[tx][ty] = 1;
    color_at_[tx][ty] = *c;
    color_location_[c->getR()][c->getG()][c->getB()].insert(Point(tx, ty));
    status_[tx][ty] = kCandidate;
    updateNeighbour(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor, 1);
  }
}

bool RandomWalkDistributor::isOccupied(int x, int y) {
  if (!inBounds(x, y)) {
    return false;
  }
  return status_[x][y] == kOccupied;
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

  *c = candidates_[0];
  prev_color_ = *c;
  colorUpdate(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor);

  new_points_.clear();
  best_dist_ = 100;
  while (new_points_.empty()) {
    findNeighbour(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor);
    best_dist_ *= 2;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, new_points_.size() - 1);
  std::set<Point>::iterator it(new_points_.begin());
  std::advance(it, dis(gen));
  *p = *it;

  int dx[] = {0, 0, -1 ,1};
  int dy[] = {-1, 1, 0, 0};
  for (int i = 0; i < 4; i++) {
    int tx = p->getX() + dx[i];
    int ty = p->getY() + dy[i];
    Color tc = color_at_[tx][ty];
    if (isOccupied(tx, ty) || !inBounds(tx, ty)) {
      continue;
    }
    if (status_[tx][ty] == kCandidate) {
      color_location_[tc.getR()][tc.getG()][tc.getB()].erase(Point(tx, ty));
      prev_color_ = tc;
      updateNeighbour(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor, -1);
    }
    color_at_[tx][ty] = Color(
      (tc.getR() * counter_[tx][ty] + c->getR()) / (counter_[tx][ty] + 1),
      (tc.getG() * counter_[tx][ty] + c->getG()) / (counter_[tx][ty] + 1),
      (tc.getB() * counter_[tx][ty] + c->getB()) / (counter_[tx][ty] + 1));
    counter_[tx][ty] ++;
    tc = color_at_[tx][ty];
    color_location_[tc.getR()][tc.getG()][tc.getB()].insert(Point(tx, ty));
    prev_color_ = tc;
    updateNeighbour(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor, 1);
    status_[tx][ty] = kCandidate;
  }
  assert(status_[p->getX()][p->getY()] == kCandidate);
  Color tc = color_at_[p->getX()][p->getY()];
  color_location_[tc.getR()][tc.getG()][tc.getB()].erase(*p);
  prev_color_ = tc;
  updateNeighbour(0, 0, 0, 0, kMaxColor, 0, kMaxColor, 0, kMaxColor, -1);
  color_at_[p->getX()][p->getY()] = *c;
  status_[p->getX()][p->getY()] = kOccupied;
  prev_color_ = *c;
}

void RandomWalkDistributor::findNeighbour(
  int px, int py, int pz, int xlo, int xhi, int ylo, int yhi, int zlo, int zhi
) {
  if (xlo > prev_color_.getR() + best_dist_ * kRadiusRatio ||
      xhi < prev_color_.getR() - best_dist_ * kRadiusRatio ||
      ylo > prev_color_.getG() + best_dist_ * kRadiusRatio ||
      yhi < prev_color_.getG() - best_dist_ * kRadiusRatio ||
      zlo > prev_color_.getB() + best_dist_ * kRadiusRatio ||
      zhi < prev_color_.getB() - best_dist_ * kRadiusRatio ||
      neighbour_spots_[px][py][pz] == 0) {
    return;
  }
  if (xlo == xhi && ylo == yhi && zlo == zhi) {
    int dist = abs(xlo - prev_color_.getR()) +
      abs(ylo - prev_color_.getG()) +
      abs(zlo - prev_color_.getB());
    if (dist < best_dist_) {
      new_points_.clear();
      best_dist_ = dist;
    }
    if (dist == best_dist_) {
      new_points_.insert(
        color_location_[xlo][ylo][zlo].begin(),
        color_location_[xlo][ylo][zlo].end());
    }
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
  if (xlo > prev_color_.getR() + color_range_ ||
      xhi < prev_color_.getR() - color_range_ ||
      ylo > prev_color_.getG() + color_range_ ||
      yhi < prev_color_.getG() - color_range_ ||
      zlo > prev_color_.getB() + color_range_ ||
      zhi < prev_color_.getB() - color_range_ ||
      candidates_.size() > 0 ||
      colors_used_[px][py][pz] == (xhi - xlo + 1) * (yhi - ylo + 1) * (zhi - zlo + 1)) {
    return;
  }
  if (xlo == xhi && ylo == yhi && zlo == zhi) {
    candidates_.push_back(Color(xlo, ylo, zlo));
    return;
  }
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(0, 1);
  auto channel_value = std::bind(distribution, generator);
  std::tuple<int, int, int> ax[] = {
    std::make_tuple(px * 2 + 1, xlo, (xlo + xhi) / 2),
    std::make_tuple(px * 2 + 2, (xlo + xhi) / 2 + 1, xhi)};
  if (channel_value()) {
    std::swap(ax[0], ax[1]);
  }
  std::tuple<int, int, int> ay[] = {
    std::make_tuple(py * 2 + 1, ylo, (ylo + yhi) / 2),
    std::make_tuple(py * 2 + 2, (ylo + yhi) / 2 + 1, yhi)};
  if (channel_value()) {
    std::swap(ay[0], ay[1]);
  }
  std::tuple<int, int, int> az[] = {
    std::make_tuple(pz * 2 + 1, zlo, (zlo + zhi) / 2),
    std::make_tuple(pz * 2 + 2, (zlo + zhi) / 2 + 1, zhi)};
  if (channel_value()) {
    std::swap(az[0], az[1]);
  }
  for (auto x : ax) {
    for (auto y : ay) {
      for (auto z : az) {
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
