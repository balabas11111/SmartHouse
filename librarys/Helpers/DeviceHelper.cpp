/*
 * Looper.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */
#include "Arduino.h"
#include "ArduinoJson.h"
#include "EspSettingsBox.h"
#include <DeviceHelper.h>
#include "ESP_Consts.h"
#ifdef ESP32
#include "SPIFFS.h"
#endif
#ifdef ESP8266
#include "FS.h"
#endif
#include <interfaces/Initializable.h>
#include <interfaces/Loopable.h>
#include <interfaces/JSONprocessor.h>
#include <interfaces/JSONprovider.h>
#include <AbstractSensor.h>
#include <interfaces/SendAble.h>
#include <interfaces/SendAbleAbstractSensors.h>
#include <interfaces/EntityService.h>
#include <services/AbstractSensorService.h>


void DeviceHelper::constr(Loopable** loopItems,uint8_t loopItemsSize,
							EntityService** services,uint8_t servicesSize,
							AbstractSensor** sensors,uint8_t sensorsSize,
							SendAble** senders,uint8_t sendersSize,
							EspSettingsBox* espSettingsBox,
							long minAlarmInterval){

	this->loopItems=loopItems;
	this->loopItemsSize=loopItemsSize;

	this->services=services;
	this->servicesSize=servicesSize;

	this->senders=senders;
	this->sendersSize=sendersSize;

	this->espSettingsBox=espSettingsBox;

	this->alarmMode=false;
	this->minAlarmInterval=minAlarmInterval*1000;

	this->lastAlarmTime=0;

	this->triggerInitiated=false;
	postPonedTrigger=nullptr;
}

void DeviceHelper::displayDetails(){
	Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_LOOPERS));

	Serial.print(FPSTR(MESSAGE_DEVICE_HELPER_LOOP_SIZE));
	Serial.println(loopItemsSize);

	for(uint8_t i=0;i<loopItemsSize;i++){
		loopItems[i]->displayDetails();
	}

	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}


boolean DeviceHelper::loop(){
	boolean result=false;

	for(uint8_t i=0;i<loopItemsSize;i++){
		bool currentLoop=loopItems[i]->loop();
		result=result | currentLoop;
	}

	if(triggerInitiated){
		postPonedTrigger->loop();
	}
	#ifdef DISPLAY_LOOPS
		Serial.println("DeviceHelper loop="+String(result));
	#endif

	return result;
}

void DeviceHelper::startDevice(String deviceId,int wifiResetpin){
  Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_STARTED));
  Serial.print(FPSTR(MESSAGE_DEVICE_START_DEVICE_ID));Serial.println(deviceId);

  Serial.println(FPSTR("Device count"));
  Serial.print(FPSTR("Loopable="));
  Serial.println(loopItemsSize);
  Serial.print(FPSTR("EntityService="));
  Serial.println(servicesSize);

  printDeviceDiagnostic();

  espSettingsBox->init();
  espSettingsBox->printSpiffsInfo();

  if(wifiResetpin!=NULL && wifiResetpin>-1){
	  boolean doReset=(digitalRead(wifiResetpin)==HIGH);

	  if(doReset){
		  delay(500);
		  doReset=(digitalRead(wifiResetpin)==HIGH);
	  }

	  if(doReset){
		  espSettingsBox->resetToAp();
	  }else{
		  Serial.println(FPSTR("No reset to AP is required"));
	  }
  }
  Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

boolean DeviceHelper::init(Initializable** initItems,uint8_t initItemsSize){
	Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_INITIALIZING));
	Serial.print(FPSTR(MESSAGE_DEVICE_INIT_STARTED_COUNT));Serial.println(initItemsSize);
	uint8_t initOk=0;

	for(uint8_t i=0;i<initItemsSize;i++){
		Serial.print("init ");
		Serial.println(i);
		Serial.println(ESP.getFreeHeap());
		boolean init=initItems[i]->init();
		if(init){
			initOk++;
		}
	}

	Serial.print(FPSTR(MESSAGE_DEVICE_INIT_COUNT));Serial.println(initOk);
	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
	displayDetails();

	return initOk==initItemsSize;
}

void DeviceHelper::printDeviceDiagnosticNoSpiff(){
	Serial.print(FPSTR(MESSAGE_DEVICE_FREE_HEAP));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getFreeHeap());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	Serial.print(FPSTR(MESSAGE_DEVICE_CPU_MHZ));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getCpuFreqMHz());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	Serial.print(FPSTR(MESSAGE_DEVICE_FLASH_CHIP_SIZE));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getFlashChipSize());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	Serial.print(FPSTR(MESSAGE_DEVICE_FREE_SCETCH_SPACE));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getFreeSketchSpace());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	Serial.println();
}


void DeviceHelper::printDeviceDiagnostic(){
	delay(1);
	Serial.println(FPSTR(MESSAGE_DEVICE_DIAGNOSTIC_BEGIN));
	printDeviceDiagnosticNoSpiff();

#ifdef ESP32
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_spiffsUsed_EQ));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(SPIFFS.usedBytes());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_spiffsTotal_EQ));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(SPIFFS.totalBytes());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
#endif
#ifdef ESP8266
	//Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_spiffsUsed_EQ));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(SPIFFS.);Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	//Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_spiffsTotal_EQ));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(SPIFFS.totalBytes());Serial.print(FPSTR(MESSAGE_DOT_COMMA));
	//Serial.print(FPSTR(MESSAGE_DEVICE_RESET_REASON));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getResetReason());Serial.println(FPSTR(MESSAGE_DOT_COMMA));
#endif
	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}

void DeviceHelper::printDeviceArrayDetails(){
	Serial.println(FPSTR("----------Device Items----------"));

	uint8_t size=ARRAY_SIZE(loopItems);
	Serial.print(FPSTR("Loopable size="));
	Serial.print(loopItemsSize);
	Serial.print(FPSTR(" aSize="));
	Serial.println(size);
	for(uint8_t i=0;i<size;i++){
		Serial.print(VAR_NAME(loopItems[i]));
		Serial.print(FPSTR(" "));
	}
	Serial.println();

	Serial.print(FPSTR("EntityService size="));
	Serial.print(servicesSize);
	Serial.print(FPSTR(" aSize="));
	Serial.println(size);

	Serial.println(FPSTR("--------------------------------"));
}

String DeviceHelper::processAlarm(){
	unsigned long now=millis();

	if(minAlarmInterval==0){
		return "";
	}

	if(alarmMode && (lastAlarmTime+minAlarmInterval>now)){
		return "";
	}

	String alarmMessage="";
	boolean alarmStarted=alarmMode;

	AbstractSensor** sensors=getSensors();

	for(uint8_t i=0;i<getSensorsCount();i++){
		boolean alarm=sensors[i]->checkForAlarm();

		if(alarm){
			if(!alarmStarted){
				alarmStarted=true;
			}
			alarmMode=true;
			alarmMessage+=sensors[i]->generateAlarmText();
		}
	}

	if(alarmMessage!=""){
		if(alarmStarted){
			Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_STARTED));
		}else{
			Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_IDENTIFIED));
		}
		Serial.println(alarmMessage);
		lastAlarmTime=now;
	}else{
		Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_NOT_IDENTIFIED));

		if(alarmMode){
			Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_FINISHED));
			alarmMode=false;
			alarmMessage=FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_FINISHED_RESULT);
			lastAlarmTime=now;
		}
	}
	if(alarmMessage!=""){
		Serial.println(alarmMessage);
	}

	return alarmMessage;
}

void DeviceHelper::createPostponedCommand(String command) {
	prepareTrigger();
	postponedCommand=command;
	postPonedTrigger->start();
	Serial.println(FPSTR("post poned restart trigger started"));
}

void DeviceHelper::prepareTrigger() {
	if(!triggerInitiated){
		postPonedTrigger=new TimeTrigger(0,5000,false,[this](){executePostponedCommand();});
		triggerInitiated=true;
	}
}

void DeviceHelper::executePostponedCommand() {
	if(triggerInitiated){
		postPonedTrigger->stop();
	}

	if(postponedCommand=="restart"){
		Serial.print("Executing restart");
		ESP.restart();
	}
}

String DeviceHelper::processJsonAsEntity(String json) {
		DynamicJsonBuffer jsonBuffer;
		delay(1);
		JsonObject& root = jsonBuffer.parse(json);

		if(root.containsKey("Entity")){
			JsonObject& entity = root["Entity"];
			JsonObject& data = root["data"];

			uint8_t id;
			const char* entityName;

			uint8_t pageId;
			const char* pageName;

			entityName = entity["name"].as<char*>();

			EntityService* service;

			if(entity.containsKey("id")){
				id = entity["id"];
			}

			pageId=entity["pageId"];
			pageName=entity["pageName"].as<char*>();


			uint8_t pageIdRes=0;

			switch (id){
				case Entity_sensors:{
					service = this;
					break;
				}
				case Entity_settings:{
					service = espSettingsBox;
					break;
				}
				default:{
					service = getEntityServiceByName(entityName);
				}
			}

			if(service==NULL || service==nullptr){
				return processEntityServiceError("Not found");
			}
			//process request
			switch (pageId){
				case Page_listVal:
				case Page_list:{
					pageIdRes=pageId;
					break;
				}

				case Page_delete:
				case Page_save:{
					pageIdRes=Page_list;
					service->postAbstractItems(data[entityName], pageId);
					break;
				}
				case Page_saveVal:{
					pageIdRes=Page_listVal;
					service->postAbstractItems(data[entityName], pageId);
					break;
				}
			}
			//generate result
			if(root.containsKey(entityName)){
				root.remove(entityName);
			}

			JsonArray& items=root.createNestedArray(entityName);
			service->getAbstractItems(items, pageIdRes);

			entity["httpStatus"]=200;
		}
		String result;
		root.printTo(result);

		return result;
}

String DeviceHelper::processEntityServiceError(String json) {
	//TODO: construct detailed error JSON here; Better return StatusMessage
	return json;
}

EntityService* DeviceHelper::getEntityServiceByName(String name) {
	for(uint8_t i=0;i<servicesSize;i++){
		/*if(services[i]->getName()==name){
			return services[i];
		}*/
	}
	return NULL;
}
/*
	uint8_t initializableCount=0;
	uint8_t loopableCount=0;
	uint8_t JSONprocessorCount=0;
	uint8_t JSONproviderCount=0;
	uint8_t AbstractSensorCount=0;
	uint8_t SendAbleCount=0;
	uint8_t SendAbleAbstractSensorsCount=0;

	for(uint8_t i=0;i<ARRAY_SIZE(allItems);i++){

		//DeviceLibable* d=allItems[i];
		//dynamic_cast<Initializable&>(d);
		//Initializable& init=dynamic_cast<Initializable&>(d);
		//reinterpret_cast<Initializable&>(d);

		if ( implements( allItems[i], Initializable) ){
			initializableCount++;
		}
		if ( implements( allItems[i], Loopable) ){
			loopableCount++;
		}
		if ( implements( allItems[i], JSONprocessor ) ){
			JSONprocessorCount++;
		}
		if ( implements( allItems[i], JSONprovider ) ){
			JSONproviderCount++;
		}
		if ( implements( allItems[i], AbstractSensor ) ){
			AbstractSensorCount++;
		}
		if ( implements( allItems[i], SendAble ) ){
			SendAbleCount++;
		}
		if ( implements( allItems[i], SendAbleAbstractSensors ) ){
			SendAbleAbstractSensorsCount++;
		}

	}*/

/*
String getJson(JSONprovider** sensors,uint8_t size);
String getJsonAbstractSensors(AbstractSensor** sensors,uint8_t size);

String DeviceHelper::getJson(JSONprovider** providers, uint8_t size) {
	yield();
		String result="{\"sensors\":[";

			for(uint8_t i=0;i<size;i++){
				delay(1);
				result+=providers[i]->getJson();
				if(i!=size-1){
					result+=",";
				}
			}
		result+="]}";

		return result;
}

String DeviceHelper::getJsonAbstractSensors(AbstractSensor** sensors, uint8_t size) {
	yield();
		String result="{\"sensors\":[";

			for(uint8_t i=0;i<size;i++){
				delay(1);
				result+=sensors[i]->getJson();
				if(i!=size-1){
					result+=",";
				}
			}
		result+="]}";

		return result;
}
*/
