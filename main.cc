#include <unistd.h>

#include "color.h"
#include "graphicsengine.h"
#include "sdlengine.h"

const int kWindowHeight = 4096;
const int kWindowWidth = 4096;

int main(int argc, char* argv[]) {
  char tmp;
  while ((tmp=getopt(argc,argv,"")) != -1) {
    switch (tmp) {
      default:
        break;
    }
  }
  GraphicsEngine* display = new SDLEngine();
  display->Init(kWindowHeight, kWindowWidth);
  return 0;
}
