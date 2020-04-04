#include "board_control.h"
#include "test.h"

#include <stdio.h>
#include <string.h>


#define MAX_CALLS 20
#define MAX_SIZE 256

char stub_send_buffer[MAX_CALLS][MAX_SIZE] = { 0 };
size_t stub_send_size[MAX_CALLS] = { 0 };
size_t stub_send_return[MAX_CALLS] = { 0 };
size_t stub_send_nb_calls = 0;

static void stub_send_init() {
  memset(stub_send_buffer, 0, MAX_SIZE * MAX_CALLS);
  memset(stub_send_size, 0, MAX_CALLS);
  memset(stub_send_return, 0, MAX_CALLS);
  stub_send_nb_calls = 0;
}

static size_t stub_send(const void * const buffer, size_t size) {
  stub_send_size[stub_send_nb_calls] = size;
  memcpy(stub_send_buffer[stub_send_nb_calls], buffer, size);
  ++stub_send_nb_calls;

  return stub_send_return[stub_send_nb_calls - 1];
}

int main(int argc, const char *argv[])
{
  P4ReturnCode ret;
  printf("*** board_control.c ***\n");

  P4SerialContext context = {
    .send = stub_send,
  };

  P4MatrixZone zone = { 
      .startPoint = {
        .row = 1,
        .column = 2
      },
      .endPoint = {
        .row = 3,
        .column = 4
      }
    };
  P4Color color = { 
      .red = 0x12,
      .green = 0x34,
      .blue = 0x56
    };

  /////////////////////////////////////////////////////////////////
  setCurrentTest("zone color");
  stub_send_init();
  stub_send_return[0] = P4_CMD_ZONE_COLOR_SIZE;
  ret = p4SetZoneColor(&context, zone, color);

  assert(ret == P4RC_OK, "Should return OK");
  assert(stub_send_nb_calls == 1, "Should call only one time send");
  assert(stub_send_size[0] == P4_CMD_ZONE_COLOR_SIZE, "Should send correct number of bytes");
  assert(memcmp(stub_send_buffer[0], "c1234123456\n", P4_CMD_ZONE_COLOR_SIZE) == 0, "Should send correct command");

  /////////////////////////////////////////////////////////////////
  setCurrentTest("zone on");
  stub_send_init();
  stub_send_return[0] = P4_CMD_ZONE_ON_SIZE;
  ret = p4SetZoneOn(&context, zone);

  assert(ret == P4RC_OK, "Should return OK");
  assert(stub_send_nb_calls == 1, "Should call only one time send");
  assert(stub_send_size[0] == P4_CMD_ZONE_ON_SIZE, "Should send correct number of bytes");
  assert(memcmp(stub_send_buffer[0], "O1234\n", P4_CMD_ZONE_ON_SIZE) == 0, "Should send correct command");
  
  /////////////////////////////////////////////////////////////////
  setCurrentTest("zone off");
  stub_send_init();
  stub_send_return[0] = P4_CMD_ZONE_OFF_SIZE;
  ret = p4SetZoneOff(&context, zone);

  assert(ret == P4RC_OK, "Should return OK");
  assert(stub_send_nb_calls == 1, "Should call only one time send");
  assert(stub_send_size[0] == P4_CMD_ZONE_OFF_SIZE, "Should send correct number of bytes");
  assert(memcmp(stub_send_buffer[0], "o1234\n", P4_CMD_ZONE_OFF_SIZE) == 0, "Should send correct command");
  
  /////////////////////////////////////////////////////////////////
  setCurrentTest("zone intensity");
  stub_send_init();
  stub_send_return[0] = P4_CMD_ZONE_INTENSITY_SIZE;
  ret = p4SetZoneIntensity(&context, zone, 0x4B);

  assert(ret == P4RC_OK, "Should return OK");
  assert(stub_send_nb_calls == 1, "Should call only one time send");
  assert(stub_send_size[0] == P4_CMD_ZONE_INTENSITY_SIZE, "Should send correct number of bytes");
  assert(memcmp(stub_send_buffer[0], "i12344B\n", P4_CMD_ZONE_INTENSITY_SIZE) == 0, "Should send correct command");
  
  /////////////////////////////////////////////////////////////////
  setCurrentTest("zone blink");
  stub_send_init();
  stub_send_return[0] = P4_CMD_ZONE_BLINK_SIZE;
  ret = p4SetZoneBlink(&context, zone, 0x1234, 0x5678);

  assert(ret == P4RC_OK, "Should return OK");
  assert(stub_send_nb_calls == 1, "Should call only one time send");
  assert(stub_send_size[0] == P4_CMD_ZONE_BLINK_SIZE, "Should send correct number of bytes");
  assert(memcmp(stub_send_buffer[0], "b123412345678\n", P4_CMD_ZONE_BLINK_SIZE) == 0, "Should send correct command");

  ////////////////////////////////////////////////////////////////
  setCurrentTest("ring color");
  stub_send_init();
  stub_send_return[0] = P4_CMD_SHORT_RING_SIZE;
  ret = p4SetRingColor(&context, zone.startPoint, color);

  assert(ret == P4RC_OK, "Should return OK");
  assert(stub_send_nb_calls == 1, "Should call only one time send");
  assert(stub_send_size[0] == P4_CMD_SHORT_RING_SIZE, "Should send correct number of bytes");
  assert(memcmp(stub_send_buffer[0], "R12123456\n", P4_CMD_SHORT_RING_SIZE) == 0, "Should send correct command");

}
