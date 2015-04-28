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
  if (window_height < 0 || window_width < 0) {
    window_height = 480;
    window_width = 640;
  }
  display->Init(window_height, window_width);
  std::pair<int, int> event;
  while (event.second != kEscapeCode) {
    event = display->EventPoll();
#ifdef DEBUG
    if (event.first != kNoEvent) {
      fprintf(stderr, "Event read: %d %d\n", event.first, event.second);
    }
#endif
    //display->DrawRectangle(0,0,1000,1000,Color(0,0,255));
    //display->DrawRectangle(320,240,320,240,Color(0,255,0));
    display->Display();
  }
  return 0;
}
