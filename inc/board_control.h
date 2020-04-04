#ifndef _P4_BOARD_CONTROL
#define _P4_BOARD_CONTROL

#include "common.h"

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
P4ReturnCode p4SetZoneIntensity(const P4SerialContext * const context, const P4MatrixZone zone, const P4Intensity intensity);

/**
 * @brief Change color of a specific ring
 *        To turn off, send black color
 *
 * @param context Serial to use to send command
 * @param ring Coordonates of ring
 * @param color New color
 * @return - P4RC_INVALID_PARAMETERS if invalid parameters
 *         - P4RC_SEND_ERROR if error sending command
 *         - P4RC_OK otherwise
 */
P4ReturnCode p4SetRingColor(const P4SerialContext * const context, const P4MatrixPoint ring, const P4Color color);

#endif
