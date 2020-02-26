#include "leds_control.h"

#include <stdio.h>
#include <string.h>

#define MAX_LENGTH_PATHNAME 255

static int stub_open_return = 0;
static char stub_open_pathname[MAX_LENGTH_PATHNAME] = {0};
static int stub_open_flags = 0;
static unsigned int stub_open_nb_calls = 0;
int stub_open(const char *pathname, int flags) {
  ++stub_open_nb_calls;

  strncpy(stub_open_pathname, pathname, MAX_LENGTH_PATHNAME);
  stub_open_flags = flags;

  return stub_open_return;
}

static int stub_close_return = 0;
static int stub_close_fd = 0;
int stub_close(int fd) {
  stub_close_fd = fd;
  return stub_close_return;
}

#define MAX_SIZE_BUFFER 255

static ssize_t stub_write_return = 0;
static int stub_write_fd = 0;
static char stub_write_buffer[MAX_SIZE_BUFFER] = {0};
static size_t stub_write_size = 0;
ssize_t stub_write(int fd, const char * const buffer, size_t size) {
  stub_write_fd = fd;
  stub_write_size = size;
  memcpy(stub_write_buffer, buffer, (size > MAX_SIZE_BUFFER ? MAX_SIZE_BUFFER : size));

  return stub_write_return;
}

#define VA_ARGS(...) , ##__VA_ARGS__
#define assert(cond, msg, ...) {\
  if (!(cond)) {\
    printf(" ---> ASSERT FAIL at %d : "\
    msg\
    "\n", __LINE__ VA_ARGS(__VA_ARGS__));\
  }\
}

int main(int argc, const char *argv[])
{
  LedControlReturnCode ret;
  printf("*** test_leds_control ***\n");

  printf(" 1/ Error on opening serial link\n");
  stub_open_return = -1;
  ret = setLedColor(1, 1, 0, 0, 0);
  assert(ret == LCRC_ERROR_SERIAL_OPEN, "Return should be an error");

  printf(" 2/ Error on row\n");
  ret = setLedColor(0, 1, 0, 0, 0);
  assert(ret == LCRC_ERROR_ROW, "Return should be an error");
  ret = setLedColor(0, 0, 0, 0, 0);
  assert(ret == LCRC_ERROR_ROW, "Return should be an error");

  printf(" 3/ Error on col\n");
  ret = setLedColor(1, 0, 0, 0, 0);
  assert(ret == LCRC_ERROR_COL, "Return should be an error");

  printf(" 4/ Error on write\n");
}
