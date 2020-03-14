#include "board_control.h"

#include <stdbool.h>

#if P4_BOARD_NB_ROWS > 9
  #error P4Libs is designed for 9 rows maximum
#endif

#if P4_BOARD_NB_COLUMNS > 9
  #error P4Libs is designed for 9 columns maximum
#endif

#define CHECK_ZONE_VALIDITY {\
    if (!isValidMatrixZone(zone)) {\
      return P4RC_INVALID_PARAMETERS;\
    }\
  }

inline static unsigned char toOneCharAscii(const unsigned char i) {
  if (i < 9) {
    return '0' + i;
  }
  else if (i < 16) {
    return 'A' + (i - 10);
  }
  else {
    return 'X';
  }
}

inline static void encodeByte(unsigned char * const buffer, const unsigned char byte) {
  buffer[0] = toOneCharAscii((byte >> 4) & 0x0F); 
  buffer[1] = toOneCharAscii(byte & 0x0F);
}

inline static void encodeShort(unsigned char * const buffer, const unsigned short value) {
  buffer[0] = toOneCharAscii((value >> 12) & 0x0F); 
  buffer[1] = toOneCharAscii((value >> 8) & 0x0F); 
  buffer[2] = toOneCharAscii((value >> 4) & 0x0F); 
  buffer[3] = toOneCharAscii(value & 0x0F);
}

inline static void encodeZone(unsigned char * const buffer, const P4MatrixZone zone) {
  buffer[0] = toOneCharAscii(zone.startPoint.row);
  buffer[1] = toOneCharAscii(zone.startPoint.column);
  buffer[2] = toOneCharAscii(zone.endPoint.row);
  buffer[3] = toOneCharAscii(zone.endPoint.column);
}

inline static void encodeColor(unsigned char * const buffer, const P4Color color) {
  encodeByte(&(buffer[0]), color.red);
  encodeByte(&(buffer[2]), color.green);
  encodeByte(&(buffer[4]), color.blue);
}

P4ReturnCode p4SetZoneColor(const P4SerialContext context, const P4MatrixZone zone, const P4Color color) {
  unsigned char buffer[P4_CMD_ZONE_COLOR_SIZE] = { 0 };

  CHECK_ZONE_VALIDITY;

  buffer[0] = P4_CMD_ZONE_COLOR_CMD;
  encodeZone(&(buffer[1]), zone);
  encodeColor(&(buffer[5]), color);
  buffer[11] = P4_CMD_DELIMITER;

  size_t sent = context.send(buffer, P4_CMD_ZONE_COLOR_SIZE);

  return sent == P4_CMD_ZONE_COLOR_SIZE ? P4RC_OK : P4RC_SEND_ERROR;
}

P4ReturnCode p4SetZoneOn(const P4SerialContext context, const P4MatrixZone zone) {
  unsigned char buffer[P4_CMD_ZONE_ON_SIZE] = { 0 };

  CHECK_ZONE_VALIDITY;

  buffer[0] = P4_CMD_ZONE_ON_CMD;
  encodeZone(&(buffer[1]), zone);
  buffer[5] = P4_CMD_DELIMITER;

  size_t sent = context.send(buffer, P4_CMD_ZONE_ON_SIZE);

  return sent == P4_CMD_ZONE_ON_SIZE ? P4RC_OK : P4RC_SEND_ERROR;
}

P4ReturnCode p4SetZoneOff(const P4SerialContext context, const P4MatrixZone zone) {
  unsigned char buffer[P4_CMD_ZONE_OFF_SIZE] = { 0 };

  CHECK_ZONE_VALIDITY;

  buffer[0] = P4_CMD_ZONE_OFF_CMD;
  encodeZone(&(buffer[1]), zone);
  buffer[5] = P4_CMD_DELIMITER;

  size_t sent = context.send(buffer, P4_CMD_ZONE_OFF_SIZE);

  return sent == P4_CMD_ZONE_OFF_SIZE ? P4RC_OK : P4RC_SEND_ERROR;
}

P4ReturnCode p4SetZoneIntensity(const P4SerialContext context, const P4MatrixZone zone, const unsigned char intensity) {
  unsigned char buffer[P4_CMD_ZONE_INTENSITY_SIZE] = { 0 };

  CHECK_ZONE_VALIDITY;

  buffer[0] = P4_CMD_ZONE_INTENSITY_CMD;
  encodeZone(&(buffer[1]), zone);
  encodeByte(&(buffer[5]), intensity);
  buffer[7] = P4_CMD_DELIMITER;

  size_t sent = context.send(buffer, P4_CMD_ZONE_INTENSITY_SIZE);

  return sent == P4_CMD_ZONE_INTENSITY_SIZE ? P4RC_OK : P4RC_SEND_ERROR;
}

P4ReturnCode p4SetZoneBlink(const P4SerialContext context, const P4MatrixZone zone, const unsigned short onTime, const unsigned short offTime) {
  unsigned char buffer[P4_CMD_ZONE_BLINK_SIZE] = { 0 };

  CHECK_ZONE_VALIDITY;

  buffer[0] = P4_CMD_ZONE_BLINK_CMD;
  encodeZone(&(buffer[1]), zone);
  encodeShort(&(buffer[5]), onTime);
  encodeShort(&(buffer[9]), offTime);
  buffer[13] = P4_CMD_DELIMITER;

  size_t sent = context.send(buffer, P4_CMD_ZONE_BLINK_SIZE);

  return sent == P4_CMD_ZONE_BLINK_SIZE ? P4RC_OK : P4RC_SEND_ERROR;
}
