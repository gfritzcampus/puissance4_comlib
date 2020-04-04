#ifndef _P4_BOARD_RECEIVER
#define _P4_BOARD_RECEIVER

#include "common.h"

typedef enum {
  P4BCT_None,
  P4BCT_RingColor,
  P4BCT_ZoneIntensity
} P4BoardCommandType ;

typedef struct {
  P4ReturnCode returnCode;
  P4BoardCommandType type;
  union {
    struct {
      P4MatrixPoint point;
      P4Color color;
    } ringColor;
    struct {
      P4MatrixZone zone;
      P4Intensity intensity;
    } zoneIntensity;
  };
} P4BoardCommand;

/**
 * @brief init receiver part of serial context, when receiving from core
 *        to board displayer
 *
 * @param context Context to initialize
 * @return P4RC_INVALID_PARAMETERS if size of allocated buffer is not enough, 
 *         P4RC_OK otherwise
 */
P4ReturnCode p4ReceiverInitFromCore(P4SerialContext * const context);

/**
 * @brief Accumulate and decode commands received from core to board displayer
 *
 * @param context Serial context
 * @parma data Byte received
 */
P4BoardCommand p4AccumulateFromCore(P4SerialContext * const context, const char data);

#endif
