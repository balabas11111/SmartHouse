/*
 * Bme180Measures.h
 *
 *  Created on: 14 îêò. 2018 ã.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_BME280_MEASURER_H_
#define LIBRARIES_MEASURER_BME280_MEASURER_H_

#include "Arduino.h"
#include "Initializable.h"
#include "AbstractItem.h"
#include "Initializable.h"

#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"

const char MEASURER_BME280_NAME[] PROGMEM ="Bme280Sensor";
const char MEASURER_BME280_DESCRIPTION[] PROGMEM ="Bme280Sensor";
const char MEASURER_BME280_SIZE[] PROGMEM ="C/%/Pa/m";
const char MEASURER_BME280_DESCRIPTION_RU[] PROGMEM ="Климат BME280";

class BME280_Measurer: public AbstractItem, public Initializable{

public:

	BME280_Measurer(uint8_t id,String name,uint8_t fieldId,String queueName)
		: AbstractItem(id,name,FPSTR(MEASURER_BME280_DESCRIPTION),FPSTR(MEASURER_BME280_SIZE),FPSTR(MEASURER_BME280_DESCRIPTION_RU),
					0, 1,fieldId,queueName){
	}

	~BME280_Measurer(){
		Serial.println("destructed");
	}

	boolean initialize(boolean _init){
		Serial.println("Begin initialize of BME280_Measurer");
		if(_init){
			Serial.println("-------BME280_Measurer initialize------------");
			Serial.print("Init BME-280");
			bool status=bme.begin();
			Serial.print(" status="+String(status));
			Serial.println("...done");
			Serial.println("---------------------------------------------");
			initSensor();
		}
		initialized=_init;
		return initialized;
	}

	void initSensor(){
		items[0]={0,FPSTR(TEMPERATURE),FPSTR(MEASURE_CELSIUS_DEGREES),FPSTR(MEASURE_CELSIUS_DEGREES_RU),FPSTR(TEMPERATURE_RU),(float)bme.readTemperature(),1};
		items[1]={0,FPSTR(PRESSURE),FPSTR(MEASURE_PASCAL),FPSTR(MEASURE_PASCAL_RU),FPSTR(PRESSURE_RU),(float)bme.readPressure(),2};
		items[2]={0,FPSTR(HUMIDITY),FPSTR(MEASURE_PERSENT),FPSTR(MEASURE_PERSENT_RU),FPSTR(HUMIDITY_RU),(float)bme.readHumidity(),3};
		items[3]={0,FPSTR(ALTITUDE),FPSTR(MEASURE_METER),FPSTR(MEASURE_METER_RU),FPSTR(ALTITUDE_RU),(float)bme.readAltitude(1013.25),4};
	}

	void measure(){
		items[0].val=(float)bme.readTemperature();
		items[1].val=(float)bme.readPressure();
		items[2].val=(float)bme.readHumidity();
		items[3].val=(float)bme.readAltitude(1013.25);
	}

private:
	Adafruit_BME280 bme;
};



#endif /* LIBRARIES_MEASURER_BME180MEASURES_H_ */
