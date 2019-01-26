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
//#include "ESP8266WebServer.h"

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

void DeviceHelper::startDevice(String deviceId){
  Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_STARTED));
  Serial.print(FPSTR(MESSAGE_DEVICE_START_DEVICE_ID));Serial.println(deviceId);
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

StatusMessage DeviceHelper::processJson(String target,String page,String json){
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

String DeviceHelper::getProvidersJson(){
	yield();
	String result="{\"sensors\":[";

		for(uint8_t i=0;i<jsonProvidersSize;i++){
			delay(1);
			result+=jsonProviders[i]->getJson();
			if(i!=jsonProvidersSize-1){
				result+=",";
			}
		}
	result+="]}";

	return result;
}

String DeviceHelper::getAbstrItemsJson(){
	yield();
		String result="{\"sensors\":[";

			for(uint8_t i=0;i<abstrItemsSize;i++){
				delay(1);
				result+=abstrItems[i]->getJson();
				if(i!=abstrItemsSize-1){
					result+=",";
				}
			}
		result+="]}";

		return result;
}

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

StatusMessage DeviceHelper::saveSensorValues(String page,String json){
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
