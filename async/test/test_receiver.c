#include "receiver.h"
#include "test.h"

#include <stdio.h>
#include <string.h>

#define MAX_CALLS 20

#define DECODER(name) \
static P4SerialContext * cmd_ ## name ## _context[MAX_CALLS] = { 0 };\
static size_t cmd_ ## name ## _nb_calls = 0;\
\
static void decode_ ## name (P4SerialContext * const context) {\
  cmd_ ## name ## _context[cmd_ ## name ## _nb_calls] = context;\
  ++cmd_ ## name ## _nb_calls;\
}

DECODER(a);
DECODER(b);
DECODER(c);


#define INIT_DECODER(name) \
  memset(cmd_ ## name ## _context, 0, MAX_CALLS);\
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

  setCurrentTest("Macro INIT_BUFFER");

  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should point to first cell");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  setCurrentTest("receiver init");

  context.buffer.head = 4;
  context.buffer.tail = 6;
  context.buffer.isFull = true;
  context.buffer.inError = true;

  ret = p4ReceiverInit(&context);

  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == 0, "Tail should point to first cell");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  P4Command commands[] = {
     { 'a', 4, decode_a },
     { 'b', 6, decode_b },
     { 'c', 8, decode_c },
  };

  setCurrentTest("Add correctly one char");
  init_cmds();
  context.buffer.head = SIZE_OF_BUFFER - 2;
  context.buffer.tail = SIZE_OF_BUFFER - 2;
  ret = p4Accumulate(&context, commands, 'a');

  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == SIZE_OF_BUFFER - 1, "Head should point to new cell");
  assert(context.buffer.tail == SIZE_OF_BUFFER - 2, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");

  setCurrentTest("Add correctly one char when head pointer point last cell of buffer");
  ret = p4Accumulate(&context, commands, 'b');
  assert(ret == P4RC_OK, "Should return OK");
  assert(context.buffer.data == buffer, "Ring buffer should point to given buffer");
  assert(context.buffer.size == SIZE_OF_BUFFER, "Ring buffer should have correct size");
  assert(context.buffer.head == 0, "Head should point to first cell");
  assert(context.buffer.tail == SIZE_OF_BUFFER - 2, "Tail should still same");
  assert(context.buffer.isFull == false, "Ring buffer should not be mark as full");
  assert(context.buffer.inError == false, "Ring buffer should not be mark as faulted");
}

