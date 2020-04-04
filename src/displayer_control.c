#include "displayer_control.h"

/**
 * @brief Return character representing player
 *
 * @param player Player number to encode
 * @return '1' or '2' according player number, otherwise X
 */
inline static unsigned char encodePlayer(const P4Player player) {
  switch (player) {
    case P4P_PLAYER_1:
    case P4P_PLAYER_2:
      return (unsigned char) player;
    default:
      return 'X';
  }
}

/**
 * @brief Return character representing action
 *
 * @param action Action to encode
 * @return 'l', 'r', 'u', 'd' according action, otherwise X
 */
inline static unsigned char encodeAction(const P4Action action) {
  switch (action) {
    case P4A_LEFT:
    case P4A_RIGHT:
    case P4A_UP:
    case P4A_DOWN:
      return (unsigned char) action;
    default:
      return 'X';
  }
}

/**
 * @brief Return character representing action status
 *
 * @param status Status to encode
 * @return 'u' or 'd' according status, otherwise X
 */
inline static unsigned char encodeStatus(const P4ActionStatus status) {
  switch (status) {
    case P4AS_DOWN:
    case P4AS_UP:
      return (unsigned char) status;
    default:
      return 'X';
  }
}

P4ReturnCode p4PlayerPress(const P4SerialContext * const context, const P4Player player, const P4Action action, const P4ActionStatus status) {
  unsigned char buffer[P4_CMD_PLAYER_SIZE] = { 0 };

  buffer[0] = P4_CMD_PLAYER_CMD;
  buffer[1] = encodePlayer(player);
  buffer[2] = encodeAction(action);
  buffer[3] = encodeStatus(status);
  buffer[4] = P4_CMD_DELIMITER;

  size_t sent = context->send(buffer, P4_CMD_PLAYER_SIZE);

  return sent == P4_CMD_PLAYER_SIZE ? P4RC_OK : P4RC_SEND_ERROR;
}

P4ReturnCode p4LightSensor(const P4SerialContext * const context, const P4LightSensor lightSensorValue) {
  unsigned char buffer[P4_CMD_LIGHT_SENSOR_SIZE] = { 0 };

  buffer[0] = P4_CMD_LIGHT_SENSOR_CMD;
  encodeByte(&(buffer[1]), lightSensorValue);
  buffer[3] = P4_CMD_DELIMITER;

  size_t sent = context->send(buffer, P4_CMD_LIGHT_SENSOR_SIZE);

  return sent == P4_CMD_LIGHT_SENSOR_SIZE ? P4RC_OK : P4RC_SEND_ERROR;
}

