#ifndef _P4_RECEIVER
#define _P4_RECEIVER

#include "common.h"

/**
 * @brief Structure to store context of receiver
 */
typedef struct {
  P4RingBuffer * buffer;
  void * cookie;
} P4ReceiverContext;

/**
 * @brief Structure describing a possible command
 */
typedef struct {
  //! Code on one byte of command
  char code;
  //! Length of command
  size_t length;
  //! Function to decode command
  P4ReturnCode (*decoder)(P4ReceiverContext * const);
} P4Command;

/**
 * @brief init receiver part of serial context
 *
 * @param context Context to initialize
 * @return P4RC_INVALID_PARAMETERS if size of allocated buffer is not enough, 
 *         P4RC_OK otherwise
 */
P4ReturnCode p4ReceiverInit(P4ReceiverContext * const context);

/**
 * @brief Accumulate and decode data received from serial link
 *
 * @param context Context of receiver
 * @param commands Array of commands, ended by a command with code '\0'
 * @param data Byte received by serial link
 */
P4ReturnCode p4Accumulate(P4ReceiverContext * const context, const P4Command * const commands, const char data);


/**
 * @brief Get next byte from received data
 *
 * @param context Context of receiver
 */
char p4PopReceivedData(P4ReceiverContext * const context);


#endif
