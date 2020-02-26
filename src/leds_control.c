#include "leds_control.h"

#include <fcntl.h>
#include <unistd.h>

#define NUMBER_OF_LEDS_PER_RING 24
#define MAX_ROW 7
#define MAX_COL 7

#define SIZE_OF_BUFFER (4 + NUMBER_OF_LEDS_PER_RING * 3)

#ifndef LC_OPEN
#define LC_OPEN open
#endif

#ifndef LC_WRITE
#define LC_WRITE write
#endif

#ifndef LC_CLOSE
#define LC_CLOSE close
#endif

static void computeMessage(unsigned char * const buffer,
                           const unsigned int row, 
                           const unsigned int col, 
                           const unsigned int red, 
                           const unsigned int green, 
                           const unsigned int blue) {
  buffer[0] = 'r';
  buffer[1] = '0' + row;
  buffer[2] = '0' + col;

  for(size_t i = 0; i < NUMBER_OF_LEDS_PER_RING; ++i) {
    buffer[3+(i*3)] = red;
    buffer[4+(i*3)] = green;
    buffer[5+(i*3)] = blue;
  }

  buffer[SIZE_OF_BUFFER - 1] = '\n';
}

LedControlReturnCode setLedColor(const unsigned int row, 
                                 const unsigned int col, 
                                 const unsigned int red, 
                                 const unsigned int green, 
                                 const unsigned int blue) {
  if (row > MAX_ROW || row == 0)
    return LCRC_ERROR_ROW;

  if (col > MAX_COL || col == 0)
    return LCRC_ERROR_COL;

  if (red > 255 || green > 255 || blue > 255)
    return LCRC_ERROR_RGB_VALUE;

  const unsigned int finalRow = row - 1;
  const unsigned int finalCol = col - 1;

  const int fd = LC_OPEN("/tmp/puissance4/serial/ttyS1", O_WRONLY | O_NDELAY);

  if (fd == -1)
    return LCRC_ERROR_SERIAL_OPEN;

  char buffer[SIZE_OF_BUFFER] = { 0 };
  computeMessage(buffer, finalRow, finalCol, red, green, blue);

  const ssize_t nbOfWrittenBytes = LC_WRITE(fd, buffer, SIZE_OF_BUFFER);

  if (nbOfWrittenBytes != SIZE_OF_BUFFER) {
    LC_CLOSE(fd);
    return LCRC_ERROR_SERIAL_WRITE;
  }

  if (LC_CLOSE(fd) != 0)
    return LCRC_ERROR_SERIAL_CLOSE;


  return LCRC_OK;
}

