#include <cstdio>
#include <getopt.h>
#include <map>

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
  display->DrawRectangle(2048, 2048, 315, 234, Color(255,0,0));
  Event event;
  std::map<int, int> key_status;
  clock_t last_key_handle = clock();
  while (event.GetValue() != kEscapeCode) {
    event = display->EventPoll();
    key_status[event.GetValue()] = event.GetType();
    if (event.GetType() != kNoEvent) {
#ifdef DEBUG
      fprintf(stderr, "Event read: %d %d\n", event.GetType(), event.GetValue());
#endif
    }
    double diff = (double) (clock() - last_key_handle ) / CLOCKS_PER_SEC;
    if (diff >= kKeyProcess) {
      display->HandleKeys(&key_status);
      last_key_handle = clock();
    }
    display->Display();
  }
  return 0;
}
