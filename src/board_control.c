#include "board_control.h"

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

#define CHECK_POINT_VALIDITY(point) {\
    if (!isValidMatrixPoint(point)) {\
      return P4RC_INVALID_PARAMETERS;\
    }\
  }

/**
 * @brief encode short in its hexadecimal representation
 *
 * @param buffer Buffer to store result, should be at least size 4
 * @param value Short to encode
 */
inline static void encodeShort(unsigned char * const buffer, const unsigned short value) {
  buffer[0] = toHexaHalfByte((value >> 12) & 0x0F); 
  buffer[1] = toHexaHalfByte((value >> 8) & 0x0F); 
  buffer[2] = toHexaHalfByte((value >> 4) & 0x0F); 
  buffer[3] = toHexaHalfByte(value & 0x0F);
}

/**
 * @brief encode matrix point
 *
 * @param buffer Buffer to store result, should be at least size 2
 * @param point Coordonates of point to encode
 */
inline static void encodePoint(unsigned char * buffer, const P4MatrixPoint point) {
  buffer[0] = toHexaHalfByte(point.row);
  buffer[1] = toHexaHalfByte(point.column);
}

/**
 * @brief encode matrix zone
 *
 * @param buffer Buffer to store result, should be at least size 4
 * @param zone Coordonates of zone to encode
 */
inline static void encodeZone(unsigned char * const buffer, const P4MatrixZone zone) {
  encodePoint(&(buffer[0]), zone.startPoint);
  encodePoint(&(buffer[2]), zone.endPoint);
}

/**
 * @brief encode color in its hexacimal representation
 *
 * @param buffer Buffer to store result, should be at least size 6
 * @param color Colot to encode in its hexadecimal representation
 */
inline static void encodeColor(unsigned char * const buffer, const P4Color color) {
  encodeByte(&(buffer[0]), color.red);
  encodeByte(&(buffer[2]), color.green);
  encodeByte(&(buffer[4]), color.blue);
}

P4ReturnCode p4SetZoneIntensity(const P4SerialContext * const context, const P4MatrixZone zone, const P4Intensity intensity) {
  unsigned char buffer[P4_CMD_ZONE_INTENSITY_SIZE] = { 0 };

  CHECK_ZONE_VALIDITY;

  buffer[0] = P4_CMD_ZONE_INTENSITY_CMD;
  encodeZone(&(buffer[1]), zone);
  encodeByte(&(buffer[5]), intensity);
  buffer[7] = P4_CMD_DELIMITER;

  size_t sent = context->send(buffer, P4_CMD_ZONE_INTENSITY_SIZE);

  return sent == P4_CMD_ZONE_INTENSITY_SIZE ? P4RC_OK : P4RC_SEND_ERROR;
}

P4ReturnCode p4SetRingColor(const P4SerialContext * const context, const P4MatrixPoint ring, const P4Color color) {
  unsigned char buffer[P4_CMD_SHORT_RING_SIZE] = { 0 };

  CHECK_POINT_VALIDITY(ring);

  buffer[0] = P4_CMD_SHORT_RING_CMD;
  encodePoint(&(buffer[1]), ring);
  encodeColor(&(buffer[3]), color);
  buffer[P4_CMD_SHORT_RING_SIZE- 1] = P4_CMD_DELIMITER;

  size_t sent = context->send(buffer, P4_CMD_SHORT_RING_SIZE);

  return sent == P4_CMD_SHORT_RING_SIZE ? P4RC_OK : P4RC_SEND_ERROR;
}

