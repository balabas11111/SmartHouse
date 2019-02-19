/*
 * Device_CommandsService.cpp
 *
 *  Created on: Feb 12, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <services/commands/Device_CommandsService.h>

Device_CommandsService::Device_CommandsService() {
	triggerTime=0;
	commandTriggered=false;
}

boolean Device_CommandsService::loop(){
	if(commandTriggered && millis()>triggerTime){
		executePostponedCommand();
		return true;
	}
	return false;
}

void Device_CommandsService::createPostponedCommand(String command) {
	prepareTrigger(5000);
	postponedCommand=command;
	Serial.println(FPSTR("post poned restart trigger started"));
}

void Device_CommandsService::prepareTrigger(long interval) {
	if(!commandTriggered){
		triggerTime=millis()+interval;
	}
}

void Device_CommandsService::executePostponedCommand() {
	if(commandTriggered){
		commandTriggered=false;
	}

	if(postponedCommand=="restart"){
		Serial.print("Executing restart");
		ESP.restart();
	}
}

