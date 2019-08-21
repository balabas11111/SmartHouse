/*
 * DS18D20sensor.h
 *
 *  Created on: Mar 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSENSORS_DS18D20SENSOR_H_
#define LIBRARIES_ENTITYLIBSENSORS_DS18D20SENSOR_H_

#include <ArduinoJson.h>
#include <Entity.h>
#include "UpdateAble.h"

#include <OneWire.h>
#include "DallasTemperature.h"
#include "ObjectUtils.h"

#include <OneWireMock.h>
#include "DallasTemperatureMock.h"

//--------------------------------
#define DS18D20_NAME "ds18d20"
#define DS18D20_DESCRIPTION "Temperature DallasTemperature"

#define DS18D20_DESCR "d"

#define DS18D20_SENSOR_COUNT "c"
#define DS18D20_SENSOR_ID "i"
#define DS18D20_SENSOR_NAME "n"
#define DS18D20_SENSOR_TEMPERATURE "t"

#define DS18D20_SENSOR_ITEMS "si"

class DS18D20item {
public:
	DeviceAddress uid;
	char* uidStr;
	char* descr;
	float temp;
};

class DS18D20sensor: public Entity, public UpdateAble {
public:
	DS18D20sensor(int pin) :
			Entity(ROOT_GROUP_SENSORS, DS18D20_NAME, DS18D20_DESCRIPTION) {
		construct(pin);
	}

	virtual ~DS18D20sensor() {
	}

	virtual void init() override {
		dallasTemperature->begin();
		itemCount = dallasTemperature->getDeviceCount();

		items = new DS18D20item[itemCount];

		for (uint8_t i = 0; i < itemCount; i++) {
			DS18D20item item = new DS18D20item();

			item.uidStr = dallasTemperature->getAddress(item.uid, i).c_str();
			item.descr = item.uidStr;
			item.temp = 127;

			items[i] = item;
		}
		readTemperatures(false);
		print();
	}

	virtual void update() override {
		readTemperatures(true);
	}

	void print() {
		Serial.println(FPSTR("DS18D20 Entity"));
		Serial.print(FPSTR(" name = "));
		Serial.print(name);
		Serial.print(FPSTR(" itemCount = "));
		Serial.println(itemCount);
		Serial.println(FPSTR("-----"));
		for (uint8_t i = 0; i < itemCount; i++) {
			Serial.print(FPSTR("i="));
			Serial.print(i);
			Serial.print(FPSTR(" address="));
			Serial.print(getDeviceAddress(i));

			Serial.print(FPSTR(" uidStr="));
			Serial.print(items[i].uidStr);
			Serial.print(FPSTR(" descr="));
			Serial.print(items[i].descr);
			Serial.print(FPSTR(" val="));
			Serial.println(items[i].temp);
		}
		Serial.println(FPSTR("-----"));
	}

	virtual void doGet(JsonObject& params, JsonObject& response) override {
		setJsonField(response, DS18D20_SENSOR_COUNT, this->itemCount);
		JsonObject& sensors = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(response, DS18D20_SENSOR_ITEMS);

		for(int i = 0;i,itemCount;i++){
			JsonObject& sensor = JsonObjectUtil::getObjectChildOrCreateNewNoKeyDup(sensors, items[i].uidStr);
			setJsonField(sensor, strdup(DS18D20_SENSOR_NAME), items[i].descr);
			setJsonField(sensor, strdup(DS18D20_SENSOR_TEMPERATURE), items[i].temp);
		}
	}

	virtual void doPost(JsonObject& params, JsonObject& response) override {
	}

	virtual void doLoad(JsonObject& jsonFromFile) override {
	}

	virtual void doSave(JsonObject& jsonToFile) override {
	}
protected:
	int itemCount = 0;
	OneWireMock* oneWire;
	DallasTemperatureMock* dallasTemperature;

	DS18D20item* items;

	void construct(int pin) {
		this->oneWire = new OneWireMock(pin);
		this->dallasTemperature = new DallasTemperatureMock(oneWire);
	}

	virtual void readTemperatures(bool dispatchChange) {
		dallasTemperature->requestTemperatures();
		bool tChanged = false;

		for (uint8_t i = 0; i < itemCount; i++) {
			float t = dallasTemperature->getTempCByIndex(i);

			if (t != items[i].temp) {
				tChanged = true;
			}
			items[i].temp = t;
		}

		if (dispatchChange) {
			dispatchChangeEvent(tChanged);
		}
	}

	String getDeviceAddress(uint8_t index) {
		DeviceAddress deviceAddress;
		return dallasTemperature->getAddress(deviceAddress, index);
	}
	/*uint8_t size = sizeof(deviceAddress);
	 ObjectUtils::printInt8Arr(deviceAddress);
	 return deviceAddressToString(deviceAddress, size);

	 }

	 String deviceAddressToString(DeviceAddress deviceAddress, uint8_t size) {
	 String devAddrStr = "";
	 for (uint8_t k = 0; k < size; k++) {
	 devAddrStr += String(deviceAddress[k]);
	 }
	 return devAddrStr;
	 }*/
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_DS18D20sensor_H_ */
