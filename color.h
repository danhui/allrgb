#ifndef __COLOR_H__
#define __COLOR_H__

// Since rendering might change from SDL2, a custom Color class was used.

const int kMaxColor = 255;

class Color {
  public:
    Color (int r, int g, int b) {
      r_ = r;
      g_ = g;
      b_ = b;
      a_ = 255;
    }
    Color (int r, int g, int b, int a) {
      r_ = r;
      g_ = g;
      b_ = b;
      a_ = a;
    }
    Color () {
      r_ = 0;
      g_ = 0;
      b_ = 0;
      a_ = 255;
    }
    int getR() {
      return r_;
    }
    int getG() {
      return g_;
    }
    int getB() {
      return b_;
    }
    int getA() {
      return a_;
    }
  private:
    int r_, g_, b_, a_;
};

#endif
