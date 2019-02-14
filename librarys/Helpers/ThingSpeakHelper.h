/*
 * ThingSpeakHelper.h
 *
 *  Created on: Nov 20, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HELPERS_THINGSPEAKHELPER_H_
#define LIBRARIES_HELPERS_THINGSPEAKHELPER_H_

#include <AbstractSensorOld.h>
#include "Arduino.h"
#include "EspSettingsBox.h"
#include "WiFiHelper.h"
#include "StatusMessage/StatusMessage.h"
#include "ESP_Consts.h"
#include "Consts/CommandsConsts.h"
#include "interfaces/JSONprocessor.h"
#include "interfaces/SendAbleAbstractSensors.h"

#include "extraBoxes/EspSett_ThSpeak.h"

const PROGMEM char ThingSpeakHelper_NAME[] = "thingSpeakHelper";

class ThingSpeakHelper: public JSONprocessor, public SendAbleAbstractSensors{
public:
	ThingSpeakHelper(EspSettingsBox* espSettingsBox,WiFiHelper* wifiHelper){
		this->espSettingsBox=espSettingsBox;
		this->wifiHelper=wifiHelper;
	}
	virtual ~ThingSpeakHelper(){};

	String getName() override{
		return FPSTR(ThingSpeakHelper_NAME);
	}

	StatusMessage sendItems(AbstractSensor** items,uint8_t size) override{

		if(espSettingsBox->getExtraValueBoolean(ExtraBox_thingSpeak,THINGSPEAK_enabled)
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
				String res=wifiHelper->executeGetRequest(baseUrl+espSettingsBox->getExtraValue(ExtraBox_thingSpeak,THINGSPEAK_writeKey)+params);
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

	void sendAbstractSensorToThingSpeak(AbstractSensor* item){
		String baseUrl=FPSTR(MESSAGE_THINGSPEAK_BASE_URL);
			baseUrl+=espSettingsBox->getExtraValue(ExtraBox_thingSpeak,THINGSPEAK_writeKey);
		String url=item->constructGetUrl(baseUrl, FPSTR(MESSAGE_THINGSPEAK_FIELD_FOR_REQUEST_EQ));
		wifiHelper->executeGetRequest(url);
	}

	String constructThingSpeakParameters(AbstractSensor* item){
		return item->constructGetUrl("", FPSTR(MESSAGE_THINGSPEAK_FIELD_FOR_REQUEST));
	}

	StatusMessage recreateThingSpeakChannelskWithCheckDefault(){
		return recreateThingSpeakChannelskWithCheck(getItems(),getItemsSize());
	}

	StatusMessage recreateThingSpeakChannelskWithCheck(AbstractSensor** items,uint8_t size){
		if(size==0){
			return StatusMessage(STATUS_CONF_ERROR_INT,FPSTR(MESSAGE_THINGSPEAK_PUBLISH_NOT_ALLOWED));
		}
		if(!espSettingsBox->getExtraValueBoolean(ExtraBox_thingSpeak,THINGSPEAK_enabled)){
			return StatusMessage(STATUS_CONF_ERROR_INT,FPSTR(MESSAGE_THINGSPEAK_PUBLISH_NOT_ALLOWED));
		}else
		if(espSettingsBox->getExtraValue(ExtraBox_thingSpeak,THINGSPEAK_userKey)==""
				|| espSettingsBox->getExtraValue(ExtraBox_thingSpeak,THINGSPEAK_userKey)==FPSTR(MESSAGE_THINGSPEAK_EMPTY_KEY)){
			return StatusMessage(STATUS_CONF_ERROR_INT,FPSTR(MESSAGE_THINGSPEAK_NO_USER_SPECIFIED));
		}else{
			String message=recreateThingSpeak(items,size);
			return StatusMessage(STATUS_OK_ACCEPTED_INT,message);
		}

	}

	String recreateThingSpeak(AbstractSensor** items,uint8_t size){
		Serial.println(FPSTR(MESSAGE_THINGSPEAK_CHANNEL_CREATE_STARTED));
		String result="";

		String commandGet=FPSTR(MESSAGE_THINGSPEAK_API_KEY_EQ);
		commandGet+=espSettingsBox->getExtraValue(ExtraBox_thingSpeak,THINGSPEAK_userKey);

		uint8_t countGet=0;
		uint8_t countSet=0;

		for(uint8_t i=0;i<size;i++){
			AbstractSensor* item=items[i];

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

			commandGet+=getDeviceNameFull();
			commandGet+=getDeviceDescriptionFull();

			String getResult=wifiHelper->executeFormPostRequest(FPSTR(MESSAGE_THINGSPEAK_CREATE_CHANNEL_URL),commandGet);
			saveThingSpeakChannelCreation(getResult);
			delay(10);
		}
/*
		if(countSet!=0){
				//---------------------------------------------------------------------------
				for(uint8_t i=0;i<size;i++){
					AbstractSensor* item=items[i];
					boolean doSave=false;

					if(item->getAutoCreateChannel()){
						for(uint8_t j=0;j<item->getItemCount();j++){
							if(item->getFieldId(j)!=0 && item->getSetAllowed(j)){
								item->setQueue(j, getThingSpeakReadChannelName(item->getFieldIdStr(j)));
								doSave=true;
								delay(10);
							}
						}

						if(doSave){
							espSettingsBox->saveAbstractSensorToFile(item);
						}
					}
				}
				//---------------------------------------------------------------------------
		}
*/
		result+=String(countGet);
		result+=FPSTR(MESSAGE_THINGSPEAK_GET_CHANNELS);
		result+=FPSTR(MESSAGE_SPACE);
		result+=String(countSet);
		result+=FPSTR(MESSAGE_THINGSPEAK_SET_CHANNELS);

		return result;
	}

	boolean saveThingSpeakChannelCreation(String response) {

			Serial.println(FPSTR(ESPSETTINGSBOX_THINGSPEAK_PARSE_CHCREATION));
			Serial.print(FPSTR(ESPSETTINGSBOX_THINGSPEAK_CHANNEL_JSON));
			Serial.println(response);

			DynamicJsonBuffer jsonBuffer;

			JsonObject& root = jsonBuffer.parseObject(response);

			if (!root.success()) {
				Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
				return false;
			} else {
				Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_VALUE_PARSED));
				root.printTo(Serial);

				int channelId=root["id"];
				String flag1=root["api_keys"][0]["write_flag"];

				flag1.toLowerCase();
				uint8_t writeKeyFlag=0;
				uint8_t readKeyFlag=1;

				if(flag1!="true"){
					writeKeyFlag=1;
					readKeyFlag=0;
				}
				String writeKey=root["api_keys"][writeKeyFlag]["api_key"];
				String readKey=root["api_keys"][readKeyFlag]["api_key"];

				Serial.print(FPSTR(ESPSETTINGSBOX_THINGSPEAK_CHANNELID));
				Serial.println(channelId);
				Serial.print(FPSTR(ESPSETTINGSBOX_THINGSPEAK_WRITEKEY));
				Serial.println(writeKey);
				Serial.print(FPSTR(ESPSETTINGSBOX_THINGSPEAK_READKEY));
				Serial.println(readKey);

					if(channelId>0 && writeKey!="" && readKey!=""){
							Serial.println(FPSTR(ESPSETTINGSBOX_THINGSPEAK_UPDATE_CHANNEL));
							espSettingsBox->setExtraValue(ExtraBox_thingSpeak, THINGSPEAK_channel,String(channelId));
							espSettingsBox->setExtraValue(ExtraBox_thingSpeak, THINGSPEAK_writeKey,writeKey);
							espSettingsBox->setExtraValue(ExtraBox_thingSpeak, THINGSPEAK_readKey,readKey);
						//mqtt_TStopic="channels/"+String(thSkChId)+"/subscribe/json/"+thSkRKey;

					}
				}

			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

			return true;
	}

	String getThingSpeakReadChannelName(String fieldId){
		String result=FPSTR(MESSAGE_THINGSPEAK_CHANNELS_PREF);
				result+=espSettingsBox->getExtraValue(ExtraBox_thingSpeak, THINGSPEAK_channel);
				result+=FPSTR(MESSAGE_THINGSPEAK_SUBSCRIBE_FIELDS_FIELD);
				result+=fieldId;
				result+=FPSTR(MESSAGE_DIVIDE);
				result+=espSettingsBox->getExtraValue(ExtraBox_thingSpeak, THINGSPEAK_readKey);
		return  result;
	}

	String getDeviceNameFull(){
		uint8_t boxInd=espSettingsBox->getExtraBoxIndex(FPSTR(DEVICE_SETTINGS_BOX_NAME));

		String result=FPSTR(MESSAGE_THINGSPEAK_NAME_FOR_REQUEST_EQ);
		result+=espSettingsBox->getExtraValue(boxInd, DEVICE_location);
		result+=FPSTR(MESSAGE_SPACE);
		result+=espSettingsBox->getExtraValue(boxInd, DEVICE_id);

		return result;
	}

	String getDeviceDescriptionFull(){
		uint8_t boxInd=espSettingsBox->getExtraBoxIndex(FPSTR(DEVICE_SETTINGS_BOX_NAME));

		String result= FPSTR(MESSAGE_THINGSPEAK_DESCRIPTION_FOR_REQUEST_EQ);
				result+=espSettingsBox->getExtraValue(boxInd, DEVICE_descr);
				result+=FPSTR(MESSAGE_SPACE);
				result+=DEVICE_KIND;
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
	AbstractSensor** sensors;
};

#endif /* LIBRARIES_HELPERS_THINGSPEAKHELPER_H_ */
