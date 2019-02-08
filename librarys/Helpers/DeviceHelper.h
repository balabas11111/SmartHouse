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

#include "services/AbstractSensorService.h"

#include "StatusMessage/StatusMessage.h"
#include "TimeTrigger.h"



const PROGMEM char DeviceHelper_NAME[] = "deviceHelper";

class DeviceHelper:public Loopable, public AbstractSensorService, public JSONprocessor{

public:
	DeviceHelper(Loopable** loopItems,uint8_t loopItemsSize,
				JSONprocessor** jsonProcessors,uint8_t jsonProcessorsSize,
				JSONprovider** jsonProviders,uint8_t jsonProvidersSize,
				AbstractSensor** abstrItems,uint8_t abstrItemsSize,
				SendAble** senders,uint8_t sendersSize,
				EspSettingsBox* espSettingsBox,
				long minAlarmInterval);
	void displayDetails();
	boolean loop();

	void startDevice(String deviceId,int wifiResetpin);
	boolean init(Initializable** initItems,uint8_t initItemsSize);

	String getProvidersAndSensorsJson();
	String getProvidersJson();

	static void printHeap(){
		Serial.print(FPSTR(MESSAGE_DEVICE_FREE_HEAP));Serial.println(ESP.getFreeHeap());
	}

	static void printDeviceDiagnostic();
	void printDeviceArrayDetails();

	static void printDeviceDiagnosticNoSpiff();

	String processAlarm();

	void createPostponedCommand(String command);
	void prepareTrigger();
	void executePostponedCommand();

	String getName(){
		return FPSTR(DeviceHelper_NAME);
	}

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

	String getProvidersJson(String provider,String page){
		yield();
		if(provider==NULL || provider.length()==0){
			return StatusMessage(STATUS_INVALID_LENGTH_INT).getJson();
		}

		if(provider==FPSTR(MESSAGE_SERVER_ARG_VAL_ALL)){
			return getProvidersAndSensorsJson();
		}

		if(provider==FPSTR(DeviceHelper_NAME)){
			if(page==FPSTR(PAGE_SENSORS)){
				return getAbstractSensorsAsString();
			}
			if(page==FPSTR(PAGE_PROVIDERS)){
				return getProvidersJson();
			}
		}

		for(uint8_t i=0;i<jsonProvidersSize;i++){
			if(jsonProviders[i]->checkName(provider)){
				return jsonProviders[i]->getJson(page);
			}
		}

		return StatusMessage(STATUS_ITEM_NOT_FOUND_INT).getJson();
	}

protected:
	String getJSONprovidersAsString(JSONprovider** items,uint8_t size);
private:

	EspSettingsBox* espSettingsBox;

	Loopable** loopItems;
	uint8_t loopItemsSize;

	JSONprocessor** jsonProcessors;
	uint8_t jsonProcessorsSize;

	JSONprovider** jsonProviders;
	uint8_t jsonProvidersSize;
/*
	AbstractSensor** abstrItems;
	uint8_t abstrItemsSize;
*/
	SendAble** senders;
	uint8_t sendersSize;

	boolean alarmMode;
	unsigned long minAlarmInterval;
	unsigned long lastAlarmTime;

	boolean triggerInitiated;
	String postponedCommand;
	TimeTrigger* postPonedTrigger;
	//void executePostPonedCommand();
};

#endif /* LIBRARIES_PINDIGITAL_DeviceHelper_H_ */
