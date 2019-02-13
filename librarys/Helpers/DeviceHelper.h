/*
 * DeviceHelper.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINDIGITAL_DeviceHelper_H_
#define LIBRARIES_PINDIGITAL_DeviceHelper_H_

#include <Arduino.h>

#include "AbstractSensor.h"

#include "Consts/PagesConsts.h"
#include "Consts/CommandsConsts.h"

#include "interfaces/Initializable.h"
#include "interfaces/JSONprocessor.h"
#include "interfaces/JSONprovider.h"
#include "interfaces/Loopable.h"
#include "interfaces/SendAble.h"
#include "interfaces/Nameable.h"
#include "interfaces/EntityService.h"

#include "services/AbstractSensorService.h"

#include "StatusMessage/StatusMessage.h"
#include "TimeTrigger.h"

const PROGMEM char DeviceHelper_NAME[] = "deviceHelper";

class DeviceHelper:public Loopable, public AbstractSensorService{

public:
	DeviceHelper(Loopable** loopItems,uint8_t loopItemsSize,
			EntityService** services,uint8_t servicesSize,
			AbstractSensor** sensors,uint8_t sensorsSize,
			SendAble** senders,uint8_t sendersSize,
			EspSettingsBox* espSettingsBox):AbstractSensorService(sensors,sensorsSize){
		constr( loopItems,loopItemsSize,services,servicesSize,sensors, sensorsSize,senders,sendersSize,espSettingsBox);
	}

	void constr (Loopable** loopItems,uint8_t loopItemsSize, EntityService** services,uint8_t servicesSize, AbstractSensor** sensors,uint8_t sensorsSize, SendAble** senders,uint8_t sendersSize, EspSettingsBox* espSettingsBox);
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
	String processEntityServiceError(String json);

	virtual EntityService* getEntityServiceById(uint8_t id);
	virtual EntityService* getEntityServiceByName(String name);

	virtual const char*  getEntityNameById(uint8_t id);
	virtual int getEntityIdByName(const char*  name);

	virtual const char*  getPageNameById(uint8_t id);
	virtual int getPageIdByName(const char*  name);
/*
	StatusMessage processIncomeJson(String target,String page,String json){
		yield();
		if(target==NULL || target.length()==0){
			return StatusMessage(STATUS_INVALID_LENGTH_INT);
		}

		if(checkNamePage(target, page)){
			return processJson(page,json);
		}

		for(uint8_t i=0;i<jsonProcessorsSize;i++){
			if(jsonProcessors[i]->checkNamePage(target,page)){
				return jsonProcessors[i]->processJson(page,json);
			}
		}


		return StatusMessage(STATUS_NOT_FOUND_INT);
	}

	StatusMessage processJson(String page,String json){
		printProcessParams(page, json);

		if(page==FPSTR(PAGE_MANAGE) && json==FPSTR(COMMAND_RESTART)){
			createPostponedCommand(json);

			return StatusMessage(STATUS_OK_ACCEPTED_INT);
		}

		if(page==FPSTR(PAGE_SENSORS)){
			Serial.println(FPSTR("Process sensors"));
			return putAbstractSensorFromJson(json,true,false,true);
		}

		return StatusMessage(STATUS_UNKNOWN_INT);
	}

*/
protected:
	void checkResetPin(int resetPin);

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
