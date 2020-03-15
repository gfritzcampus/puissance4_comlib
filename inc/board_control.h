#ifndef _P4_BOARD_CONTROL
#define _P4_BOARD_CONTROL

#include "common.h"

/**
 * @brief Set color of a display matrix zone
 *
 * @param context Serial to use to send command
 * @param zone Display matrix zone affected by new color
 * @param color New color
 * @return - P4RC_INVALID_PARAMETERS if invalid parameters
 *         - P4RC_SEND_ERROR if error sending command
 *         - P4RC_OK otherwise
 */ 
P4ReturnCode p4SetZoneP4Color(const P4SerialContext context, const P4MatrixZone zone, const P4Color color);

/**
 * @brief Turn on display matrix zone, using color
 *        previously set using setLineP4Color()
 *
 * @param context Serial to use to send command
 * @param zone Display matrix zone to turn on
 * @return - P4RC_INVALID_PARAMETERS if invalid parameters
 *         - P4RC_SEND_ERROR if error sending command
 *         - P4RC_OK otherwise
 */
P4ReturnCode p4SetZoneOn(const P4SerialContext context, const P4MatrixZone zone);

/**
 * @brief Turn off display matrix zone
 *
 * @param context Serial to use to send command
 * @param zone Display matrix zone to turn off
 * @return - P4RC_INVALID_PARAMETERS if invalid parameters
 *         - P4RC_SEND_ERROR if error sending command
 *         - P4RC_OK otherwise
 */
P4ReturnCode p4SetZoneOff(const P4SerialContext context, const P4MatrixZone zone);

/**
 * @brief Change led intensity of a zone
 *
 * @param context Serial to use to send command
 * @param zone Display matrix zone affected by new led intensity
 * @param intensity New led intensity, in pourcentage, from 0-100. If greater than 100%,
 *                  intensity will be set to 100%
 * @return - P4RC_INVALID_PARAMETERS if invalid parameters
 *         - P4RC_SEND_ERROR if error sending command
 *         - P4RC_OK otherwise
 */
P4ReturnCode p4SetZoneIntensity(const P4SerialContext context, const P4MatrixZone zone, const P4Intensity intensity);

/**
 * @brief Change blink mode of a display matrix zone.
 *
 * @param context Serial to use to send command
 * @param zone Display matrix zone affected by new blink mode
 * @param onTime Time in millisecond during which leds will be on
 * @param offTime Time in millisecond during which leds will be off
 * @return - P4RC_INVALID_PARAMETERS if invalid parameters
 *         - P4RC_SEND_ERROR if error sending command
 *         - P4RC_OK otherwise
 */
P4ReturnCode p4SetZoneBlink(const P4SerialContext context, const P4MatrixZone zone, const P4DelayMs onTime, const P4DelayMs offTime);

/**
 * @brief Change color of a specific ring
 *
 * @param context Serial to use to send command
 * @param ring Coordonates of ring
 * @param color New color
 * @return - P4RC_INVALID_PARAMETERS if invalid parameters
 *         - P4RC_SEND_ERROR if error sending command
 *         - P4RC_OK otherwise
 */
P4ReturnCode p4SetRingP4Color(const P4SerialContext context, const P4MatrixPoint ring, const P4Color color);

#endif
