/*
 * DeviceManager.cpp
 *
 *  Created on: Oct 7, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <serve/DeviceManager.h>

void DeviceManager::doGet(JsonObject& params, JsonObject& response) {
	UNUSED(params);
	response[DEVICE_MANAGER_FIELD_RESTART] = this->triggeredRestart;
	//response[DEVICE_MANAGER_FIELD_FREE_HEAP] = ESP.getFreeHeap();
}

void DeviceManager::doAppendFieldsSwg(JsonObject& swgJson){
	EntityDescriptor::appendSwgFieldString(swgJson, ENTITY_FIELD_DESCRIPTION, EDC_DESCR_SERVICE);

	EntityDescriptor::appendSwgFieldBooleanCommand(swgJson,
			DEVICE_MANAGER_FIELD_RESTART, nullptr,
			false, nullptr, EDC_FIELD_BOOLEAN_VALUE_ON, EDC_DESCR_RESTART, EDC_DESCR_ACTION_RESTART);
}

void DeviceManager::doPost(JsonObject& params, JsonObject& response) {
	UNUSED(response);
	//JsonObjectUtil::print(params);
	if(JsonObjectUtil::getFieldBooleanValue(params,DEVICE_MANAGER_FIELD_RESTART)){
		triggerRestart();
		addMessage(response, MESSAGE_SEVERITY_WARN, MESSAGE_CODE_DEVICE_WILL_BE_RESTARTED);
	} else {
		Serial.println(FPSTR("NO restart triggered"));
	}
}

void DeviceManager::loop() {
	if(this->triggeredRestart) {
		if(restartTime < millis()){
			Serial.println(FPSTR("RESTART device"));
			DeviceUtils::restart();
		}
	}

}

void DeviceManager::doUpdate() {
	#ifdef DEVICE_MANAGER_PRINT_HEAP
		DeviceUtils::printHeap();
	#endif
	#ifdef DEVICE_MANAGER_MIN_HEAP_RESTART
		triggerRestart(DeviceUtils::isHeapLessThan(DEVICE_MANAGER_MIN_HEAP_RESTART));
	#endif
}

void DeviceManager::triggerRestart(bool doRestart) {
	this->triggeredRestart = doRestart;
	if(this->triggeredRestart){
		Serial.println(FPSTR(" Restart Triggered"));
		this->restartTime = millis() + 3000;
	}

}