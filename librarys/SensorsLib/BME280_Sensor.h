/*
 * Bme180Measures.h
 *
 *  Created on: 14 îêò. 2018 ã.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_SENSOR_BME280_SENSOR_H_
#define LIBRARIES_SENSOR_BME280_SENSOR_H_

#include "Arduino.h"
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"

#include "interfaces/Initializable.h"
#include "AbstractSensor.h"
#include "ESP_Consts.h"

#define BME280_ID 200

#define BME_280_TEMPERATURE_INDEX 0
#define BME_280_PRESSURE_INDEX 1
#define BME_280_HUMIDITY_INDEX 2
#define BME_280_ALTITUDE_INDEX 3

const char SENSOR_BME280_NAME[] PROGMEM ="Bme280Sensor";
const char SENSOR_BME280_DESCRIPTION[] PROGMEM ="Bme280Sensor";
const char SENSOR_BME280_SIZE[] PROGMEM ="C/%/Pa/m";
const char SENSOR_BME280_DESCRIPTION_RU[] PROGMEM ="Климат BME280";

const char HEADER_BME280[] PROGMEM ="-------BME280_SENSOR initialize------------";
const char INIT_STR_BME280[] PROGMEM ="Init BME-280";

class BME280_Sensor: public AbstractSensor, public Initializable{

public:

	BME280_Sensor(String name)
		: AbstractSensor(BME280_ID,name,FPSTR(SENSOR_BME280_DESCRIPTION),FPSTR(SENSOR_BME280_SIZE),FPSTR(SENSOR_BME280_DESCRIPTION_RU),4){
	}

	~BME280_Sensor(){}

	boolean initialize(boolean _init){
		if(_init){
			Serial.println(FPSTR(HEADER_BME280));
			Serial.print(FPSTR(INIT_STR_BME280));

			initSensor();
			update();

			Serial.println(FPSTR(MESSAGE_DONE));
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

		}
		initialized=_init;
		return initialized;
	}

	void initSensor(){

		bool status=bme.begin();
		Serial.print(FPSTR(MESSAGE_STATUS_EQUAL));
		Serial.println(status);

		this->status=status;

		items[BME_280_TEMPERATURE_INDEX]={BME_280_TEMPERATURE_INDEX,FPSTR(TEMPERATURE),FPSTR(MEASURE_CELSIUS_DEGREES),FPSTR(MEASURE_CELSIUS_DEGREES_RU),FPSTR(TEMPERATURE_RU),-127,0,-512,512,0};
		items[BME_280_PRESSURE_INDEX]={BME_280_PRESSURE_INDEX,FPSTR(PRESSURE),FPSTR(MEASURE_PASCAL),FPSTR(MEASURE_PASCAL_RU),FPSTR(PRESSURE_RU),-127,0,-512,120000,0};
		items[BME_280_HUMIDITY_INDEX]={BME_280_HUMIDITY_INDEX,FPSTR(HUMIDITY),FPSTR(MEASURE_PERSENT),FPSTR(MEASURE_PERSENT_RU),FPSTR(HUMIDITY_RU),-127,0,-512,512,0};
		items[BME_280_ALTITUDE_INDEX]={BME_280_ALTITUDE_INDEX,FPSTR(ALTITUDE),FPSTR(MEASURE_METER),FPSTR(MEASURE_METER_RU),FPSTR(ALTITUDE_RU),-127,0,-512,10000,0};

	}

	void update(){
		items[BME_280_TEMPERATURE_INDEX].val=(float)bme.readTemperature();
		items[BME_280_PRESSURE_INDEX].val=(float)bme.readPressure();
		items[BME_280_HUMIDITY_INDEX].val=(float)bme.readHumidity();
		items[BME_280_ALTITUDE_INDEX].val=(float)bme.readAltitude(1013.25);
	}
	void print(){
		printValues();
	}

	float getTemperatureCelsius(){
		return items[BME_280_TEMPERATURE_INDEX].val;
	}

	float getPressurePascal(){
		return items[BME_280_PRESSURE_INDEX].val;
	}

	float getPressureHgmm(){
		float result=items[BME_280_PRESSURE_INDEX].val;
		result=result*(0.00750062);
		return result;
	}

	float getHumidityPercent(){
		return items[BME_280_HUMIDITY_INDEX].val;
	}

	String getKind() override{
		return KIND_SENSOR;
	}

private:
	Adafruit_BME280 bme;
};



#endif /* LIBRARIES_SENSOR_BME180MEASURES_H_ */
