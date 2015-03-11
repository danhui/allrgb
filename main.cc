#include <cstdio>
#include <GetOpt.h>

#include "color.h"
#include "graphicsengine.h"
#include "sdlengine.h"

int main(int argc, char* argv[]) {
  int window_height = -1;
  int window_width = -1;
  int option_char;
  while ((option_char = getopt(argc, argv, "w:h:")) != EOF) {
    switch (option_char) {
      case 'w': window_width = atoi(optarg); break;
      case 'h': window_height = atoi(optarg); break;
      default:
        break;
    }
  }
  GraphicsEngine* display = new SDLEngine();
#ifdef DEBUG
  if (window_height < 0 && window_width < 0) {
    window_height = 480;
    window_width = 640;
  }
#endif
  display->Init(window_height, window_width);
  return 0;
}
