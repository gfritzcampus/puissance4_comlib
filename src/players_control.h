#include "players_control.h"

P4ReturnCode p4PlayerPress(const P4SerialContext context, const P4Player player, const P4Action action, const P4ActionStatus status) {
  unsigned char buffer[P4_CMD_PLAYER_SIZE] = { 0 };

  CHECK_ZONE_VALIDITY;

  buffer[0] = P4_CMD_PLAYER_CMD;
  buffer[1] = encodePlayer(player);
  buffer[2] = encodeAction(action);
  buffer[3] = encodeStatus(status);
  buffer[4] = P4_CMD_DELIMITER;

  size_t sent = context.send(buffer, P4_CMD_PLAYER_SIZE);

  return sent == P4_CMD_PLAYER_SIZE ? P4RC_OK : P4RC_SEND_ERROR;
}
