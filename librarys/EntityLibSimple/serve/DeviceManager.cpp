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
	response[DEVICE_MANAGER_FIELD_FREE_HEAP] = ESP.getFreeHeap();
}

void DeviceManager::doPost(JsonObject& params, JsonObject& response) {
	UNUSED(response);

	if(JsonObjectUtil::getObjectFieldExistsAndEquals(params, DEVICE_MANAGER_FIELD_RESTART, 1)
		|| JsonObjectUtil::getObjectFieldExistsAndEquals(params, DEVICE_MANAGER_FIELD_RESTART, "1")){

		triggerRestart();
	}
}

void DeviceManager::loop() {
	if(this->triggeredRestart){
		DeviceUtils::restart();
	}
}

void DeviceManager::doUpdate() {
	#if defined(DEVICE_MANAGER_PRINT_HEAP) || defined(DEVICE_MANAGER_MIN_HEAP_RESTART)
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
	}

}
