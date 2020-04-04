#include "board_receiver.h"
#include "test.h"

#include "../src/receiver.c"

#include <stdio.h>
#include <string.h>

int main(int argc, const char *argv[])
{
  P4ReturnCode ret;
  P4BoardCommand command;
  printf("*** board_receiver.c ***\n");

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

  ret = p4ReceiverInitFromCore(&context);

  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should point to first cell");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("Receive ring color command");

  command = p4AccumulateFromCore(&context, 'R');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '1');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '2');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '6');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '4');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, 'F');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, 'f');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '0');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '6');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '\n');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_RingColor, "Should return ring color command");
  assert(command.ringColor.point.row == 1, "Should get row");
  assert(command.ringColor.point.column == 2, "Should get column");
  assert(command.ringColor.color.red = 100, "Should get red color");
  assert(command.ringColor.color.green = 255, "Should get green color");
  assert(command.ringColor.color.blue = 6, "Should get blue color");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("Receive intensity zone command");

  command = p4AccumulateFromCore(&context, 'i');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '1');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '2');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '3');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '4');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, 'a');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, 'f');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_None, "Should return None command");
  command = p4AccumulateFromCore(&context, '\n');
  assert(command.returnCode == P4RC_OK, "Should return OK");
  assert(command.type == P4BCT_ZoneIntensity, "Should return light sensor command");
  assert(command.zoneIntensity.zone.startPoint.row == 1, "Should get top left row");
  assert(command.zoneIntensity.zone.startPoint.column == 2, "Should get top left column");
  assert(command.zoneIntensity.zone.endPoint.row == 3, "Should get bottom right row");
  assert(command.zoneIntensity.zone.endPoint.column == 4, "Should get bottom right column");
  assert(command.zoneIntensity.intensity== 175, "Should get light sensor value");

  return displayStats();
}

