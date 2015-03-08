#include <unistd.h>

#include "color.h"
#include "graphicsengine.h"
#include "sdlengine.h"

int main(int argc, char* argv[]) {
  char tmp;
  while ((tmp=getopt(argc,argv,"")) != -1) {
    switch (tmp) {
      default:
        break;
    }
  }
  return 0;
}
