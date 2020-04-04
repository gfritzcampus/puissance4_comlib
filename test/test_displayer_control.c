#include "displayer_control.h"
#include "test.h"

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
  printf("*** payers_control.c ***\n");

  P4SerialContext context = {
    .send = stub_send,
  };
  /////////////////////////////////////////////////////////////////
  setCurrentTest("light sensor value");
  stub_send_init();
  for(size_t i = 0; i < 3; ++i) {
    stub_send_return[i] = P4_CMD_LIGHT_SENSOR_SIZE;
  }
  ret = p4LightSensor(&context, 0);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4LightSensor(&context, 100);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4LightSensor(&context, 255);
  assert(ret == P4RC_OK, "Should return OK");

  assert(stub_send_nb_calls == 3, "Should call only one time send");
  assert(stub_send_size[0] == P4_CMD_LIGHT_SENSOR_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[1] == P4_CMD_LIGHT_SENSOR_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[2] == P4_CMD_LIGHT_SENSOR_SIZE, "Should send correct number of bytes");
  assert(memcmp(stub_send_buffer[0], "l00\n", P4_CMD_LIGHT_SENSOR_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[1], "l64\n", P4_CMD_LIGHT_SENSOR_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[2], "lFF\n", P4_CMD_LIGHT_SENSOR_SIZE) == 0, "Should send correct command");

  /////////////////////////////////////////////////////////////////
  setCurrentTest("player press");
  stub_send_init();
  for(size_t i = 0; i < 16; ++i) {
    stub_send_return[i] = P4_CMD_PLAYER_SIZE;
  }
  ret = p4PlayerPress(&context, P4P_PLAYER_1, P4A_UP, P4A_UP);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_1, P4A_UP, P4A_DOWN);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_1, P4A_DOWN, P4A_UP);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_1, P4A_DOWN, P4A_DOWN);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_1, P4A_LEFT, P4A_UP);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_1, P4A_LEFT, P4A_DOWN);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_1, P4A_RIGHT, P4A_UP);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_1, P4A_RIGHT, P4A_DOWN);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_2, P4A_UP, P4A_UP);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_2, P4A_UP, P4A_DOWN);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_2, P4A_DOWN, P4A_UP);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_2, P4A_DOWN, P4A_DOWN);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_2, P4A_LEFT, P4A_UP);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_2, P4A_LEFT, P4A_DOWN);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_2, P4A_RIGHT, P4A_UP);
  assert(ret == P4RC_OK, "Should return OK");
  ret = p4PlayerPress(&context, P4P_PLAYER_2, P4A_RIGHT, P4A_DOWN);
  assert(ret == P4RC_OK, "Should return OK");

  assert(stub_send_nb_calls == 16, "Should call only one time send");
  assert(stub_send_size[0 ] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[1 ] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[2 ] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[3 ] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[4 ] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[5 ] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[6 ] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[7 ] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[8 ] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[9 ] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[10] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[11] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[12] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[13] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[14] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(stub_send_size[15] == P4_CMD_PLAYER_SIZE, "Should send correct number of bytes");
  assert(memcmp(stub_send_buffer[0 ], "p1uu\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[1 ], "p1ud\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[2 ], "p1du\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[3 ], "p1dd\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[4 ], "p1lu\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[5 ], "p1ld\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[6 ], "p1ru\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[7 ], "p1rd\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[8 ], "p2uu\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[9 ], "p2ud\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[10], "p2du\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[11], "p2dd\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[12], "p2lu\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[13], "p2ld\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[14], "p2ru\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");
  assert(memcmp(stub_send_buffer[15], "p2rd\n", P4_CMD_PLAYER_SIZE) == 0, "Should send correct command");

  displayStats();
}

