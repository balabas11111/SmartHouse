/*
 * ESP8266_SensorsDS18.cpp
 *
 *  Created on: 14 ���. 2018 �.
 *      Author: Vitaliy
 */

#include "SensorsDS18_helper.h"
#include "OneWire.h"
#include "DallasTemperature.h"

SensorsDS18_helper::SensorsDS18_helper(){

}

void SensorsDS18_helper::initSensor(DallasTemperature* sensorz,String description){
	sensorz->begin();
	sensorz->setDescription(description);

	Serial.print("sensors count=");
	Serial.print(sensorz->getDeviceCount());
	Serial.print("; descr=");
	Serial.println(sensorz->getDescription());

	sensorz->requestTemperatures();

	for(uint8_t i=0;i<sensorz->getDeviceCount();i++){
		float val=sensorz->getTempCByIndex(i);
		printSensorToSerial(i,val);
	}
}

void SensorsDS18_helper::printSensorToSerial(uint8_t i,float val){
	Serial.print("sensors i=");
	Serial.print(i);
	Serial.print(" val=");
	Serial.print(val);
	Serial.println(";");
}

String SensorsDS18_helper::sensorToHtmlV2(DallasTemperature *sensorz){
		String html="";

			uint8_t countz=sensorz->getDeviceCount();

			String userData;
			String deviceAddressStr;

		   for(uint8_t j=0;j<countz;j++){
			   deviceAddressStr=getSensorAddressByIndexStr(sensorz,j);
				html+="<label style=\"max-width: 240px;\"><b>";
				html+=getValueByKey(deviceAddressStr);
				html+="</b></label>\
					<input class=\"w3-input w3-border\" style=\"width:95%\" type=\"text\" value=\"";
					html+=sensorz->getTempCByIndex(j);
					html+="\" disabled>";
		   }

			return html;
	}

String SensorsDS18_helper::getSensorAddressByIndexStr(DallasTemperature *sensorz,uint8_t index){
	String result="";

	DeviceAddress deviceAddressD;
	boolean found=sensorz->getAddress(deviceAddressD, index);

	for(uint8_t j=0;j<sizeof(deviceAddressD);j++){
		result+=String(deviceAddressD[j]);
	}

	Serial.print("Get device address index=");
	Serial.print(index);
	Serial.print("; found=");
	Serial.print(found);
	Serial.print("; Address=");
	Serial.print(result);
	Serial.println(";");

	return result;
};

String SensorsDS18_helper::getSettingsHtmlTableBodyV2(/*DallasTemperature* sensorz,*/String sensPreffix,String tSPreffix){
	String html="";

	for(int i=0;i<_kvc->getMapLength();i++){

		String key=_kvc->getKeyValueByIndex(i).getKey();
		/*		DeviceAddress addr=key.c_str();

		char ht=sensorz->getHighAlarmTemp(addr);
		char lt=sensorz->getLowAlarmTemp(addr);
*/
		if(key!=""){
			String keyParam=sensPreffix+key;
			String value=_kvc->getKeyValueByIndex(i).getValue();
			String tsParam=tSPreffix+key;
			String utParam="ut_"+key;
			String ltParam="lt_"+key;
			String tsValue=String(_kvc->getKeyValueByIndex(i).getKind());

			html+="<tr>\
					<td>";
			html+=key;
			html+="</td>";

			if(!(key.startsWith("Pin_")||key.startsWith("pin_"))){
				html+="<td><input type=\"text\" name=\"";
						html+=keyParam;
						html+="\" value=\"";
						html+=value;
						html+="\">\
						</td>\
				<td><input type=\"text\" name=\"";
					html+=tsParam;
					html+="\" value=\"";
					html+=tsValue;
					html+="\"  maxlength=\"2\" size=\"2\">\
					</td></tr>";
			}else{
				html+="<td><b>(Шина данных)</b>\
					<input type=\"text\" name=\"";
						html+=keyParam;
						html+="\" value=\"";
						html+=value;
						html+="\">\
							</td>\
					<td></td></tr>";
			}
		}
	}

	return html;
}

void SensorsDS18_helper::putKeyValue(String key,String value){
	_kvc->putKeyValue(KeyValue(key, value,0));
};

String SensorsDS18_helper::getValueByKey(String key){
	if(_kvc){
		return _kvc->getValue(key);
	}
	return key;
}

KeyValue SensorsDS18_helper::getKeyValueByKey(String key){
	return _kvc->getKeyValue(key);
}

void SensorsDS18_helper::setKeyValueContainer(KeyValueContainer *kvc){
	_kvc=kvc;
}

String SensorsDS18_helper:: sensorToJson(uint8_t id,String busId,String sensorId,float value,String thingSpeakId){

	String result="";
	result+="{\"id\":";
	result+=String(id);
	result+=",\"busId\":";
	result+=busId;
	result+=",\"busName\":";
	result+=getValueByKey(busId);
	result+=",\"sensorId\":";
	result+=sensorId;
	result+=",\"sensorName\":";
	result+=getValueByKey(sensorId);
	result+=",\"thSId\":";
	result+=thingSpeakId;
	result+=",\"value\":";
	result+=String(value);
	result+="}";

	return result;
}








