#ifndef _P4_COMMON
#define _P4_COMMON

#include <stddef.h>
#include <stdbool.h>

#define P4_CMD_DELIMITER '\n'

#define P4_CMD_ZONE_INTENSITY_SIZE (8)
#define P4_CMD_ZONE_INTENSITY_CMD 'i'

#define P4_CMD_LIGHT_SENSOR_SIZE (4)
#define P4_CMD_LIGHT_SENSOR_CMD 'l'

#define P4_CMD_PLAYER_SIZE (5)
#define P4_CMD_PLAYER_CMD 'p'

#ifndef P4_BOARD_NB_ROWS
  #define P4_BOARD_NB_ROWS (7)
#endif

#ifndef P4_BOARD_NB_COLUMNS
  #define P4_BOARD_NB_COLUMNS (7)
#endif

#define P4_CMD_SHORT_RING_SIZE (10)
#define P4_CMD_SHORT_RING_CMD 'R'

/**
 * @brief Store point of display matrix
 */
typedef struct {
  unsigned char row;   //!< Line identifying point
  unsigned char column; //!< Column identifying point
} P4MatrixPoint;

/**
 * @brief Store zone of display matrix
 */
typedef struct {
  P4MatrixPoint startPoint; //!< Starting point of matrix (including)
  P4MatrixPoint endPoint;   //!< Ending point of matrix (including)
} P4MatrixZone;

/**
 * @brief Store a color, in RGB format
 */
typedef struct {
  unsigned char red;   //!< Red value of RGB color, 0 to 255
  unsigned char green; //!< Green value of RGB color, 0 to 255
  unsigned char blue;  //!< Blue value of RGB color, 0 to 255
} P4Color;

/**
 * @brief Black color
 */
static const P4Color P4BlackColor = { 
  .red = 0, 
  .green = 0, 
  .blue = 0
};

/**
 * @brief White color
 */
static const P4Color P4WhiteColor = { 
  .red = 255, 
  .green = 255, 
  .blue = 255
};

/**
 * @brief Red color
 */
static const P4Color P4RedColor = {
  .red = 255, 
  .green = 0, 
  .blue = 0
};

/**
 * @brief Green color
 */
static const P4Color P4GreenColor = {
  .red = 0, 
  .green = 255, 
  .blue = 0
};

/**
 * @brief Blue color
 */
static const P4Color P4BlueColor = {
  .red = 255, 
  .green = 0, 
  .blue = 0
};

/**
 * @brief Yellow color
 */
static const P4Color P4YellowColor = {
  .red = 255, 
  .green = 255, 
  .blue = 0
};

/**
 * @brief Cyan color
 */
static const P4Color P4CyanColor = {
  .red = 0,
  .green = 255,
  .blue = 255
};

/**
 * @brief Player identifier
 */
typedef enum {
  P4P_PLAYER_1 = '1',
  P4P_PLAYER_2 = '2',
} P4Player;

/**
 * @brief Status of player action
 */
typedef enum {
  P4AS_DOWN= 'd',    //!< Player press action button
  P4AS_UP = 'u'   //!< Player release action button
} P4ActionStatus;

/**
 * @brief Player's action
 */
typedef enum {
  P4A_LEFT = 'l',   //!< Left button
  P4A_RIGHT = 'r',  //!< Right button
  P4A_UP = 'u',     //!< Up button
  P4A_DOWN = 'd'    //!< Down button
} P4Action;

//! Value used when get an unknown value
#define P4_UNKNOWN '\0'

/**
 * @brief Return code of functions
 */
typedef enum {
  P4RC_OK,                  //!< Everything has correctly run
  P4RC_INVALID_PARAMETERS,  //!< Parameters of functions are invalid
  P4RC_SEND_ERROR,          //!< Error sending command
  P4RC_BUFFER_FULL,         //!< Buffer full
  P4RC_DECODE_ERROR,        //!< Received command can't be decoded
} P4ReturnCode;

//! Store an intensity
typedef unsigned char P4Intensity;

//! Store a light sensor
typedef unsigned char P4LightSensor;

/**
 * @brief describe a ring buffer to accumulate data
 */
typedef struct {
  char * data;                                          //!< Buffer to store received bytes before decode (minimal size : 256 bytes)
  size_t size;                                          //!< Size of buffer in bytes
  size_t head;                                          //!< Head of ring buffer, ie index of first empty cell
  size_t tail;                                          //!< Tail of ring buffer, ie index of first available data
  bool isFull;                                          //!< Store if ring buffer is full
  bool inError;                                         //!< Store if ring buffer is in error
} P4RingBuffer;

#define INIT_BUFFER(BUFFER, SIZE) { \
  .data = BUFFER,\
  .size = SIZE, \
  .head = 0,\
  .tail = 0,\
  .isFull = false,\
  .inError = false\
}

/**
 * @brief Store serial context used to send command
 */
typedef struct 
{
  //! Function used to send data
  size_t (*send)(const void * const buffer, size_t size);

  //! Buffer used to store received data before decoding
  P4RingBuffer buffer;
} P4SerialContext;

/**
 * @brief Check if is a valid matrix point : column should be between
 *        0 and P4_BOARD_NB_COLUMNS not included and row should be 
 *        between 0 and P4_BOARD_NB_ROWS not included
 *
 * @param point Matrix point to check
 * @return true if is a valid matrix point, otherwise false
 */
static inline bool isValidMatrixPoint(P4MatrixPoint point)
{
  return 0 < point.row && point.row < P4_BOARD_NB_ROWS
      && 0 < point.column && point.column < P4_BOARD_NB_COLUMNS;
}

/**
 * @brief Check if is a valid matrix zone, ie check if startPoint and
 *        endPoint are valid matrix points
 *
 * @param zone Matrix zone to check
 * @return true if is a valid matrix zone, otherwise false
 */
static inline bool isValidMatrixZone(P4MatrixZone zone)
{
  return isValidMatrixPoint(zone.startPoint) &&
         isValidMatrixPoint(zone.endPoint);
}

/**
 * @brief encode number between 0 and 15 in its hexadecimal representation
 *
 * @param i Number to encode
 * @return [0-9A-F] if 0 < i < 16; X otherwise
 */
static inline unsigned char toHexaHalfByte(const unsigned char i) {
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

/**
 * @brief encode byte in its hexadecimal representation
 *
 * @param buffer Buffer to store result, should be at least size 2
 * @param byte Byte to encode
 */
static inline void encodeByte(unsigned char * const buffer, const unsigned char byte) {
  buffer[0] = toHexaHalfByte((byte >> 4) & 0x0F); 
  buffer[1] = toHexaHalfByte(byte & 0x0F);
}

#endif
