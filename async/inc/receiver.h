#ifndef _P4_RECEIVER
#define _P4_RECEIVER

#include "common.h"

/**
 * @brief Structure describing a possible command
 */
typedef struct {
  //! Code on one byte of command
  char code;
  //! Length of command
  size_t length;
  //! Function to decode command
  void (*decoder)(P4SerialContext * const);
} P4Command;

/**
 * @brief init receiver part of serial context
 *
 * @param context Context to initialize
 * @return P4RC_INVALID_PARAMETERS if size of allocated buffer is not enough, 
 *         P4RC_OK otherwise
 */
P4ReturnCode p4ReceiverInit(P4SerialContext * const context);

/**
 * @brief Accumulate and decode data received from serial link
 *
 * @param context Context of serial link
 * @param commands Array of commands, ended by a command with code '\0'
 * @param data Byte received by serial link
 */
P4ReturnCode p4Accumulate(P4SerialContext * const context, const P4Command * const commands, const char data);


/**
 * @brief Get next byte from received data
 *
 * @param context Context of serial link
 */
char p4PopReceivedData(P4SerialContext * const context);


#endif
