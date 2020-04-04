#ifndef _P4_PLAYER_RECEIVER
#define _P4_PLAYER_RECEIVER

#include "common.h"

/**
 * @brief init receiver part of serial context, when receiving from board displayer
 *        to core
 *
 * @param context Context to initialize
 * @return P4RC_INVALID_PARAMETERS if size of allocated buffer is not enough, 
 *         P4RC_OK otherwise
 */
P4ReturnCode p4ReceiverInitFromDisplayer(P4SerialContext * const context);

/**
 * @brief Accumulate and decode commands received from board displayer to core
 *
 * @param context Serial context
 * @parma data Byte received
 */
P4ReturnCode p4AccumulateFromDisplayer(P4SerialContext * const context, const char data);

#endif
