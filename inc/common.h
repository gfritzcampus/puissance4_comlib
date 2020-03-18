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
  #define P4_BOARD_NB_LEDS_PER_RING (24)
#endif

#define P4_CMD_RING_COLOR_SIZE (4 + 3 * 2 * P4_BOARD_NB_LEDS_PER_RING)
#define P4_CMD_RING_COLOR_CMD 'r'

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
} P4ReturnCode;

//! Store an intensity
typedef unsigned char P4Intensity;

//! Store a delay in milliseconds
typedef unsigned short P4DelayMs;

//! @brief Callback function type to inform that player has do an action
typedef void (*P4PlayerActionCallback)(void *, const P4Player, const P4Action, const P4ActionStatus);

//! @brief Callback function type to inform zone changes a color
typedef void (*P4ZoneColorCallback)(void *, const P4MatrixZone, const P4Color);

//! @brief Callback function type to inform zone is turn on
typedef void (*P4ZoneOnCallback)(void *, const P4MatrixZone);

//! @brief Callback function type to inform zone is turn off
typedef void (*P4ZoneOffCallback)(void *, const P4MatrixZone);

//! @brief Callback function type to inform zone changes intensity
typedef void (*P4ZoneIntensityCallback)(void *, const P4MatrixZone, const P4Intensity);

//! @brief Callback function type to inform zone blink
typedef void (*P4ZoneBlinkCallback)(void *, const P4MatrixZone, const P4DelayMs, const P4DelayMs);

//! @brief Callback function type to inform ring update
typedef void (*P4RingColorCallback)(void *, const P4MatrixPoint, const size_t, const P4Color[]);

//! @brief Callback function type to inform short ring update
typedef void (*P4ShortRingColorCallback)(void *, const P4MatrixPoint, const P4Color);

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

/**
 * @brief Store serial context used to send command
 */
typedef struct 
{
  size_t (*send)(const void * const buffer, size_t size);           //!< Function to send command

  P4RingBuffer buffer;

  P4PlayerActionCallback playerActionCallback;          //!< Callback to use when a player action is received
  P4ZoneColorCallback zoneColorCallback;                //!< Callback to use when a color zone is received
  P4ZoneOnCallback zoneOnCallback;                      //!< Callback to use when a zone on is received
  P4ZoneOffCallback zoneOffCallback;                    //!< Callback to use when a zone off is received
  P4ZoneIntensityCallback zoneIntensityCallback;        //!< Callback to use when an intensity zone is received
  P4ZoneBlinkCallback zoneBlinkCallback;                //!< Callback to use when a zone blink is received
  P4RingColorCallback ringColorCallback;                //!< Callback to use when a ring is updated
  P4ShortRingColorCallback shortRingColorCallback;      //!< Callback to use when a ring is updated using short command

  void * cookie;                                        //!< Store pointer to user data, which will passed as first
                                                        //!< argument of callbacks
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

#endif
