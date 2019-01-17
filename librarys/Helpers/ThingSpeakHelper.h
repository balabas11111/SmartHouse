/*
 * ThingSpeakHelper.h
 *
 *  Created on: Nov 20, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HELPERS_THINGSPEAKHELPER_H_
#define LIBRARIES_HELPERS_THINGSPEAKHELPER_H_

#include "Arduino.h"
#include "EspSettingsBox.h"
#include "WiFiHelper.h"
#include "AbstractItem.h"
#include "StatusMessage.h"
#include "ESP_Consts.h"

class ThingSpeakHelper {
public:
	ThingSpeakHelper(EspSettingsBox* espSettingsBox,WiFiHelper* wifiHelper){
		this->espSettingsBox=espSettingsBox;
		this->wifiHelper=wifiHelper;
	}
	virtual ~ThingSpeakHelper(){};

	void sendItemsToThingSpeak(AbstractItem** items,uint8_t size){
		if(espSettingsBox->isThingSpeakEnabled){
			Serial.println(FPSTR(MESSAGE_THINGSPEAK_SEND_STARTED));

			uint8_t count=0;

			String baseUrl=FPSTR(MESSAGE_THINGSPEAK_BASE_URL);
			String params="";

			for(uint8_t i=0;i<size;i++){
				params+=constructThingSpeakParameters(items[i]);
				if(params!=""){
					count++;
				}
			}

			if(params!=""){
				wifiHelper->executeGetRequest(baseUrl+espSettingsBox->thSkWKey+params);
				Serial.print(count);
				Serial.println(FPSTR(MESSAGE_DONE));
				Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
			}
		}
	}

	void sendAbstractItemToThingSpeak(AbstractItem* item){
		String baseUrl=FPSTR(MESSAGE_THINGSPEAK_BASE_URL);
			baseUrl+=espSettingsBox->thSkWKey;
		String url=item->constructGetUrl(baseUrl, FPSTR(MESSAGE_THINGSPEAK_FIELD_FOR_REQUEST_EQ));
		wifiHelper->executeGetRequest(url);
	}

	String constructThingSpeakParameters(AbstractItem* item){
		return item->constructGetUrl("", FPSTR(MESSAGE_THINGSPEAK_FIELD_FOR_REQUEST));
	}

	StatusMessage recreateThingSpeaChannelskWithCheck(AbstractItem** items,uint8_t size){
		if(!espSettingsBox->isThingSpeakEnabled){
			return StatusMessage(STATUS_FAILED_INT,FPSTR(MESSAGE_THINGSPEAK_PUBLISH_NOT_ALLOWED));
		}else
		if(espSettingsBox->thSkUsrKey=="" || espSettingsBox->thSkUsrKey==FPSTR(MESSAGE_THINGSPEAK_EMPTY_KEY)){
			return StatusMessage(STATUS_FAILED_INT,FPSTR(MESSAGE_THINGSPEAK_NO_USER_SPECIFIED));
		}else{
			String message=recreateThingSpeak(items,size);
			return StatusMessage(STATUS_FAILED_INT,message);
		}

	}

	String recreateThingSpeak(AbstractItem** items,uint8_t size){
		Serial.println(FPSTR(MESSAGE_THINGSPEAK_CHANNEL_CREATE_STARTED));
		String result="";

		String commandGet=FPSTR(MESSAGE_THINGSPEAK_API_KEY_EQ);
		commandGet+=espSettingsBox->thSkUsrKey;

		uint8_t countGet=0;
		uint8_t countSet=0;

		for(uint8_t i=0;i<size;i++){
			AbstractItem* item=items[i];

			if(item->getAutoCreateChannel()){
				for(uint8_t j=0;j<item->getItemCount();j++){
					if(item->getFieldId(j)!=0){

						countGet++;
						commandGet+=FPSTR(MESSAGE_THINGSPEAK_FIELD_FOR_REQUEST);
						commandGet+=item->getFieldIdStr(j);
						commandGet+=FPSTR(MESSAGE_EQUALS);
						commandGet+=item->getDescr(j);

						if(item->getSetAllowed(j)){
							countSet++;
						}
					}
				}
			}
		}

		if(countGet!=0){

			commandGet+=espSettingsBox->getDeviceNameFull();
			commandGet+=espSettingsBox->getDeviceDescriptionFull();

			String getResult=wifiHelper->executeFormPostRequest(FPSTR(MESSAGE_THINGSPEAK_CREATE_CHANNEL_URL),commandGet);
			espSettingsBox->saveThingSpeakChannelCreation(getResult);
			delay(10);
		}

		if(countSet!=0){
				//---------------------------------------------------------------------------
				for(uint8_t i=0;i<size;i++){
					AbstractItem* item=items[i];
					boolean doSave=false;

					if(item->getAutoCreateChannel()){
						for(uint8_t j=0;j<item->getItemCount();j++){
							if(item->getFieldId(j)!=0 && item->getSetAllowed(j)){
								item->setQueue(j, espSettingsBox->getThingSpeakReadChannelName(item->getFieldIdStr(j)));
								doSave=true;
								delay(10);
							}
						}

						if(doSave){
							espSettingsBox->saveAbstractItemToFile(item);
						}
					}
				}
				//---------------------------------------------------------------------------
		}

		result+=String(countGet);
		result+=FPSTR(MESSAGE_THINGSPEAK_GET_CHANNELS);
		result+=FPSTR(MESSAGE_SPACE);
		result+=String(countSet);
		result+=FPSTR(MESSAGE_THINGSPEAK_SET_CHANNELS);

		return result;
	}

private:
	EspSettingsBox* espSettingsBox;
	WiFiHelper* wifiHelper;
};

#endif /* LIBRARIES_HELPERS_THINGSPEAKHELPER_H_ */
