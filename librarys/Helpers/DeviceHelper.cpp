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
#include <interfaces/SendAble.h>
#include <interfaces/SendAbleAbstractSensors.h>
#include <services/AbstractSensorService.h>


void DeviceHelper::constr(Loopable** loopItems,uint8_t loopItemsSize,
							SendAble** senders,uint8_t sendersSize,
							EspSettingsBox* espSettingsBox){

	this->loopItems=loopItems;
	this->loopItemsSize=loopItemsSize;

	this->servicesSize=servicesSize;

	this->senders=senders;
	this->sendersSize=sendersSize;

	this->espSettingsBox=espSettingsBox;
}

void DeviceHelper::displayDetails(){
	Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_LOOPERS));

	Serial.print(FPSTR(MESSAGE_DEVICE_HELPER_LOOP_SIZE));
	Serial.println(loopItemsSize);

	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
}


boolean DeviceHelper::loop(){
	boolean result=false;

	for(uint8_t i=0;i<loopItemsSize;i++){
		bool currentLoop=loopItems[i]->loop();
		result=result | currentLoop;
	}

	#ifdef DISPLAY_LOOPS
		Serial.println("DeviceHelper loop="+String(result));
	#endif

	return result;
}

void DeviceHelper::startDevice(String deviceId,int wifiResetPin){
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

  checkResetPin(wifiResetPin);

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

/* Entity request Is deployed at /entity HTTP_ANY param=entity
 *
 * //entity?entity={"Entity":{"id":"0","name":"sensors","pageId":"0","pageName":"list"},"data":{}}
 *
 * { "Entity":{
 * 			"id":"0",
 *	 		"name":"sensors",
 * 			"pageId":"0",
 * 			"pageName":"list"
 * 			},
 * 	 "data":{}
 * }
 */

String DeviceHelper::processJsonAsEntity(String json) {
	/*
		long startParse=millis();

		Serial.println(FPSTR("-----------Process Entity----------"));
		//Serial.println(json);
		printHeap();

		long endParse=0;
		long startPost=0;
		long startGet=0;
		long endGet=0;
		long endPrint=0;

		int requestStatus=0;
		String requestText="OK";

		DynamicJsonBuffer jsonBuffer;
		delay(1);
		JsonObject& root = jsonBuffer.parse(json);

		if(root.containsKey("Entity")){
			JsonObject& entity = root["Entity"];
			JsonObject& data = root["data"];

			if(!entity.success()){
				return FPSTR("Entity not parsed");
			}
			int id;
			const char* entityName;

			int pageId;
			const char* pageName;

			entityName = entity["name"].as<char*>();

			if(entity.containsKey("id")){
				id = entity["id"];
			}

			pageId=entity["pageId"];
			pageName=entity["pageName"].as<char*>();

			if(getEntityNameById(id)!=entityName){
				Serial.print(FPSTR("Entity id dismiss income id="));
				Serial.print(id);
				Serial.print(FPSTR(" name="));
				Serial.print(entityName);

				id = getEntityIdByName(entityName);
				Serial.print(FPSTR(" found id="));
				Serial.println(id);

				if(id==-1){
					Serial.println(FPSTR("WRONG Entity id name config"));
				}
			}

			if(getPageNameById(pageId)!=pageName){
				Serial.print(FPSTR("Page id dismiss income id="));
				Serial.print(pageId);
				Serial.print(FPSTR(" name="));
				Serial.print(pageName);

				pageId=getPageIdByName(pageName);

				Serial.print(FPSTR(" found id="));
				Serial.println(pageId);

				if(pageId==-1){
					Serial.println(FPSTR("WRONG page id name config"));
				}
			}

			uint8_t pageIdRes=0;

			Serial.print(FPSTR("entityId="));
			Serial.print(id);
			Serial.print(FPSTR(" entityName="));
			Serial.print(getEntityNameById(id));
			Serial.print(FPSTR(" pageId="));
			Serial.print(pageId);
			Serial.print(FPSTR(" pageName="));
			Serial.println(getPageNameById(id));

			service = getEntityServiceByIdName(id, entityName);

			endParse=millis();

			if(service==NULL || service==nullptr){
				requestStatus = HTTP_CODE_BAD_REQUEST;
				requestText = FPSTR(STATUS_WRONG_SERVICE);
			}else{
				//process request
				Serial.print(FPSTR("Entity="));
				Serial.println(service->getEntityName());

				startPost=millis();
				switch (pageId){
					case Page_listVal:
					case Page_list:{
						pageIdRes=pageId;
						break;
					}

					case Page_delete:
					case Page_save:{
						pageIdRes=Page_list;
						requestStatus = service->postAbstractItems(data[entityName], pageId);
						break;
					}
					case Page_saveVal:{
						pageIdRes=Page_listVal;
						requestStatus = service->postAbstractItems(data[entityName], pageId);
						break;
					}
					default:{
						requestStatus = HTTP_CODE_BAD_REQUEST;
						requestText = FPSTR(STATUS_WRONG_PAGE);
					}
				}
				startGet=millis();

				if(requestStatus<300){
					//generate result
					if(root.containsKey(FPSTR(DEFAULT_RESULT_TAG))){
						root.remove(FPSTR(DEFAULT_RESULT_TAG));
					}

					JsonArray& items=root.createNestedArray(FPSTR(DEFAULT_RESULT_TAG));
					requestStatus = service->getAbstractItems(items, pageIdRes);
				}
			}
			Serial.println(FPSTR("post process request"));
			entity[FPSTR(HTTP_STATUS_TAG)]=requestStatus;
			entity[FPSTR(HTTP_STATUS_DETAILS_TAG)]=requestText;

			endGet=millis();
		}else{
			requestStatus = HTTP_CODE_BAD_REQUEST;
			requestText = FPSTR(STATUS_NO_ENTITY);
		}

		Serial.println(FPSTR("Build result"));
		String result;
		root.printTo(result);
		endPrint=millis();

		Serial.print(FPSTR("Entity ...done"));
		Serial.print(FPSTR(" TotalTime ="));
		Serial.print(millis()-startParse);

		Serial.print(FPSTR(" status ="));
		Serial.print(requestStatus);

		printHeap();
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
*/
		return "";
}



void DeviceHelper::checkResetPin(int resetPin) {
	if(resetPin!=NULL && resetPin>-1){
		  boolean doReset=(digitalRead(resetPin)==HIGH);

		  if(doReset){
			  delay(500);
			  doReset=(digitalRead(resetPin)==HIGH);
		  }

		  if(doReset){
			  espSettingsBox->resetToAp();
		  }else{
			  Serial.println(FPSTR("No reset to AP is required"));
		  }
	  }
}

