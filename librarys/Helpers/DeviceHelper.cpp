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
#include <AbstractItem.h>
#include <interfaces/SendAble.h>
#include <interfaces/SendAbleAbstractItems.h>


DeviceHelper::DeviceHelper(Loopable** loopItems,uint8_t loopItemsSize,
							JSONprocessor** jsonProcessors,uint8_t jsonProcessorsSize,
							JSONprovider** jsonProviders,uint8_t jsonProvidersSize,
							AbstractItem** abstrItems,uint8_t abstrItemsSize,
							SendAble** senders,uint8_t sendersSize,
							EspSettingsBox* espSettingsBox,
							long minAlarmInterval){

	this->loopItems=loopItems;
	this->loopItemsSize=loopItemsSize;

	this->jsonProcessors=jsonProcessors;
	this->jsonProcessorsSize=jsonProcessorsSize;

	this->jsonProviders=jsonProviders;
	this->jsonProvidersSize=jsonProvidersSize;

	this->abstrItems=abstrItems;
	this->abstrItemsSize=abstrItemsSize;

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
  Serial.println(ARRAY_SIZE(loopItems));
  Serial.print(FPSTR("AbstractItems="));
  Serial.println(ARRAY_SIZE(abstrItems));
  Serial.print(FPSTR("JSONprovider="));
Serial.println(ARRAY_SIZE(jsonProviders));
Serial.print(FPSTR("JSONprovider="));
Serial.println(ARRAY_SIZE(jsonProviders));

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

	size=ARRAY_SIZE(jsonProcessors);
	Serial.print(FPSTR("jsonProcessors size="));
	Serial.print(jsonProcessorsSize);
	Serial.print(FPSTR(" aSize="));
	Serial.println(size);
	for(uint8_t i=0;i<size;i++){
		Serial.print(VAR_NAME(jsonProcessors[i]));
		Serial.print(FPSTR(" "));
	}
	Serial.println();

	size=ARRAY_SIZE(jsonProviders);
	Serial.print(FPSTR("jsonProviders size="));
	Serial.print(jsonProvidersSize);
	Serial.print(FPSTR(" aSize="));
	Serial.println(size);
	for(uint8_t i=0;i<size;i++){
		Serial.print(VAR_NAME(jsonProviders[i]));
		Serial.print(FPSTR(" "));
	}
	Serial.println();

	size=ARRAY_SIZE(abstrItems);
	Serial.print(FPSTR("abstrItems size="));
	Serial.print(abstrItemsSize);
	Serial.print(FPSTR(" aSize="));
	Serial.println(size);
	for(uint8_t i=0;i<size;i++){
		Serial.print(VAR_NAME(abstrItems[i]));
		Serial.print(FPSTR(" "));
	}
	Serial.println();

	size=ARRAY_SIZE(senders);
	Serial.print(FPSTR("senders size="));
	Serial.print(sendersSize);
	Serial.print(FPSTR(" aSize="));
	Serial.println(size);
	for(uint8_t i=0;i<size;i++){
		Serial.print(VAR_NAME(senders[i]));
		Serial.print(FPSTR(" "));
	}
	Serial.println();

	Serial.println(FPSTR("--------------------------------"));
}

void DeviceHelper::update(){
	Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_UPDATE_EXECUTION));

	for(uint8_t i=0;i<abstrItemsSize;i++){
		abstrItems[i]->update();
		abstrItems[i]->checkForAlarm();
		Serial.println(abstrItems[i]->getJson());
	}

	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
	printDeviceDiagnostic();
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

	for(uint8_t i=0;i<abstrItemsSize;i++){
		boolean alarm=abstrItems[i]->checkForAlarm();

		if(alarm){
			if(!alarmStarted){
				alarmStarted=true;
			}
			alarmMode=true;
			alarmMessage+=abstrItems[i]->generateAlarmText();
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

String DeviceHelper::getProvidersAndSensorsJson(){
	String result="{\"sensors\":[";

	result+=getAbstractItemsAsString(abstrItems,abstrItemsSize);
	result+=getJSONprovidersAsString(jsonProviders,jsonProvidersSize);
	result.setCharAt(result.length(), ']');
	result+="}";

	return result;
}

String DeviceHelper::getProvidersJson(){
	String result="{\"sensors\":[";

	result+=getJSONprovidersAsString(jsonProviders,jsonProvidersSize);
	result.setCharAt(result.length(), ']');
	result+="}";

	return result;
}

String DeviceHelper::getAbstrItemsJson(){
	String result="{\"sensors\":[";

	result+=getAbstractItemsAsString(abstrItems,abstrItemsSize);
	result.setCharAt(result.length(), ']');
	result+="}";

	return result;
}

String DeviceHelper::getAbstractItemsAsString(AbstractItem** items,uint8_t itemsSize){
	yield();
	String result="";
	uint8_t size=ARRAY_SIZE(items);

	for(uint8_t i=0;i<size;i++){
		result+=items[i]->getJson();
		result+=",";
	}

	return result;
}

String DeviceHelper::getJSONprovidersAsString(JSONprovider** items,uint8_t itemsSize){
	yield();
	String result="";
	uint8_t size=ARRAY_SIZE(items);

	for(uint8_t i=0;i<size;i++){
		result+=items[i]->getJson();
		result+=",";
	}

	return result;
}

StatusMessage DeviceHelper::saveSensorSettings(String page,String json){
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(json);

	if(!root.success()){
		Serial.println(FPSTR("Error parse JSOn"));
		return StatusMessage(STATUS_PARSE_ERROR_INT);
	}

	uint8_t id=root["id"];
	String name=root["name"];
	String descr=root["descr"];
	uint8_t itemCount=root["itemCount"];

	AbstractItem* aitem=NULL;

	Serial.println("id="+String(id)+" name="+name+" descr="+descr+" itemCount="+String(itemCount));

	for(uint8_t i=0;i<abstrItemsSize;i++){
		if(abstrItems[i]->checkName(name)){
			aitem=abstrItems[i];
			break;
		}
	}

	if(aitem==NULL){
		return StatusMessage(STATUS_NOT_FOUND_INT);
	}

	aitem->setDescr(descr);

	for(uint8_t i=0;i<itemCount;i++){
		uint8_t idItem=root["items"][i]["id"];
		String nameItem=root["items"][i]["name"];
		String descrItem=root["items"][i]["descr"];
		String queue=root["items"][i]["queue"];
		float minVal=root["items"][i]["minVal"];
		float maxVal=root["items"][i]["maxVal"];
		uint8_t fieldId=root["items"][i]["fieldId"];

		if(aitem->getName(idItem)==nameItem){
			aitem->setDescr(idItem, descrItem);
			aitem->setQueue(idItem, queue);
			aitem->setMinVal(idItem, minVal);
			aitem->setMaxVal(idItem, maxVal);
			aitem->setFieldId(id, fieldId);
		}
		Serial.println("idItem="+String(idItem)+" name="+nameItem+" descr="+descrItem+" queue="+queue+" minVal="+String(minVal)+" maxVal="+String(maxVal)+" fieldId="+String(fieldId));
	}

	espSettingsBox->saveAbstractItemToFile(aitem);

	return StatusMessage(STATUS_OK_SAVED_INT);
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

/*
	uint8_t initializableCount=0;
	uint8_t loopableCount=0;
	uint8_t JSONprocessorCount=0;
	uint8_t JSONproviderCount=0;
	uint8_t AbstractItemCount=0;
	uint8_t SendAbleCount=0;
	uint8_t SendAbleAbstractItemsCount=0;

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
		if ( implements( allItems[i], AbstractItem ) ){
			AbstractItemCount++;
		}
		if ( implements( allItems[i], SendAble ) ){
			SendAbleCount++;
		}
		if ( implements( allItems[i], SendAbleAbstractItems ) ){
			SendAbleAbstractItemsCount++;
		}

	}*/

/*
String getJson(JSONprovider** sensors,uint8_t size);
String getJsonAbstractItems(AbstractItem** sensors,uint8_t size);

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

String DeviceHelper::getJsonAbstractItems(AbstractItem** sensors, uint8_t size) {
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
