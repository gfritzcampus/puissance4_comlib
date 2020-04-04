#ifndef _P4_BOARD_RECEIVER
#define _P4_BOARD_RECEIVER

#include "common.h"

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
P4ReturnCode p4AccumulateFromCore(P4SerialContext * const context, const char data);

#endif
