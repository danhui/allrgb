#ifndef __COLOR_H__
#define __COLOR_H__

class Color {
  public:
    Color (int r, int g, int b) {
      this->r = r;
      this->g = g;
      this->b = b;
    }
    Color () {
      r = 0;
      g = 0;
      b = 0;
    }
    int r, g, b;
};

#endif
