/*
 * DeviceManager.h
 *
 *  Created on: Oct 7, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_SERVE_DEVICEMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_SERVE_DEVICEMANAGER_H_

#include "Arduino.h"
#include <ArduinoJson.h>
#include <DeviceUtils.h>
#include "Entity.h"
#include <EntityUpdate.h>

#define DEVICE_MANAGER "DeviceManager"
#define DEVICE_MANAGER_DESCRIPTION "ESP8266 DeviceManager"

//#define DEVICE_MANAGER_PRINT_HEAP 1
//#define DEVICE_MANAGER_MIN_HEAP_RESTART 1000

#ifndef DEVICE_MANAGER_CHECK_INTERVAL
	#if  defined(DEVICE_MANAGER_MIN_HEAP_RESTART) || defined(DEVICE_MANAGER_PRINT_HEAP)
		#define DEVICE_MANAGER_CHECK_INTERVAL 10000
	#else
		#define DEVICE_MANAGER_CHECK_INTERVAL 0
	#endif
#endif

#define DEVICE_MANAGER_FIELD_RESTART "restart"
#define DEVICE_MANAGER_FIELD_FREE_HEAP "freeHeap"


class DeviceManager: public Entity, public EntityUpdate {
public:
	DeviceManager():
		Entity((char*)GROUP_SERVICES, DEVICE_MANAGER, (char*)DEVICE_MANAGER_DESCRIPTION){

	};
	virtual ~DeviceManager(){};

	virtual void loop() override;

	virtual void doGet(JsonObject& params, JsonObject& response) override;

	virtual void doPost(JsonObject& params, JsonObject& response) override;

	virtual void doUpdate() override;

	void triggerRestart(bool doRestart = true);

protected:
	long interval = DEVICE_MANAGER_CHECK_INTERVAL;
private:
	bool triggeredRestart = false;

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_SERVE_DEVICEMANAGER_H_ */
