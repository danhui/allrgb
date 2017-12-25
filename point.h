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
    int getX() const {
      return x_;
    }
    int getY() const {
      return y_;
    }
    bool operator<(const Point &p) const {
      return (x_ < p.getX()) || (x_ == p.getX()) && (y_ < p.getY());
    }
  private:
    int x_, y_;
};
