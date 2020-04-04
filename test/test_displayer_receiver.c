#include "displayer_receiver.h"
#include "test.h"

#include "../src/receiver.c"

#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
  P4ReturnCode ret;
  P4DisplayerCommand command;
  printf("*** displayer_receiver.c ***\n");

  #define SIZE_OF_BUFFER 256
  char buffer[SIZE_OF_BUFFER];

  P4SerialContext context = { 
    .buffer = INIT_BUFFER(buffer, SIZE_OF_BUFFER)
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("Macro INIT_BUFFER");

  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should point to first cell");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("receiver init");

  context.buffer.head = 4;
  context.buffer.tail = 6;
  context.buffer.isFull = true;
  context.buffer.inError = true;

  ret = p4ReceiverInitFromDisplayer(&context);

  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should point to first cell");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("Receive player command");

  command = p4AccumulateFromDisplayer(&context, 'p');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4DCT_None, "Should return None command");
  command = p4AccumulateFromDisplayer(&context, '1');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4DCT_None, "Should return None command");
  command = p4AccumulateFromDisplayer(&context, 'l');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4DCT_None, "Should return None command");
  command = p4AccumulateFromDisplayer(&context, 'u');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4DCT_None, "Should return None command");
  command = p4AccumulateFromDisplayer(&context, '\n');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4DCT_PlayerCommand, "Should return player command");
  assert(command.player.player == P4P_PLAYER_1, "Should decode player one");
  assert(command.player.action == P4A_LEFT, "Should decode left action");
  assert(command.player.status == P4AS_UP, "Should decode up action status");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("Receive light sensor command");

  command = p4AccumulateFromDisplayer(&context, 'l');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4DCT_None, "Should return None command");
  command = p4AccumulateFromDisplayer(&context, '6');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4DCT_None, "Should return None command");
  command = p4AccumulateFromDisplayer(&context, '4');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4DCT_None, "Should return None command");
  command = p4AccumulateFromDisplayer(&context, '\n');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4DCT_LightSensor, "Should return light sensor command");
  assert(command.lightsensor.value == 100, "Should decode an intensity of 100");

  return displayStats();
}

