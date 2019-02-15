/*
 * Device_CommandsService.h
 *
 *  Created on: Feb 12, 2019
 *      Author: Vitaliy_Vlasov
 *
 *      This service used to execute remote commands from client.
 *
 *      a.e. restart device, delete some files or folders
 */

#ifndef LIBRARIES_DEVICELIB_SERVICES_COMMANDS_DEVICE_COMMANDSSERVICE_H_
#define LIBRARIES_DEVICELIB_SERVICES_COMMANDS_DEVICE_COMMANDSSERVICE_H_

#include <Arduino.h>
#include <interfaces/EntityServiceBase.h>
#include <TimeTrigger.h>

#include <interfaces/Loopable.h>

const char COMMAND_RESTART[] PROGMEM ="restart";

enum Command: uint8_t {
	Command_restart
};
const char* const COMMAND_NAME[] PROGMEM = {
		COMMAND_RESTART
};

class Device_CommandsService:public Loopable, public EntityServiceBase{
public:
	uint8_t getEntityId();

	Device_CommandsService();
	virtual ~Device_CommandsService(){};

	boolean loop();

	virtual int postAbstractItems(JsonArray& items,uint8_t pageId);
	virtual int getAbstractItems(JsonArray& items,uint8_t pageId);

private:
	void createPostponedCommand(String command);
	void prepareTrigger(long interval);
	void executePostponedCommand();

	String postponedCommand;

	long triggerTime;
	boolean commandTriggered;
};

#endif /* LIBRARIES_DEVICELIB_SERVICES_COMMANDS_DEVICE_COMMANDSSERVICE_H_ */
