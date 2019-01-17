/*
 * DeviceHelper.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINDIGITAL_DeviceHelper_H_
#define LIBRARIES_PINDIGITAL_DeviceHelper_H_

#include <Loopable.h>
#include <Initializable.h>
#include <Arduino.h>
#include "Measurable.h"
#include "AbstractItem.h"
#include "JSONprovider.h"
#include "JSONprocessor.h"
#include "StatusMessage.h"
//#include "ESP8266WebServer.h"
#include "TimeTrigger.h"

class DeviceHelper:public Loopable{

public:
	DeviceHelper(Loopable** loopItems,uint8_t loopItemsSize,
				JSONprocessor** jsonProcessors,uint8_t jsonProcessorsSize,
				JSONprovider** jsonProviders,uint8_t jsonProvidersSize,
				long minAlarmInterval);
	void displayDetails();
	boolean loop();

	void startDevice(String deviceId);
	boolean init(Initializable** initItems,uint8_t initItemsSize);
	void update(AbstractItem** sensors,uint8_t sensorsSize);

	String processJson(String target,String page,String json);
	String getProvidersJson(String providerName);
	String getProvidersJson();

	String getJson(JSONprovider** sensors,uint8_t size);
	String getJsonAbstractItems(AbstractItem** sensors,uint8_t size);

	void printDeviceDiagnostic();

	void printDeviceDiagnosticNoSpiff();

	String processAlarm(AbstractItem** sensors, uint8_t sensorsSize);

	void createPostponedCommand(String command);
	void prepareTrigger();
	void executePostponedCommand();

private:
	Loopable** loopItems;
	uint8_t loopItemsSize;

	JSONprocessor** jsonProcessors;
	uint8_t jsonProcessorsSize;

	JSONprovider** jsonProviders;
	uint8_t jsonProvidersSize;

	boolean alarmMode;
	unsigned long minAlarmInterval;
	unsigned long lastAlarmTime;

	boolean triggerInitiated;
	String postponedCommand;
	TimeTrigger* postPonedTrigger;
	//void executePostPonedCommand();
};

#endif /* LIBRARIES_PINDIGITAL_DeviceHelper_H_ */
