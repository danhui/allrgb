#include <cstdio>
#include <GetOpt.h>

#include "color.h"
#include "event.h"
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
  Event event;
  while (event.value_ != kEscapeCode) {
    event = display->EventPoll();
#ifdef DEBUG
    if (event.type_ != kNoEvent) {
      fprintf(stderr, "Event read: %d %d\n", event.type_, event.value_);
    }
#endif
    display->DrawRectangle(2048, 2048, 315, 234, Color(255,0,0));
    display->HandleEvent(event);
    display->Display();
  }
  return 0;
}
