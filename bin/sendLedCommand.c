#include <stdlib.h>
#include <stdio.h>

#include "leds_control.h"

int main(int argc, const char *argv[])
{
  if (argc < 6) 
    return -1;

  setLedColor(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));

  return 0;
}
