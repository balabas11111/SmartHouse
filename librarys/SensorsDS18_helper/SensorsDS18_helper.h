/*
 * ESP8266_SensorsDS18.h
 *
 *  Created on: 14 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ESP8266_SENSORSDS18_HELPER_H_
#define LIBRARIES_ESP8266_SENSORSDS18_HELPER_H_

#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "KeyValueContainer.h"

class SensorsDS18_helper{
	public:
		SensorsDS18_helper();
		//String sensorToOLEdDisplay_as_JSON(DallasTemperature *sensorz);
		//String sensorToJson(DallasTemperature *sensorz,String date,String time,String deviceId);
		String sensorToHtmlV2(DallasTemperature *sensorz);
		String getSensorAddressByIndexStr(DallasTemperature *sensorz,uint8_t index);
		void initSensor(DallasTemperature* sensorz,String description);
		void printSensorToSerial(uint8_t i,float val);
		void setKeyValueContainer(KeyValueContainer *kvc);
		String getValueByKey(String key);
		KeyValue getKeyValueByKey(String key);
		void putKeyValue(String key,String value);

		String getSettingsHtmlTableBodyV2(/*DallasTemperature* sensorz,*/String sensPreffix,String tSPreffix);
		String sensorToJson(uint8_t id,String busId,String sensorId,float value,String thingSpeakId);
	private:
		KeyValueContainer *_kvc;

};

#endif /* LIBRARIES_ESP8266_SENSORSDS18_ESP8266_SENSORSDS18_H_ */
