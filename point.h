#ifndef __POINT_H__
#define __POINT_H__

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
    int GetX() {
      return x_;
    }
    int GetY() {
      return y_;
    }
  private:
    int x_, y_;
};

#endif
