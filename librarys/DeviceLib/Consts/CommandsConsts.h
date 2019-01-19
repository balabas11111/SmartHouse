/*
 * CommandsConsts.h
 *
 *  Created on: 19 янв. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_CONSTS_COMMANDSCONSTS_H_
#define LIBRARIES_DEVICELIB_CONSTS_COMMANDSCONSTS_H_

#include "Arduino.h"

//commands available to porcess on device

const PROGMEM char COMMAND_RESTART[]               = "restart";
const PROGMEM char COMMAND_DELETE_SETTINGS[]       = "deleteSettings";
const PROGMEM char COMMAND_RECREATE_CHANNELS[]     = "recreateChannels";

#endif /* LIBRARIES_DEVICELIB_CONSTS_COMMANDSCONSTS_H_ */
