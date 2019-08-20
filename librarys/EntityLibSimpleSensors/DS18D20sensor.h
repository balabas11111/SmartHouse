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
#define DS18D20_TEMPERATURE "t"

class DS18D20item{
public:
	DeviceAddress uid;
	char* descr;
	float val;
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
			initSensorModel(getDeviceAddress(i).c_str(), 127);
		}
		readTemperatures();
		print();
	}

	virtual void update() override {
		readTemperatures();
	}

	void print() {
		Serial.println(FPSTR("DS18D20 Entity"));
		Serial.print(FPSTR(" name = "));
		Serial.print(name);
		Serial.print(FPSTR(" itemCount = "));
		Serial.println(itemCount);
		Serial.println(FPSTR("-----"));
		for (uint8_t i = 0; i < itemCount; i++) {
			Serial.print("i=");
			Serial.print(i);
			Serial.print(" address=");
			Serial.println(getDeviceAddress(i));
		}
		Serial.println(FPSTR("-----"));
		Serial.println();
		Serial.println(FPSTR("data = "));
		Serial.println();
		Serial.println(FPSTR("-----"));
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

	virtual void readTemperatures() {
		dallasTemperature->requestTemperatures();
		for (uint8_t i = 0; i < itemCount; i++) {
			String devAddressStr = getDeviceAddress(i);
			float t = dallasTemperature->getTempCByIndex(i);
			setSensorValue(devAddressStr.c_str(), t);
		}
	}

	String getDeviceAddress(uint8_t index) {

		DeviceAddress deviceAddress;
		return dallasTemperature->getAddress(deviceAddress, index);
		/*uint8_t size = sizeof(deviceAddress);
		ObjectUtils::printInt8Arr(deviceAddress);
		return deviceAddressToString(deviceAddress, size);
		*/
	}

	String deviceAddressToString(DeviceAddress deviceAddress, uint8_t size) {
		String devAddrStr = "";
		for (uint8_t k = 0; k < size; k++) {
			devAddrStr += String(deviceAddress[k]);
		}
		return devAddrStr;
	}
};

#endif /* LIBRARIES_ENTITYLIBSENSORS_DS18D20sensor_H_ */
