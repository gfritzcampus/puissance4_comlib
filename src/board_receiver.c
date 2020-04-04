#include "board_receiver.h"

#include "receiver.h"
#include "decode.h"

/**
 * @brief Decode a row number and check if correct
 *
 * @param row Value to decode
 * @return Row number if is a valid row number, otherwise -1
 */
static unsigned char decodeRow(const char row) {
  unsigned char decoded = decodeHalfByte(row);
  if (decoded < P4_BOARD_NB_ROWS) {
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
  if (decoded < P4_BOARD_NB_COLUMNS) {
    return decoded;
  }
  return -1;
}

/**
 * @brief Decode a basic color
 *
 * @param context Serial context containing received data
 * @param color Color whete to store result
 * @return true if conversion is possible, otherwise false
 */
static bool decodeColor(P4ReceiverContext * const context, unsigned char * const color) {
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
static bool decodeRGB(P4ReceiverContext* const context, P4Color * const color) {
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
 * @brief Decode a point
 * 
 * @param context Serial context receiving command
 * @param point Structure to store decodage
 * @return true if conversion is possible, otherwise false
 */
inline static bool decodePoint(P4ReceiverContext * const context, P4MatrixPoint * const point) {
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
inline static bool decodeZone(P4ReceiverContext * const context, P4MatrixZone * const zone) {
  return decodePoint(context, &(zone->startPoint)) && decodePoint(context, &(zone->endPoint));
}

/**
 * @brief Decode an intensity
 *
 * @param context Serial context receiving command
 * @param intensity Pointer to variable which will store conversion
 * @return true if conversion is possible, otherwise false
 */
static bool decodeIntensity(P4ReceiverContext * const context, P4Intensity * const intensity) {
  char mshb = p4PopReceivedData(context);
  char lshb = p4PopReceivedData(context);

  return decodeByte(mshb, lshb, intensity);
}

/**
 * @brief Decode a zone intensity command and calls specific callback if command is correct
 *
 * @param context Serial context receiving command
 */
static P4ReturnCode decodeZoneIntensityCommand(P4ReceiverContext* const context) {
  P4BoardCommand * const boardCommand = (P4BoardCommand *) context->cookie;
  boardCommand->type = P4BCT_ZoneIntensity;

  return decodeZone(context, &boardCommand->zoneIntensity.zone) 
      && decodeIntensity(context, &boardCommand->zoneIntensity.intensity) ? P4RC_OK : P4RC_DECODE_ERROR; 
}

/**
 * @brief Decode a short ring command and calls specific callback if command is correct
 *
 * @param context Serial context receiving command
 */
static P4ReturnCode decodeShortRingCommand(P4ReceiverContext * const context) {
  P4BoardCommand * const boardCommand = (P4BoardCommand *) context->cookie;
  boardCommand->type = P4BCT_RingColor;

  return decodePoint(context, &boardCommand->ringColor.point) 
    && decodeRGB(context, &boardCommand->ringColor.color) ? P4RC_OK : P4RC_DECODE_ERROR; 
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
  { P4_CMD_ZONE_INTENSITY_CMD, P4_CMD_ZONE_INTENSITY_SIZE, decodeZoneIntensityCommand },
  { P4_CMD_SHORT_RING_CMD, P4_CMD_SHORT_RING_SIZE, decodeShortRingCommand },
  { '\0', 0, NULL } // Terminating command
};

P4ReturnCode p4ReceiverInitFromCore(P4SerialContext * const context) {
  P4ReceiverContext rcontext = {
    .buffer = &context->buffer,
    .cookie = NULL
  };
  return p4ReceiverInit(&rcontext);
}

P4BoardCommand p4AccumulateFromCore(P4SerialContext * const context, const char data) {
  P4BoardCommand result = {  
      .returnCode = P4RC_OK,
      .type = P4BCT_None
    };

  P4ReceiverContext rcontext = {
      .buffer = &context->buffer,
      .cookie = &result
    };

  result.returnCode = p4Accumulate(&rcontext, commands, data);

  return result;
}
