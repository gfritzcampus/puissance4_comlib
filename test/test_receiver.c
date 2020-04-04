#include "receiver.h"
#include "test.h"

#include <stdio.h>
#include <string.h>

#define MAX_CALLS 20

#define DECODER(name) \
static P4ReceiverContext * cmd_ ## name ## _context[MAX_CALLS] = { 0 };\
static size_t cmd_ ## name ## _nb_calls = 0;\
static P4ReturnCode cmd_ ## name ## _return[MAX_CALLS] = { 0 };\
\
static P4ReturnCode decode_ ## name (P4ReceiverContext * const context) {\
  cmd_ ## name ## _context[cmd_ ## name ## _nb_calls] = context;\
  ++cmd_ ## name ## _nb_calls;\
  return cmd_ ## name ## _return[cmd_ ## name ## _nb_calls - 1];\
}

DECODER(a);
DECODER(b);
DECODER(c);


#define INIT_DECODER(name) \
  memset(cmd_ ## name ## _context, 0, MAX_CALLS * sizeof (P4ReceiverContext *));\
  memset(cmd_ ## name ## _return, 0, MAX_CALLS * sizeof (P4ReturnCode));\
  cmd_ ## name ## _nb_calls = 0;

static void init_cmds() {
  INIT_DECODER(a);
  INIT_DECODER(b);
  INIT_DECODER(c);
}


int main(int argc, const char *argv[])
{
  P4ReturnCode ret;
  printf("*** receiver.c ***\n");

  #define SIZE_OF_BUFFER 10
  char buffer[SIZE_OF_BUFFER];

  P4SerialContext context = { 
    .buffer = INIT_BUFFER(buffer, SIZE_OF_BUFFER)
  };
  P4ReceiverContext rcontext = {
    .buffer = &context.buffer,
    .cookie = NULL
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

  ret = p4ReceiverInit(&rcontext);

  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should point to first cell");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  P4Command commands[] = {
     { 'a', 4, decode_a },
     { 'b', 2, decode_b },
     { 'c', 8, decode_c },
     { '\0', 0, NULL }
  };

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("Add correctly one char");
  init_cmds();
  context.buffer.head = SIZE_OF_BUFFER - 2;
  context.buffer.tail = SIZE_OF_BUFFER - 2;
  ret = p4Accumulate(&rcontext, commands, 'a');

  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == SIZE_OF_BUFFER - 1, "Head should point to new cell");
  assert(context.buffer.tail == SIZE_OF_BUFFER - 2, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("Add correctly one char when head pointer point last cell of buffer");
  ret = p4Accumulate(&rcontext, commands, 'b');
  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == SIZE_OF_BUFFER - 2, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("Fill completly buffer");
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, 'b');
  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == true, "Ring buffer should be mark as faulted");
  ret = p4Accumulate(&rcontext, commands, '\n');
  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("pop a byte");
  ret = p4Accumulate(&rcontext, commands, 'c');
  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 1, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");
  ret = p4Accumulate(&rcontext, commands, 'd');
  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 2, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");
  char pop = p4PopReceivedData(&rcontext);
  assert(pop == 'c', "Should return first inserted data");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 2, "Head should point to first cell");
  assert(context.buffer.tail == 1, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");
  pop = p4PopReceivedData(&rcontext);
  assert(pop == 'd', "Should return last inserted data");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 2, "Head should point to first cell");
  assert(context.buffer.tail == 2, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("Decode a function");
  init_cmds();
  cmd_b_return[0] = P4RC_DECODE_ERROR;
  ret = p4Accumulate(&rcontext, commands, 'b');
  ret = p4Accumulate(&rcontext, commands, '\n');
  assert(ret == P4RC_DECODE_ERROR, "Should return function result");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");
  assert(cmd_a_nb_calls == 0, "Should have not call a");
  assert(cmd_b_nb_calls == 1, "Should have call b");
  assert(cmd_c_nb_calls == 0, "Should have not call c");

  /////////////////////////////////////////////////////////////////////////////////////////////////
  setCurrentTest("Unknown function");
  init_cmds();
  ret = p4Accumulate(&rcontext, commands, 'd');
  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 1, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");
  ret = p4Accumulate(&rcontext, commands, '\n');
  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");
  assert(cmd_a_nb_calls == 0, "Should have not call a");
  assert(cmd_b_nb_calls == 0, "Should have not call b");
  assert(cmd_c_nb_calls == 0, "Should have not call c");



  return displayStats();
}

