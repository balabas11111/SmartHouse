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

uint8_t Device_CommandsService::getEntityId() {
	return Entity_commands;
}

int Device_CommandsService::postAbstractItems(JsonArray& items, uint8_t pageId) {

	if(pageId==Page_execute && items.size()>0){
		for(uint8_t i=0;i<items.size();i++){
			createPostponedCommand(items[i]["name"]);
		}

		return HTTP_CODE_OK;
	}

	return HTTP_CODE_NOT_IMPLEMENTED;
}

int Device_CommandsService::getAbstractItems(JsonArray& items,
		uint8_t pageId) {

	for(uint8_t i=0;i < sizeof(COMMAND_NAME);i++){
		JsonObject& item=items.createNestedObject();
		item["id"] = i;
		item["name"] = COMMAND_NAME[i];
	}

	return HTTP_CODE_OK;
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

