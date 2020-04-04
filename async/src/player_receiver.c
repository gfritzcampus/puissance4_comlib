#include "player_receiver.h"

#include "receiver.h"

/**
 * @brief Check if value is a player data, and convert it
 *        to P4Player
 *
 * @param value Value to check and convert
 * @return a P4Player value if conversion is possible, otherwise P4_UNKNOWN
 */
inline static P4Player decodePlayer(const char value) {
  switch(value) {
    case P4P_PLAYER_1:
    case P4P_PLAYER_2:
      return (P4Player) value;
    default:
      return P4_UNKNOWN;
  }
}

/**
 * @brief Check if value is a action data and convert it
 *        to P4Action
 *
 * @param value Value to check and convert
 * @return a P4Action value if conversion is possible, otherwise P4_UNKNOWN
 */
inline static P4Action decodeAction(const char value) {
  switch(value) {
    case P4A_LEFT:
    case P4A_RIGHT:
    case P4A_UP:
    case P4A_DOWN:
      return (P4Action) value;
    default:
      return P4_UNKNOWN;
  }
}

/**
 * @brief Check if value is a action status data and convert
 *        it to P4ActionStatus
 *
 * @param valume Value to check and convert
 * @return a P4ActionStatus value if conversion is possible, otherwise P4_UNKNOWN
 */
inline static P4ActionStatus decodeStatus(const char value) {
  switch(value) {
    case P4AS_DOWN:
    case P4AS_UP:
      return (P4ActionStatus) value;
    default:
      return P4_UNKNOWN;
  }
}

/**
 * @brief Decode a player command, and call specific callback if
 *        command is correct
 *
 * @param context Serial context used to recevied command
 */
static void decodePlayerCommand(P4SerialContext * const context) {
  P4Player player = decodePlayer(p4PopReceivedData(context));
  P4Action action = decodeAction(p4PopReceivedData(context));
  P4ActionStatus status = decodeStatus(p4PopReceivedData(context));
  
  if (player != P4_UNKNOWN && action != P4_UNKNOWN && status != P4_UNKNOWN) {
    context->playerActionCallback(context->cookie, player, action, status);
  }
}

/**
 * @brief List of commands available for core
 */
static const P4Command commands[] = {
  { P4_CMD_PLAYER_CMD, P4_CMD_PLAYER_SIZE, decodePlayerCommand }, 
  { '\0', 0, NULL } // Terminating empty command
};

P4ReturnCode p4ReceiverInitFromDisplayer(P4SerialContext * const context) {
  return p4ReceiverInit(context);
}

P4ReturnCode p4AccumulateFromDisplayer(P4SerialContext * const context, const char data) {
  return p4Accumulate(context, commands, data);
}
