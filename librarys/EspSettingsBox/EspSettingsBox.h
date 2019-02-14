/*
 * EspSettingsBox.h
 *
 *  Created on: 2 Ñ„ÐµÐ²Ñ€. 2018 Ð³.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_
#define LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_

#include "Arduino.h"
#include "FS.h"
#include "ArduinoJson.h"
#include "IPAddress.h"

#include "ESP_Consts.h"
#include "ESPExtraSettingsBox.h"
#include "projectConsts.h"
#include "Consts/PagesConsts.h"
#include "Consts/CommandsConsts.h"

#include "interfaces/Initializable.h"
#include "interfaces/EntityService.h"

#include "services/AbstractSensorService.h"
#include "extraBoxes/EspSett_Own.h"

#include "ExtraSettingsBoxService.h"

const PROGMEM char EspSettingsBox_NAME[] = "espSettingsBox";

class EspSettingsBox: public Initializable,
	public ExtraSettingsBoxService, public EntityService {

public:
	EspSettingsBox(){
		construct(false,false);
	}

	EspSettingsBox(ESPExtraSettingsBox** extraBoxes,uint8_t boxCount)
				:ExtraSettingsBoxService(extraBoxes,boxCount){
			construct(false,false);
	}

	EspSettingsBox(boolean forceLoad,boolean _initSpiff){
			construct(forceLoad,_initSpiff);
	}

	virtual boolean initialize(boolean _init) override;

	boolean setSettingsValue(String fieldName,String fieldValue);

	boolean getSaveRequired(){
		boolean result=false;

		for(uint8_t i=0;i<getExtraBoxesCount();i++){
			result=getExtraBox(i)->getSaveRequired() || result;
		}
		return result;
	}

	String getName(){
		return FPSTR(EspSettingsBox_NAME);
	}

	uint8_t getEntityId(){
		return Entity_settings;
	}

	virtual int getAbstractItems(JsonArray& items,uint8_t pageId);
	virtual int postAbstractItems(JsonArray& items,uint8_t pageId);

	StatusMessage processJson(String page,String json){
		printProcessParams(page, json);

		if(page==FPSTR(PAGE_MANAGE) && json==FPSTR(COMMAND_DELETE_SETTINGS)){
			int count=deleteSettingsFiles();

			return StatusMessage(STATUS_OK_DELETED_INT,String(count));
		}

		if(page==FPSTR(PAGE_DEVICE) || page==FPSTR(PAGE_NET) || page==FPSTR(PAGE_PUBLISH)){
			beginSetSettingsValue(page);

			DynamicJsonBuffer jsonBuffer;
			JsonObject& root = jsonBuffer.parseObject(json);

			if(!root.success()){
				Serial.println(FPSTR("Error parse JSOn"));
				return StatusMessage(STATUS_PARSE_ERROR_INT);
			}

			JsonObject::iterator it;
			  for (it = root.begin(); it != root.end(); ++it ){
				  String argName=it->key;
				  String argVal=it->value.asString();

				  if(!setSettingsValue(argName,argVal)){
				  			  				Serial.print(FPSTR("FAILED find target for argName="));
				  			  				Serial.print(argName);
				  			  				Serial.print(FPSTR(" argVal="));
				  			  				Serial.println(argVal);
  	  			  }
			  }

			finishSetSettingsValue(page);

			return StatusMessage(STATUS_OK_SAVED_INT,page,getJson(page));
		}

		return StatusMessage(STATUS_NOT_FOUND_INT);
	}

	void beginSetSettingsValue(String page){
		Serial.print(FPSTR("--- page="));
		Serial.print(page);
		Serial.println(FPSTR(" save Started"));

		beginSetExtraSettingsValue(page);
	}

	void finishSetSettingsValue(String page){

		boolean ebsr=finishSetExtraSettingsValue();

		Serial.print(FPSTR("--- page="));
		Serial.print(page);
		Serial.print(FPSTR(" save finished"));
	}

	String getJson();
	String getJson(String page);

	void resetToAp(){
		Serial.println(FPSTR("Device will be started as AP"));
		setExtraValue(ExtraBox_Own, OWN_isAccesPoint, "true");
		setExtraValue(ExtraBox_Own, OWN_password, "password");
		saveExtraBox(ExtraBox_Own);
		Serial.println(FPSTR("---Saved as AP---"));
	}

	void initSpiff();
	boolean isSpiffInitialized();

	void printSpiffsInfo();

	int deleteWebFiles();
	int deleteSettingsFiles();

	int deleteFilesByPreffix(String preffix);

	//boolean saveThingSpeakChannelCreation(String response);
	String getThingSpeakChannelUrl();

//--------------------device settings kind (page)-------------------
private:
	boolean loaded=false;

	boolean spiffInitialized;
	String _fileName;

	void construct(boolean forceLoad,boolean initSpiff);
	void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

	size_t bufferLength=2048;
};




#endif /* LIBRARIES_TIMETRIGGER_ESPSETTINGSBOX_H_ */
