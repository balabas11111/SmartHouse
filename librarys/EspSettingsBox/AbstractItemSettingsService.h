/*
 * AbstractItemSettingsService.h
 *
 *  Created on: 3 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESPSETTINGSBOX_ABSTRACTITEMSETTINGSSERVICE_H_
#define LIBRARIES_ESPSETTINGSBOX_ABSTRACTITEMSETTINGSSERVICE_H_

#include "Arduino.h"
#include "AbstractItem.h"
#include "ESP_Consts.h"
#include "ArduinoJson.h"

class AbstractItemSettingsService {
public:
	AbstractItemSettingsService(){};
	virtual ~AbstractItemSettingsService(){};

	void loadSensorsFromFile(AbstractItem** items){
		uint8_t size=ARRAY_SIZE(items);

		for(uint8_t i=0;i<size;i++){
			loadAbstractItemFromFile(items[i]);
			saveDefaultItemFieldProviderIfMissing(items[i]);
		}
	}

	virtual void saveDefaultItemFieldProviderIfMissing(AbstractItem* abstrItem){
		if(!abstrItem->hasItemFields()){
			return;
		}

		for(uint8_t j=0;j<abstrItem->getItemFieldsProvidersCount();j++){

			for(uint8_t i=0;i<abstrItem->getItemCount();i++){
				saveDefaultItemFieldProviderValuesIfMissing(j,abstrItem->getName(),abstrItem->getName(i));
			}
		}
	}

	virtual void saveDefaultItemFieldProviderValuesIfMissing(uint8_t pId,String aName,String sName)=0;

	void saveSensorsToFile(AbstractItem** items,uint8_t size){
		for(uint8_t i=0;i<size;i++){
			saveAbstractItemToFile(items[i]);
		}
	}

	void loadAbstractItemFromFile(AbstractItem* item){

		String fileName=EspSettingsUtil::getSettingsFileFileName(item->getName());

		File file = SPIFFS.open(fileName, "r");

		if(!file){
			Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_DEFAULT_VALUES_SAVED));
			saveAbstractItemToFile(item);
			file = SPIFFS.open(fileName, "r");
		}

		  if (!file){
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_FILE_NOT_EXISTS));
		  } else {
			size_t size = file.size();
			if ( size == 0 ) {
			  Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_FILE_EMPTY));
			} else {

				StaticJsonBuffer<1024> jsonBuffer;
				delay(1);

				std::unique_ptr<char[]> buf (new char[size]);
				file.readBytes(buf.get(), size);

				JsonObject& root = jsonBuffer.parseObject(buf.get());

				if (!root.success()) {
					Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
				} else {
					//Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_VALUE_PARSED));
					//root.printTo(Serial);

					item->setDescr(root["descr"].as<char*>());

					uint8_t itemCountJson=root["itemCount"];

					if(item->getItemCount()>0 && itemCountJson>0){
						//JsonArray& arrayJson=root["items"].asArray();
						//Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_TOTAL_CHILDS_EQ));
						//Serial.println(itemCountJson);

						for(uint8_t i=0;i<itemCountJson;i++){

							int8_t index=item->getChildItemIndexByName(root["items"][i]["name"]);

							if(index>-1){
								item->setDescr(index, root["items"][i]["descr"]);
								item->setFieldId(index, root["items"][i]["fieldId"]);
								item->setMinVal(index, root["items"][i]["minVal"]);
								item->setMaxVal(index, root["items"][i]["maxVal"]);
								item->setQueue(index, root["items"][i]["queue"]);
							}else{
								item->setNonActiveSensorValue(
										root["items"][i]["name"],
										root["items"][i]["descr"],
										root["items"][i]["fieldId"],
										root["items"][i]["minVal"],
										root["items"][i]["maxVal"],
										root["items"][i]["queue"]);
							}

							//Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_ITEM_SETTINGS_LOADED));
							//Serial.println(item->getJson(index));
						}
					}
					//-----------------------------------------
				}
			}
		  }

		  //Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
	}

	void saveAbstractItemToFile(AbstractItem* item){

		String fileName=EspSettingsUtil::getSettingsFileFileName(item->getName());

		Serial.print(FPSTR(MESSAGE_ESPSETTINGSBOX_ABSTRACT_ITEM_SAVE_BEGIN));
		Serial.print(fileName);

		File file = SPIFFS.open(fileName, "w");

		file.println(item->getJsonForSave());

		file.close();
		Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_SAVED));

		delay(1);
	}
private:

};

#endif /* LIBRARIES_ESPSETTINGSBOX_ABSTRACTITEMSETTINGSSERVICE_H_ */
