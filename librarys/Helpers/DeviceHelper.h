/*
 * DeviceHelper.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINDIGITAL_DeviceHelper_H_
#define LIBRARIES_PINDIGITAL_DeviceHelper_H_

#include <Arduino.h>
#include "AbstractItem.h"

#include "interfaces/Initializable.h"
#include "interfaces/JSONprocessor.h"
#include "interfaces/JSONprovider.h"
#include "interfaces/Loopable.h"
#include "interfaces/Measurable.h"
#include "interfaces/SendAble.h"


#include "StatusMessage/StatusMessage.h"
#include "TimeTrigger.h"

const PROGMEM char DeviceHelper_NAME[] = "deviceHelper";

class DeviceHelper:public Loopable, public JSONprocessor{

public:
	DeviceHelper(Loopable** loopItems,uint8_t loopItemsSize,
				JSONprocessor** jsonProcessors,uint8_t jsonProcessorsSize,
				JSONprovider** jsonProviders,uint8_t jsonProvidersSize,
				SendAble** senders,uint8_t sendersSize,
				long minAlarmInterval);
	void displayDetails();
	boolean loop();

	void startDevice(String deviceId);
	boolean init(Initializable** initItems,uint8_t initItemsSize);
	void update(AbstractItem** sensors,uint8_t sensorsSize);

	String getProvidersJson(String provider,String page);
	String getProvidersJson();

	StatusMessage processJson(String target,String page,String json);
	String getJson(JSONprovider** sensors,uint8_t size);
	String getJsonAbstractItems(AbstractItem** sensors,uint8_t size);

	void printDeviceDiagnostic();

	void printDeviceDiagnosticNoSpiff();

	String processAlarm(AbstractItem** sensors, uint8_t sensorsSize);

	void createPostponedCommand(String command);
	void prepareTrigger();
	void executePostponedCommand();

	String getName(){
		return FPSTR(DeviceHelper_NAME);
	}

	StatusMessage processJson(String page,String json){
		printProcessParams(page, json);
		return StatusMessage(STATUS_UNKNOWN_INT);
	}
private:
	Loopable** loopItems;
	uint8_t loopItemsSize;

	JSONprocessor** jsonProcessors;
	uint8_t jsonProcessorsSize;

	JSONprovider** jsonProviders;
	uint8_t jsonProvidersSize;

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
