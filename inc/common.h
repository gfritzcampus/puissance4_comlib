#ifndef _P4_COMMON
#define _P4_COMMON

#include <stddef.h>
#include <stdbool.h>

#define P4_CMD_DELIMITER '\n'

#define P4_CMD_ZONE_COLOR_SIZE (12)
#define P4_CMD_ZONE_COLOR_CMD 'c'

#define P4_CMD_ZONE_ON_SIZE (6)
#define P4_CMD_ZONE_ON_CMD 'O'

#define P4_CMD_ZONE_OFF_SIZE (6)
#define P4_CMD_ZONE_OFF_CMD 'o'

#define P4_CMD_ZONE_INTENSITY_SIZE (8)
#define P4_CMD_ZONE_INTENSITY_CMD 'i'

#define P4_CMD_ZONE_BLINK_SIZE (14)
#define P4_CMD_ZONE_BLINK_CMD 'b'

#define P4_CMD_PLAYER_SIZE (5)
#define P4_CMD_PLAYER_CMD 'p'


#ifndef P4_BOARD_NB_ROWS
  #define P4_BOARD_NB_ROWS (7)
#endif

#ifndef P4_BOARD_NB_COLUMNS
  #define P4_BOARD_NB_COLUMNS (7)
#endif

#ifndef P4_BOARD_NB_LEDS_PER_RING
  #define P4_BOARD_NB_LEDS_PER_RING (1)
#endif

/**
 * @brief Store serial context used to send command
 */
typedef struct 
{
  size_t (*send)(void * buffer, size_t size);
} P4SerialContext;

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
static const P4Color P4BlackP4Color = { 
  .red = 0, 
  .green = 0, 
  .blue = 0
};

/**
 * @brief White color
 */
static const P4Color P4WhiteP4Color = { 
  .red = 255, 
  .green = 255, 
  .blue = 255
};

/**
 * @brief Red color
 */
static const P4Color P4Red = {
  .red = 255, 
  .green = 0, 
  .blue = 0
};

/**
 * @brief Green color
 */
static const P4Color P4Green = {
  .red = 0, 
  .green = 255, 
  .blue = 0
};

/**
 * @brief Blue color
 */
static const P4Color P4Blue = {
  .red = 255, 
  .green = 0, 
  .blue = 0
};

/**
 * @brief Yellow color
 */
static const P4Color P4Yellow = {
  .red = 255, 
  .green = 255, 
  .blue = 0
};

/**
 * @brief Player identifier
 */
typedef enum {
  P4P_PLAYER_1,
  P4P_PLAYER_2
} P4Player;

/**
 * @brief Status of player action
 */
typedef enum {
  P4AS_PRESS,    //!< Player press action button
  P4AS_RELEASE   //!< Player release action button
} P4ActionStatus;

/**
 * @brief Player's action
 */
typedef enum {
  P4A_LEFT,   //!< Left button
  P4A_RIGHT,  //!< Right button
  P4A_UP,     //!< Up button
  P4A_DOWN    //!< Down button
} P4Action;

/**
 * @brief Return code of functions
 */
typedef enum {
  P4RC_OK,                  //!< Everything has correctly run
  P4RC_INVALID_PARAMETERS,  //!< Parameters of functions are invalid
  P4RC_SEND_ERROR           //!< Error sending command
} P4ReturnCode;

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

#endif
