/*
 * DeviceHelper.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINDIGITAL_DeviceHelper_H_
#define LIBRARIES_PINDIGITAL_DeviceHelper_H_

#include <Arduino.h>
#include <interfaces/EntityServiceBase.h>

#include "Consts/PagesConsts.h"
#include "Consts/CommandsConsts.h"

#include "interfaces/Initializable.h"
#include "interfaces/Loopable.h"
#include "interfaces/SendAble.h"
#include "interfaces/Nameable.h"
#include "services/AbstractSensorService.h"

#include "StatusMessage/StatusMessage.h"
#include "TimeTrigger.h"

const PROGMEM char DeviceHelper_NAME[] = "deviceHelper";

class DeviceHelper:public Loopable, public AbstractSensorService{

public:
	DeviceHelper(Loopable** loopItems,uint8_t loopItemsSize,
			EntityService** services,uint8_t servicesSize,
			AbstractSensorList* sensors,
			SendAble** senders,uint8_t sendersSize,
			EspSettingsBox* espSettingsBox):AbstractSensorService(sensors){
		constr( loopItems,loopItemsSize,services,servicesSize,sensors, senders,sendersSize,espSettingsBox);
	}

	void constr (Loopable** loopItems,uint8_t loopItemsSize, EntityService** services,uint8_t servicesSize, AbstractSensorList* sensors, SendAble** senders,uint8_t sendersSize, EspSettingsBox* espSettingsBox);
	void displayDetails();
	boolean loop();

	void startDevice(String deviceId,int wifiResetPin);
	boolean init(Initializable** initItems,uint8_t initItemsSize);

	static void printHeap(){
		Serial.print(FPSTR(MESSAGE_DEVICE_FREE_HEAP));Serial.println(ESP.getFreeHeap());
	}

	static void printDeviceDiagnostic();
	void printDeviceArrayDetails();

	static void printDeviceDiagnosticNoSpiff();

	String processJsonAsEntity(String json);

	virtual const char*  getEntityNameById(uint8_t id);
	virtual int getEntityIdByName(const char*  name);

	virtual const char*  getPageNameById(uint8_t id);
	virtual int getPageIdByName(const char*  name);
protected:
	void checkResetPin(int resetPin);
	EntityService* getEntityServiceByIdName(uint8_t id,String entityName);

	EntityService* getEntityServiceById(uint8_t id);
	EntityService* getEntityServiceByName(String name);
private:

	EspSettingsBox* espSettingsBox;

	Loopable** loopItems;
	uint8_t loopItemsSize;

	EntityService** services;
	uint8_t servicesSize;

	SendAble** senders;
	uint8_t sendersSize;
};

#endif /* LIBRARIES_PINDIGITAL_DeviceHelper_H_ */
