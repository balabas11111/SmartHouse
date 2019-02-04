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
#include "StatusMessage/StatusMessage.h"
#include "ESP_Consts.h"
#include "Consts/CommandsConsts.h"
#include "interfaces/DeviceLibable.h"
#include "interfaces/JSONprocessor.h"
#include "interfaces/SendAbleAbstractItems.h"

const PROGMEM char ThingSpeakHelper_NAME[] = "thingSpeakHelper";

class ThingSpeakHelper: public JSONprocessor, public SendAbleAbstractItems, public DeviceLibable {
public:
	ThingSpeakHelper(EspSettingsBox* espSettingsBox,WiFiHelper* wifiHelper){
		this->espSettingsBox=espSettingsBox;
		this->wifiHelper=wifiHelper;
	}
	virtual ~ThingSpeakHelper(){};

	String getName() override{
		return FPSTR(ThingSpeakHelper_NAME);
	}

	StatusMessage sendItems(AbstractItem** items,uint8_t size) override{

		if(espSettingsBox->isThingSpeakEnabled
				&& items!=nullptr
				&& size!=0){
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
				String res=wifiHelper->executeGetRequest(baseUrl+espSettingsBox->thSkWKey+params);
				Serial.print(count);
				if(res!=""){
					return StatusMessage(STATUS_SEND_ERROR_INT,res);
				}else{
					return StatusMessage(STATUS_OK_SENT_INT);
				}
			}

			return StatusMessage(STATUS_OK_NO_TO_SEND_INT);
		}else{
			return StatusMessage(STATUS_OK_DISABLED_INT);
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

	StatusMessage recreateThingSpeakChannelskWithCheckDefault(){
		return recreateThingSpeakChannelskWithCheck(getItems(),getItemsSize());
	}

	StatusMessage recreateThingSpeakChannelskWithCheck(AbstractItem** items,uint8_t size){
		if(size==0){
			return StatusMessage(STATUS_CONF_ERROR_INT,FPSTR(MESSAGE_THINGSPEAK_PUBLISH_NOT_ALLOWED));
		}
		if(!espSettingsBox->isThingSpeakEnabled){
			return StatusMessage(STATUS_CONF_ERROR_INT,FPSTR(MESSAGE_THINGSPEAK_PUBLISH_NOT_ALLOWED));
		}else
		if(espSettingsBox->thSkUsrKey=="" || espSettingsBox->thSkUsrKey==FPSTR(MESSAGE_THINGSPEAK_EMPTY_KEY)){
			return StatusMessage(STATUS_CONF_ERROR_INT,FPSTR(MESSAGE_THINGSPEAK_NO_USER_SPECIFIED));
		}else{
			String message=recreateThingSpeak(items,size);
			return StatusMessage(STATUS_OK_ACCEPTED_INT,message);
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

	StatusMessage processJson(String page,String json){
		printProcessParams(page, json);

		if(page==FPSTR(PAGE_MANAGE) && json==FPSTR(COMMAND_RECREATE_CHANNELS)){
			return recreateThingSpeakChannelskWithCheckDefault();
		}

		return StatusMessage(STATUS_UNKNOWN_INT);
	}

private:
	EspSettingsBox* espSettingsBox;
	WiFiHelper* wifiHelper;
};

#endif /* LIBRARIES_HELPERS_THINGSPEAKHELPER_H_ */
