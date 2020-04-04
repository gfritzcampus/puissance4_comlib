#ifndef _P4_DISPLAYER_RECEIVER
#define _P4_DISPLAYER_RECEIVER

#include "common.h"

/**
 * @brief Type of received command
 */
typedef enum {
  P4DCT_None,
  P4DCT_PlayerCommand,
  P4DCT_LightSensor,
} P4DisplayerCommandType;

/**
 * @brief Store decoded received command and its params
 *
 * Usage : 
 * @code{.c}
 * P4DisplayerCommand command;
 * // Received a player command :
 * if (command.type == P4DC_PlayerCommand) {
 *   command.player.player;
 *   command.player.action;
 *   command.player.status;
 * }
 *
 * // Received a light sensor command :
 * if (command.type == P4DC_LightSensor) {
 *   command.lightsensor.value;
 * }
 * @endcode
 *
 */
typedef struct {
  //! Store return of decoding
  P4ReturnCode returnCode;
  //! Store Type of received command
  P4DisplayerCommandType type;
  //! Store parameters of received command, according type of the one
  union {
    //! Store parameters of Player Command
    struct {
      //! Store player who do action
      P4Player player;
      //! Store action of player
      P4Action action;
      //! Store status of action
      P4ActionStatus status;
    } player;
    //! Store parameters of Light Sensor Command
    struct {
      //! Store value of light sensor
      P4LightSensor value;
    } lightsensor;
  };
} P4DisplayerCommand;

/**
 * @brief init receiver part of serial context, when receiving from board displayer
 *        to core
 *
 * @param context Context to initialize
 * @return P4RC_INVALID_PARAMETERS if size of allocated buffer is not enough, 
 *         P4RC_OK otherwise
 */
P4ReturnCode p4ReceiverInitFromDisplayer(P4SerialContext * const context);

/**
 * @brief Accumulate and decode commands received from board displayer to core
 *
 * @param context Serial context
 * @parma data Byte received
 */
P4DisplayerCommand p4AccumulateFromDisplayer(P4SerialContext * const context, const char data);

#endif
