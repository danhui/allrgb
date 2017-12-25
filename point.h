#pragma once

class Point {
  public:
    Point() {
      x_ = 0;
      y_ = 0;
    }
    Point(int x, int y) {
      x_ = x;
      y_ = y;
    }
    int GetX() const {
      return x_;
    }
    int GetY() const {
      return y_;
    }
    bool operator<(const Point &p) const {
      return (x_ < p.GetX()) || (x_ == p.GetX()) && (y_ < p.GetY());
    }
  private:
    int x_, y_;
};
