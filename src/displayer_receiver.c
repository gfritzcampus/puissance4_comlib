#include "displayer_receiver.h"

#include "receiver.h"
#include "decode.h"

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
 * @brief Decode a player command
 *
 * @param context context used to received command
 * @return P4RC_OK if command can be decoded, otherwise P4RC_DECODE_ERROR
 */
static P4ReturnCode decodePlayerCommand(P4ReceiverContext * const context) {
  P4DisplayerCommand * const command = (P4DisplayerCommand *) context->cookie;
  command->type = P4DCT_PlayerCommand;
  command->player.player = decodePlayer(p4PopReceivedData(context));
  command->player.action = decodeAction(p4PopReceivedData(context));
  command->player.status =  decodeStatus(p4PopReceivedData(context));

  return command->player.player == P4_UNKNOWN ||
         command->player.action == P4_UNKNOWN ||
         command->player.status == P4_UNKNOWN ? P4RC_DECODE_ERROR : P4RC_OK;
}

/**
 * @brief Decode a light sensor command
 *
 * @param context used to received command
 * @return P4RC_OK if command can be decoded, otherwise P4RC_DECODE_ERROR
 */
static P4ReturnCode decodeLightSensorCommand(P4ReceiverContext * const context) {
  P4DisplayerCommand * const command = (P4DisplayerCommand *) context->cookie;
  command->type = P4DCT_LightSensor;

  char mshb = p4PopReceivedData(context);
  char lshb = p4PopReceivedData(context); 
  return decodeByte(mshb, lshb, &command->lightsensor.value) ? P4RC_OK : P4RC_DECODE_ERROR;
}

/**
 * @brief List of commands available for core
 */
static const P4Command commands[] = {
  { P4_CMD_PLAYER_CMD, P4_CMD_PLAYER_SIZE, decodePlayerCommand }, 
  { P4_CMD_LIGHT_SENSOR_CMD, P4_CMD_LIGHT_SENSOR_SIZE, decodeLightSensorCommand },
  { '\0', 0, NULL } // Terminating empty command
};

P4ReturnCode p4ReceiverInitFromDisplayer(P4SerialContext * const context) {
  P4ReceiverContext rcontext = {
    .buffer = &context->buffer,
    .cookie = NULL
  };
  return p4ReceiverInit(&rcontext);
}

P4DisplayerCommand p4AccumulateFromDisplayer(P4SerialContext * const context, const char data) { 
  P4DisplayerCommand result = {  
      .returnCode = P4RC_OK,
      .type = P4DCT_None
    };

  P4ReceiverContext rcontext = {
      .buffer = &context->buffer,
      .cookie = &result
    };

  result.returnCode = p4Accumulate(&rcontext, commands, data);

  return result;
}
