/*
 * DS18D20Sensor.h
 *
 *  Created on: 3 нояб. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_SENSORSLIB_DS18D20_SENSOR_H_
#define LIBRARIES_SENSORSLIB_DS18D20_SENSOR_H_

#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"

#include "AbstractItem.h"
#include "interfaces/Initializable.h"
#include "interfaces/Measurable.h"

#include "ESP_Consts.h"


const char SENSOR_DS18D20_NAME[] PROGMEM ="DS18D20_Sensor";
const char SENSOR_DS18D20_DESCRIPTION[] PROGMEM ="DS18D20 Sensor";
const char SENSOR_DS18D20_SIZE[] PROGMEM ="C";
const char SENSOR_DS18D20_DESCRIPTION_RU[] PROGMEM ="Температура °C DS18D20";

const char HEADER_DS18D20[] PROGMEM ="-------DS18D20 initialize------------";
const char INIT_STR_DS18D20[] PROGMEM ="Init DS18D20";

class DS18D20_Sensor: public AbstractItem, public Initializable {
public:
	DS18D20_Sensor(String name, uint8_t pin)
				: AbstractItem(pin,name,FPSTR(SENSOR_DS18D20_DESCRIPTION),FPSTR(SENSOR_DS18D20_SIZE),FPSTR(SENSOR_DS18D20_DESCRIPTION_RU),0){

		this->pin=pin;
	};

	virtual ~DS18D20_Sensor(){};

	boolean initialize(boolean _init){
		if(_init){
			Serial.println(FPSTR(HEADER_DS18D20));
			Serial.print(FPSTR(INIT_STR_DS18D20));
			Serial.print(name);Serial.print(" ");Serial.println(pin);

			initSensor();
			update();

			Serial.println(FPSTR(MESSAGE_DONE));
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		}
		initialized=_init;
		return initialized;
	}

	void initSensor(){

		oneWire=new OneWire(pin);
		dallasTemperature=new DallasTemperature(oneWire);

		dallasTemperature->begin();
		//dallasTemperature->setDescription(getName());

		itemCount=dallasTemperature->getDeviceCount();

		initializeChildren();

		Serial.print("Sensors count=");
		Serial.println(itemCount);

		for(uint8_t i=0;i<itemCount;i++){

			String devAddressStr=getDeviceAddress(i);
			String name=String(FPSTR(TEMPERATURE))+devAddressStr;

			//uint8_t field=i+1;
			items[i]={i,name,FPSTR(MEASURE_CELSIUS_DEGREES),FPSTR(MEASURE_CELSIUS_DEGREES_RU),devAddressStr,-127,0,-512,512,"",0};
		}

		if(itemCount>0){
			periodicSend=true;
		}else{
			periodicSend=false;
		}
		Serial.println("complete init bus");

	}

	void update(){
		if(itemCount!=0){
			dallasTemperature->requestTemperatures();

			for(uint8_t i=0;i<itemCount;i++){
				items[i].val=dallasTemperature->getTempCByIndex(i);
			}
		}else{
			Serial.println(FPSTR("Reinitialize bus"));
			initSensor();
		}
	}

	void print(){
		printValues();
	}

	String getKind() override{
		return KIND_SENSOR;
	}

private:
	uint8_t pin;
	OneWire* oneWire;
	DallasTemperature* dallasTemperature;

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

#endif /* LIBRARIES_SENSORSLIB_DS18D20_SENSOR_H_ */
