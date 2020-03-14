#ifndef _LEDS_CONTROL
#define _LEDS_CONTROL

/**
 * @brief Store serial context used to send command
 */
typedef void * SerialContext;

/**
 * @brief Store point of display matrix
 */
typedef struct {
  unsigned char line;   //!< Line identifying point
  unsigned char column; //!< Column identifying point
} MatrixPoint;

/**
 * @brief Store zone of display matrix
 */
typedef struct {
  MatrixPoint startPoint; //!< Starting point of matrix (including)
  MatrixPoint endPoint;   //!< Ending point of matrix (including)
} MatrixZone;

/**
 * @brief Store a color, in RGB format
 */
typedef struct {
  unsigned char red;   //!< Red value of RGB color, 0 to 255
  unsigned char green; //!< Green value of RGB color, 0 to 255
  unsigned char blue;  //!< Blue value of RGB color, 0 to 255
} Color;

/**
 * @brief Black color
 */
static const Color BlackColor = { 
  .red = 0, 
  .green = 0, 
  .blue = 0
};

/**
 * @brief White color
 */
static const Color WhiteColor = { 
  .red = 255, 
  .green = 255, 
  .blue = 255
};

/**
 * @brief Red color
 */
static const Color Red = {
  .red = 255, 
  .green = 0, 
  .blue = 0
};

/**
 * @brief Green color
 */
static const Color Green = {
  .red = 0, 
  .green = 255, 
  .blue = 0
};

/**
 * @brief Blue color
 */
static const Color Blue = {
  .red = 255, 
  .green = 0, 
  .blue = 0
};

/**
 * @brief Yellow color
 */
static const Color Yellow = {
  .red = 255, 
  .green = 255, 
  .blue = 0
};

/**
 * @brief Set color of a display matrix zone
 *
 * @param context Serial to use to send command
 * @param zone Display matrix zone affected by new color
 * @param color New color
 */ 
void setZoneColor(const SerialContext context, const MatrixZone zone, const Color color);

/**
 * @brief Turn on display matrix zone, using color
 *        previously set using setLineColor()
 *
 * @param context Serial to use to send command
 * @param zone Display matrix zone to turn on
 */
void setZoneOn(const SerialContext context, const MatrixZone zone);

/**
 * @brief Turn off display matrix zone
 *
 * @param context Serial to use to send command
 * @param zone Display matrix zone to turn off
 */
void setZoneOff(const SerialContext context, const MatrixZone zone);

/**
 * @brief Change led intensity of a zone
 *
 * @param context Serial to use to send command
 * @param zone Display matrix zone affected by new led intensity
 * @param intensity New led intensity, in pourcentage, from 0-100. If greater than 100%,
 *                  intensity will be set to 100%
 */
void setZoneIntensity(const SerialContext context, const MatrixZone zone, const unsigned char intensity);

/**
 * @brief Change blink mode of a display matrix zone.
 *
 * @param context Serial to use to send command
 * @param zone Display matrix zone affected by new blink mode
 * @param onTime Time in millisecond during which leds will be on
 * @param offTime Time in millisecond during which leds will be off
 */
void setZoneBlink(const SerialContext context, const MatrixZone zone, const unsigned short onTime, const unsigned short offTime);

/**
 * @brief Change color of a specific ring
 *
 * @param context Serial to use to send command
 * @param ring Coordonates of ring
 * @param color New color
 */
void setRingColor(const SerialContext context, const MatrixPoint ring, const Color color);

#endif
