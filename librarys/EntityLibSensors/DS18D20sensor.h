/*
 * DS18D20sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_DS18D20SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_DS18D20SENSOR_H_

#include <ArduinoJson.h>
#include <EntityJson.h>
#include "UpdateAble.h"

#include <OneWire.h>
#include "DallasTemperature.h"

#define DS18D20sensorDescriptor "{\"data\": {\"temp\":\"-1\",\"hum\":\"-1\",\"press\":\"-1\"},\
\"model\":{\"var\":[\"temp\",\"hum\",\"press\"],\"tvar\":[\"temp\",\"hum\"]}  }"

class DS18D20sensor: public EntityJson, public UpdateAble {
public:
	DS18D20sensor(int pin):
		EntityJson(ROOT_GROUP_SENSORS,"bme280","Temperature/Humidity/AtmPressure",DS18D20sensorDescriptor){
		oneWire=new OneWire(pin);
		dallasTemperature=new DallasTemperature(oneWire);
	};

	virtual ~DS18D20sensor(){};

	virtual void init() override{ }

	virtual void postModelDataInit() override{
		//dallasTemperature->begin();
		cleanUnnededFields();
		dallasTemperature->begin();
		itemCount=dallasTemperature->getDeviceCount();

		this->getModelDataProvider()->setField(id, JSONKEY_itemCount,itemCount);

		for(uint8_t i=0;i<itemCount;i++){
			String devAddressStr=getDeviceAddress(i);
			const char* descr = getDescrFromDict(devAddressStr);

			putSensorToData(devAddressStr.c_str(),descr,127);
		}
		readTemperatures();
		Serial.print(FPSTR("DS18D20 itemCount = "));
		Serial.println(itemCount);
	}

	virtual void update() override{
		readTemperatures();
		sendAsEventSourceEntity();
	}

protected:
	int itemCount=0;
	OneWire* oneWire;
	DallasTemperature* dallasTemperature;

	void cleanUnnededFields(){
		if(modelDataProvider->getEntityData(id).containsKey(JSONKEY_items)){
			modelDataProvider->getEntityData(id).remove(JSONKEY_items);
		}
	}

	void putSensorToData(const char* sensorUid,const char* descr,float value){
		JsonObject& item =
		JsonObjectUtil::getObjectChildOrCreateNew(modelDataProvider->getEntityData(id), JSONKEY_items)
			.createNestedObject(sensorUid);

		item.set(JSONKEY_descr, descr);
		item.set(JSONKEY_temp, value);
	}
	void setSensorDataValue(const char* sensorUid,float value){
		JsonObjectUtil::getObjectChildOrCreateNew(modelDataProvider->getEntityData(id), JSONKEY_items)
			.get<JsonObject>(sensorUid)
			.set(JSONKEY_temp, value);
	}
	void setSensorDataValueDescr(const char* sensorUid,const char* descr){
		JsonObjectUtil::getObjectChildOrCreateNew(modelDataProvider->getEntityData(id), JSONKEY_items)
			.get<JsonObject>(sensorUid)
			.set(JSONKEY_descr, descr);
	}

	const char* getDescrFromDict(String& devAddressStr){
		return devAddressStr.c_str();
	}
	void putDescrFromDict(String& devAddressStr,const char* val){

	}

	virtual void readTemperatures(){
		dallasTemperature->requestTemperatures();
		for(uint8_t i=0;i<itemCount;i++){
			String devAddressStr=getDeviceAddress(i);
			float t = dallasTemperature->getTempCByIndex(i);
			setSensorDataValue(devAddressStr.c_str(),t);
		}
	}

	String getDeviceAddress(uint8_t index){
			DeviceAddress deviceAddress;
			dallasTemperature->getAddress(deviceAddress,index);
			uint8_t size=sizeof(deviceAddress);

			String addrStr=deviceAddressToString(deviceAddress,size);

			Serial.print("index=");
			Serial.print(index);
			Serial.print(" address=");
			for(uint8_t k=0;k<size;k++){
				Serial.print(deviceAddress[k]);
			}
			Serial.print(" addrStr=");
			Serial.println(addrStr);

			return addrStr;
		}

		String deviceAddressToString(DeviceAddress deviceAddress,uint8_t size){
			String devAddrStr="";

			for(uint8_t k=0;k<size;k++){
				devAddrStr+=String(deviceAddress[k]);
			}

			return devAddrStr;
		}
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_DS18D20sensor_H_ */
