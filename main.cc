#include <cstdio>
#include <getopt.h>
#include <map>

#include "color.h"
#include "debug.h"
#include "distributor.h"
#include "event.h"
#include "graphics_engine.h"
#include "random_distributor.h"
#include "random_walk_distributor.h"
#include "sdl_engine.h"

int main(int argc, char* argv[]) {
  int window_height = -1;
  int window_width = -1;
  bool walk_model = false;
  int option_char;
  // Parse command-line arguments.
  while ((option_char = getopt(argc, argv, "w:h:m:")) != EOF) {
    switch (option_char) {
      case 'w': window_width = atoi(optarg); break;
      case 'h': window_height = atoi(optarg); break;
      case 'm':
        if ((std::string) optarg == "walk") {
          walk_model = true;
        }
        break;
      default:
        break;
    }
  }
  GraphicsEngine *display = new SDLEngine();
  display->init(window_height, window_width);
  display->drawRectangle(0, 0, kMapWidth, kMapHeight, Color(255, 255, 255));
  //display->DrawRectangle(2048, 2048, 50, 50, Color(255, 0, 0));
  Distributor *distributor;
  if (walk_model) {
    debug(10, "Using random walk model.\n", "");
    distributor = new RandomWalkDistributor();
  } else {
    debug(10, "Using random model.\n", "");
    distributor = new RandomDistributor();
  }
  Color c;
  Point p;
  distributor->init(&c, &p);
  display->drawPoint(p.getX(), p.getY(), c);
  debug(1, "Initial color (%d,%d,%d) at point (%d,%d)\n",
      c.getR(), c.getG(), c.getB(), p.getX(), p.getY());
  Event event;
  // Stores the status of keyboard keys.
  std::map<int, int> key_status;
  // Handle key press status at a constant interval.
  clock_t lastKeyHandle = clock();
  int counter = 1;
  while (event.getValue() != kEscapeCode) {
    if (!distributor->done()) {
      distributor->query(&c, &p);
      display->drawPoint(p.getX(), p.getY(), c);
      debug(1, "Color (%d,%d,%d) at point (%d,%d)\n",
          c.getR(), c.getG(), c.getB(), p.getX(), p.getY());
      counter ++;
    }
    // Query for events, and update keyboard status.
    event = display->eventPoll();
    key_status[event.getValue()] = event.getType();
    if (event.getType() != kNoEvent) {
      debug(4, "Event read: %d %d\n", event.getType(), event.getValue());
    }
    if ((double) (clock() - lastKeyHandle) / CLOCKS_PER_SEC >= kKeyProcess) {
      // Process keyboard state.
      debug(10, "Last key process %.5f sec. ago\n", diff);
      display->handleKeys(key_status);
      lastKeyHandle = clock();
    }
    display->display();
  }
  return 0;
}
