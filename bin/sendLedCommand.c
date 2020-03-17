#include <stdlib.h>
#include <stdio.h>

#include "leds_control.h"

int main(int argc, const char *argv[])
{
  if (argc < 4) 
    return -1;

  for(int i = 1; i <= 7; ++i) {
    for(int j = 1; j <= 7; ++j) {
      setLedColor(i, j, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    }
  }

  return 0;
}
