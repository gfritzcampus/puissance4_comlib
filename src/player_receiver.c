#include "board_receiver.h"

#include "receiver.h"

static const P4Command commands[] = {
  { '\0', 0, NULL } // Terminating command
};

P4ReturnCode p4ReceiverInitFromDisplayer(P4SerialContext * const context) {
  return p4ReceiverInit(context);
}

P4ReturnCode p4AccumulateFromDisplayer(P4SerialContext * const context, const char data) {
  return p4Accumulate(context, commands, data);
}
