#ifndef _P4_DISPLAYER_CONTROL
#define _P4_DISPLAYER_CONTROL

#include "common.h"

/**
 * @brief Send player action
 *
 * @param context Serial to use to send command
 * @param player Player who act
 * @param action Action performs by player
 * @param status Status of action
 * @return - P4RC_INVALID_PARAMETERS if invalid parameters
 *         - P4RC_SEND_ERROR if error sending command
 *         - P4RC_OK otherwise
 */
P4ReturnCode p4PlayerPress(const P4SerialContext * const context, const P4Player player, const P4Action action, const P4ActionStatus status);

/**
 * @brief Send value of light sensor
 *
 * @param context Serial context to use to send command
 * @param lightSensorValue
 * @return - P4RC_INVALID_PARAMETERS if invalid parameters
 *         - P4RC_SEND_ERROR if error sending command
 *         - P4RC_OK otherwise
 */
P4ReturnCode p4LightSensor(const P4SerialContext * const context, const P4LightSensor lightSensorValue);

#endif
