#include <cstdio>
#include <getopt.h>
#include <map>

#include "color.h"
#include "debug.h"
#include "distributor.h"
#include "event.h"
#include "graphicsengine.h"
#include "randomdistributor.h"
#include "randomwalkdistributor.h"
#include "sdlengine.h"

int main(int argc, char* argv[]) {
  int window_height = -1;
  int window_width = -1;
  int option_char;
  // Parse command-line arguments.
  while ((option_char = getopt(argc, argv, "w:h:")) != EOF) {
    switch (option_char) {
      case 'w': window_width = atoi(optarg); break;
      case 'h': window_height = atoi(optarg); break;
      default:
        break;
    }
  }
  GraphicsEngine *display = new SDLEngine();
  display->Init(window_height, window_width);
  display->DrawRectangle(0, 0, kMapWidth, kMapHeight, Color(255, 255, 255));
  //display->DrawRectangle(2048, 2048, 50, 50, Color(255, 0, 0));
  Distributor *distributor = new RandomDistributor();
  Color c;
  Point p;
  distributor->Init(&c, &p);
  display->DrawPoint(p.GetX(), p.GetY(), c);
  debug(1, "Initial color (%d,%d,%d) at point (%d,%d)\n",
      c.GetR(), c.GetG(), c.GetB(), p.GetX(), p.GetY());
  Event event;
  // Stores the status of keyboard keys.
  std::map<int, int> key_status;
  // Handle key press status at a constant interval.
  clock_t last_key_handle = clock();
  while (event.GetValue() != kEscapeCode) {
    if (!distributor->Done()) {
      distributor->Query(&c, &p);
      display->DrawPoint(p.GetX(), p.GetY(), c);
      debug(1, "Color (%d,%d,%d) at point (%d,%d)\n",
          c.GetR(), c.GetG(), c.GetB(), p.GetX(), p.GetY());
    }
    // Query for events, and update keyboard status.
    event = display->EventPoll();
    key_status[event.GetValue()] = event.GetType();
    if (event.GetType() != kNoEvent) {
      debug(4, "Event read: %d %d\n", event.GetType(), event.GetValue());
    }
    double diff = (double) (clock() - last_key_handle ) / CLOCKS_PER_SEC;
    if (diff >= kKeyProcess) {
      // Process keyboard state.
      debug(10, "Last key process %.5f sec. ago\n", diff);
      display->HandleKeys(key_status);
      last_key_handle = clock();
    }
    display->Display();
  }
  return 0;
}
