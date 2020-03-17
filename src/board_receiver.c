#include "board_receiver.h"

#include "receiver.h"

/**
 * @brief Convert hexadecimal representation of half byte
 *
 * @param hb Half byte representing in hexa
 * @return 0 - 15 if conversion is possible, otherwise -1
 */
static unsigned char decodeHalfByte(const char hb) {
  if ('0' <= hb && hb <= '9') {
    return hb - '0';
  }
  else if ('A' <= hb && hb <= 'F') {
    return 10 + hb - 'A';
  }
  else if ('a' <= hb && hb <= 'f') {
    return 10 + hb - 'a';
  }
  else {
    return -1;
  }
}

/**
 * @brief Decode a row number and check if correct
 *
 * @param row Value to decode
 * @return Row number if is a valid row number, otherwise -1
 */
static unsigned char decodeRow(const char row) {
  unsigned char decoded = decodeHalfByte(row);
  if (0 <= row && row < P4_BOARD_NB_ROWS) {
    return decoded;
  }
  return -1;
}

/**
 * @brief Decode a column number and check if correct
 *
 * @param column Value to decode
 * @return Column number if is a valid column number, otherwise -1
 */
static unsigned char decodeColumn(const char column) {
  unsigned char decoded = decodeHalfByte(column);
  if (0 <= column && column < P4_BOARD_NB_COLUMNS) {
    return decoded;
  }
  return -1;
}

/**
 * @brief Decode a byte
 * 
 * @param mshb most significant half byte
 * @param lshb most significant halg byte
 * @param result Pointer to where to store value
 * @return true if conversion if OK, otherwise false
 */
static bool decodeByte(const char mshb, const char lshb, unsigned char * const result) {
  unsigned char decoded_mshb = decodeHalfByte(mshb);
  unsigned char decoded_lshb = decodeHalfByte(lshb);

  if (decoded_mshb == -1 || decoded_lshb == -1) {
    return false;
  }

  *result = (mshb << 4) + lshb;

  return true;
}

/**
 * @brief Decode a basic color
 *
 * @param context Serial context containing received data
 * @param color Color whete to store result
 * @return true if conversion is possible, otherwise false
 */
static bool decodeColor(P4SerialContext * const context, unsigned char * const color) {
  char mshb = p4PopReceivedData(context);
  char lshb = p4PopReceivedData(context);

  return decodeByte(mshb, lshb, color);
}

/**
 * @brief Decode a RGB
 *
 * @param context Serial context containing received data
 * @param color Color where to store result
 * @return true if conversion is possible, otherwise false
 */
static bool decodeRGB(P4SerialContext * const context, P4Color * const color) {
  return decodeColor(context, &(color->red)) && 
         decodeColor(context, &(color->green)) &&
         decodeColor(context, &(color->blue));
}

/**
 * @brief Check if point is correctly decoded
 *
 * @param point Decoded point to check
 * @return true if is a valid point, otherwise false
 */
inline static bool isCorrectlyDecodedPoint(const P4MatrixPoint point) {
  return point.row != -1 && point.column != -1;
}

/**
 * @brief Check if zone is correctly decoded
 *
 * @param zone Decoded zone to check
 * @return true is is a valid zone, otherwise false
 */
inline static bool isCorrectlyDecodedZone(const P4MatrixZone zone) {
  return isCorrectlyDecodedPoint(zone.startPoint) && isCorrectlyDecodedPoint(zone.endPoint);
}

/**
 * @brief Decode a point
 * 
 * @param context Serial context receiving command
 * @param point Structure to store decodage
 * @return true if conversion is possible, otherwise false
 */
inline static bool decodePoint(P4SerialContext * const context, P4MatrixPoint * const point) {
  point->row = decodeRow(p4PopReceivedData(context));
  point->column = decodeColumn(p4PopReceivedData(context));

  return isCorrectlyDecodedPoint(*point);
}

/**
 * @brief Decode a zone
 *
 * @param context Serial context receiving command
 * @param zone Structure to store decodage
 * @return true if conversion is possible, otherwise false
 */
inline static bool decodeZone(P4SerialContext * const context, P4MatrixZone * const zone) {
  return decodePoint(context, &(zone->startPoint)) && decodePoint(context, &(zone->endPoint));
}

/**
 * @brief Decode a ring command, and calls specific callback if command
 *        is correct
 *
 * @param context Serial context receiving command
 */
static void decodeRingColorCommand(P4SerialContext * const context) {
  P4MatrixPoint ring = { 0 };

  if (!decodePoint(context, &ring)) {
    return;
  }

  P4Color led_colors[P4_BOARD_NB_LEDS_PER_RING] = { { 0 } };
  for(int i = 0; i < P4_BOARD_NB_LEDS_PER_RING; ++i) {
    if (!decodeRGB(context, &(led_colors[i]))) {
      return;
    }
  }

  context->ringColorCallback(context->cookie, ring, P4_BOARD_NB_LEDS_PER_RING, led_colors);
}

/**
 * @brief Decode a zone color command and calls specific callback if command
 *        is correct
 *
 * @param context Serial context receiving command
 */
static void decodeZoneColorCommand(P4SerialContext * const context) {
  P4MatrixZone zone = { 0 };
  P4Color color = { 0 };

  if (decodeZone(context, &zone) && decodeRGB(context, &color)) {
    context->zoneColorCallback(context->cookie, zone, color);
  }
}

/**
 * @brief Decode a zone on command and calls specific callback if command is correct
 *
 * @param context Serial context receiving command
 */
static void decodeZoneOnCommand(P4SerialContext * const context) {
  P4MatrixZone zone = { 0 };

  if (decodeZone(context, &zone)) {
    context->zoneOnCallback(context->cookie, zone);
  }
}

/**
 * @brief Decode a zone on command and calls specific callback if command is correct
 *
 * @param context Serial context receiving command
 */
static void decodeZoneOffCommand(P4SerialContext * const context) {
  P4MatrixZone zone = { 0 };

  if (decodeZone(context, &zone)) {
    context->zoneOffCallback(context->cookie, zone);
  }
}

/**
 * @brief Decode an intensity
 *
 * @param context Serial context receiving command
 * @param intensity Pointer to variable which will store conversion
 * @return true if conversion is possible, otherwise false
 */
static bool decodeIntensity(P4SerialContext * const context, P4Intensity * const intensity) {
  char mshb = p4PopReceivedData(context);
  char lshb = p4PopReceivedData(context);

  return decodeByte(mshb, lshb, intensity);
}

/**
 * @brief Decode a zone intensity command and calls specific callback if command is correct
 *
 * @param context Serial context receiving command
 */
static void decodeZoneIntensityCommand(P4SerialContext * const context) {
  P4MatrixZone zone = { 0 };
  P4Intensity intensity = 0;

  if (decodeZone(context, &zone) && decodeIntensity(context, &intensity)) {
    context->zoneIntensityCallback(context->cookie, zone, intensity);
  }
}

/**
 * @brief Decode a short ring command and calls specific callback if command is correct
 *
 * @param context Serial context receiving command
 */
static void decodeShortRingCommand(P4SerialContext * const context) {
  P4MatrixPoint ring = { 0 };
  P4Color color = { 0 };

  if (decodePoint(context, &ring) && decodeRGB(context, &color)) {
    context->shortRingColorCallback(context->cookie, ring, color);
  }
}

/**
 * @brief Decode an delay
 *
 * @param context Serial context receiving command
 * @param delay Point to variable which will store conversion
 * @return true if conversion is possible, otherwise false
 */

/**
 * @brief list of command from core to displayer, for displayer
 */
static const P4Command commands[] = {
  { P4_CMD_ZONE_COLOR_CMD, P4_CMD_ZONE_COLOR_SIZE, decodeZoneColorCommand },
  { P4_CMD_ZONE_ON_CMD, P4_CMD_ZONE_ON_SIZE, decodeZoneOnCommand },
  { P4_CMD_ZONE_OFF_CMD, P4_CMD_ZONE_OFF_SIZE, decodeZoneOffCommand },
  { P4_CMD_ZONE_INTENSITY_CMD, P4_CMD_ZONE_INTENSITY_SIZE, decodeZoneIntensityCommand },
  { P4_CMD_SHORT_RING_CMD, P4_CMD_SHORT_RING_SIZE, decodeShortRingCommand },
  { P4_CMD_RING_COLOR_CMD, P4_CMD_RING_COLOR_SIZE, decodeRingColorCommand },
  { '\0', 0, NULL } // Terminating command
};

P4ReturnCode p4ReceiverInitFromCore(P4SerialContext * const context) {
  return p4ReceiverInit(context);
}

P4ReturnCode p4AccumulateFromCore(P4SerialContext * const context, const char data) {
  return p4Accumulate(context, commands, data);
}
