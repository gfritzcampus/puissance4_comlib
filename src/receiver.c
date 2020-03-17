#include "receiver.h"

/**
 * @brief check if ring buffer is full
 *
 * @param buffer Ring buffer to check
 * @return true if ring buffer is full, otherwise false
 */
inline static bool isFullBuffer(const P4RingBuffer * const buffer) {
  return buffer->isFull;
}

/**
 * @brief check if ring buffer is empty
 *
 * @param buffer Ring buffer to check
 * @return true if ring buffer is empty, otherwise false
 */
inline static bool isEmptyBuffer(const P4RingBuffer * const buffer) {
  return !buffer->isFull && buffer->head == buffer->tail;
}

/**
 * @brief get number of elements stored in ring buffer
 *
 * @param buffer Ring buffer containing data
 * @return number of elements stored in ring buffer
 */
inline static size_t sizeBuffer(const P4RingBuffer * const buffer) {
  size_t size = buffer->size;

  if (!isFullBuffer(buffer)) {
    if (buffer->head >= buffer->tail) {
      size = buffer->head - buffer->tail;
    }
    else {
      size = buffer->size + buffer->head - buffer->tail;
    }
  }
  
  return size;
}

/**
 * @brief Add byte in ring buffer if not empty
 *
 * @param buffer Buffer which will store data
 * @param data Data to store inside rig buffer
 */
inline static void pushByte(P4RingBuffer * const buffer, const char data) {
  if (isFullBuffer(buffer)) {
    return;
  }

  buffer->data[buffer->head] = data;

  buffer->head = (buffer->head + 1) % buffer->size;
  buffer->isFull = (buffer->head == buffer->tail);
}

/**
 * @brief Clear ring buffer, ie delete every data inside ring buffer
 *
 * @param buffer Ring buffer to clear
 */
inline static void clearBuffer(P4RingBuffer * const buffer) {
  buffer->head = 0;
  buffer->tail = 0;
  buffer->isFull = false;
  buffer->inError = false;
}

/**
 * @brief Try to decode commands containing inside ring buffer
 *
 * @param context Serial context
 * @param commands Array of commands, last should be a command with code equal to '\0'
 */
inline static void decodeCommand(P4SerialContext * const context, const P4Command * const commands) {
  const P4Command * current_cmd = commands;
  P4RingBuffer * const buffer = &context->buffer;

  while(current_cmd->code != '\0') {
    if (current_cmd->code == buffer->data[buffer->tail] &&
        current_cmd->length == (sizeBuffer(buffer) + 1)) {
      p4PopReceivedData(context);
      current_cmd->decoder(context);
    }
  }

  clearBuffer(buffer);
}

P4ReturnCode p4ReceiverInit(P4SerialContext * context) {
  clearBuffer(&context->buffer);

  return P4RC_OK;
}

P4ReturnCode p4Accumulate(P4SerialContext * const context, const P4Command * const commands, const char data) {
  if (isFullBuffer(&context->buffer)) {
    clearBuffer(&context->buffer);
    context->buffer.inError = true;
    return P4RC_BUFFER_FULL;
  }

  if (data == P4_CMD_DELIMITER) {
    if (context->buffer.inError) {
      context->buffer.inError = false;
    }
    else {
      decodeCommand(context, commands);
    }
  }
  else {
    if (!context->buffer.inError) {
      pushByte(&context->buffer, data);
    }
  }
  
  return P4RC_OK;
}

char p4PopReceivedData(P4SerialContext * const context) {
  size_t idx = context->buffer.tail;

  context->buffer.isFull = false;
  context->buffer.tail = (context->buffer.tail + 1) % context->buffer.size;

  return context->buffer.data[idx];
}

