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
#include "ObjectUtils.h"

#include <OneWireMock.h>
#include "DallasTemperatureMock.h"

#define DS18D20sensorDescriptor "{\"data\": {},\
\"model\":{\"var\":[]}  }"

class DS18D20sensor: public EntityJson, public UpdateAble {
public:
	DS18D20sensor(int pin):
		EntityJson(ROOT_GROUP_SENSORS,"ds18d20","Temperature DallasTemperature",DS18D20sensorDescriptor){
		construct(pin);
	};

	virtual ~DS18D20sensor(){};

	virtual void init() override{ }

	virtual void postModelDataInit() override{
		cleanUnnededFields();
		dallasTemperature->begin();
		itemCount=dallasTemperature->getDeviceCount();

		this->getModelDataProvider()->setField(id, JSONKEY_itemCount,itemCount);

		for(uint8_t i=0;i<itemCount;i++){
			initSensorModel(getDeviceAddress(i).c_str(),127);
		}
		readTemperatures();
		print();
	}

	virtual void update() override{
		readTemperatures();
		sendAsEventSourceEntity();
	}
	virtual bool processFieldPreSave(const char* key,const char* value) override{
		String keyStr=key;

		if(keyStr.endsWith(JSONKEY_descr)){
			keyStr=keyStr.substring(0, keyStr.length()-6);
			Serial.print(FPSTR("Sensor UID = "));Serial.print(keyStr);
			Serial.print(FPSTR(" key="));Serial.println(key);
			setSensorDescr(keyStr.c_str(),value);
			return false;
		}

		return true;
	}

	float getSensorValue(const char* sensorUid){
		return JsonObjectUtil::getObjectChildOrCreateNew(modelDataProvider->getEntityData(id), JSONKEY_items)
				.get<JsonObject>(sensorUid).get<float>(JSONKEY_temp);
	}
	const char* getSensorDescr(const char* sensorUid){
		return JsonObjectUtil::getObjectChildOrCreateNew(modelDataProvider->getEntityData(id), JSONKEY_items)
				.get<JsonObject>(sensorUid).get<const char*>(JSONKEY_descr);
	}

	void print(){
		Serial.println(FPSTR("DS18D20 Entity"));
		Serial.print(FPSTR(" name = "));
		Serial.print(name);
		Serial.print(FPSTR(" itemCount = "));
		Serial.println(itemCount);
		Serial.println(FPSTR("-----"));
		for(uint8_t i=0;i<itemCount;i++){
			Serial.print("i=");
			Serial.print(i);
			Serial.print(" address=");
			Serial.println(getDeviceAddress(i));
		}
		Serial.println(FPSTR("-----"));
		Serial.println(FPSTR("dict = "));
		getDictionary().printTo(Serial);
		Serial.println();
		Serial.println(FPSTR("data = "));
		getData().printTo(Serial);
		Serial.println();
		Serial.println(FPSTR("-----"));
	}

protected:
	int itemCount=0;
	OneWireMock* oneWire;
	DallasTemperatureMock* dallasTemperature;

	void construct(int pin){
		this->oneWire=new OneWireMock(pin);
		this->dallasTemperature=new DallasTemperatureMock(oneWire);
	}

	void cleanUnnededFields(){
		if(modelDataProvider->getEntityData(id).containsKey(JSONKEY_items)){
			modelDataProvider->getEntityData(id).remove(JSONKEY_items);
		}
	}

	void initSensorModel(const char* sensorUid,float value){
		Serial.print(FPSTR("UID = "));
		Serial.println(sensorUid);

		JsonObject& item = JsonObjectUtil::getObjectChildOrCreateNew(modelDataProvider->getEntityData(id), JSONKEY_items).createNestedObject(strdup(sensorUid));

		item.set(JSONKEY_descr, getDictionaryValue(sensorUid));
		item.set(JSONKEY_temp, value);
	}
	void setSensorValue(const char* sensorUid,float value){
		JsonObjectUtil::getObjectChildOrCreateNew(modelDataProvider->getEntityData(id), JSONKEY_items).get<JsonObject>(sensorUid).set(JSONKEY_temp, value);
	}
	void setSensorDescr(const char* sensorUid,const char* descr){
		JsonObjectUtil::getObjectChildOrCreateNew(modelDataProvider->getEntityData(id), JSONKEY_items).get<JsonObject>(sensorUid).set(JSONKEY_descr, descr);
		setDictionaryValue(sensorUid, descr);
	}

	virtual void readTemperatures(){
		dallasTemperature->requestTemperatures();
		for(uint8_t i=0;i<itemCount;i++){
			String devAddressStr=getDeviceAddress(i);
			float t = dallasTemperature->getTempCByIndex(i);
			setSensorValue(devAddressStr.c_str(),t);
		}
	}

	String getDeviceAddress(uint8_t index){

		DeviceAddress deviceAddress;
		return dallasTemperature->getAddress(deviceAddress,index);
		uint8_t size=sizeof(deviceAddress);
		ObjectUtils::printInt8Arr(deviceAddress);
		return deviceAddressToString(deviceAddress,size);
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
