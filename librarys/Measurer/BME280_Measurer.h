/*
 * Bme180Measures.h
 *
 *  Created on: 14 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_BME280_MEASURER_H_
#define LIBRARIES_MEASURER_BME280_MEASURER_H_

#include <memory>
#include "Arduino.h"
//#include "Wire.h"
#include "Measurer.h"
#include "HtmlWidget.h"

#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"

const char MEASURER_NAME[] PROGMEM ="Bme180Sensor";
const char MEASURER_DESCRIPTION[] PROGMEM ="Bme180Sensor";

class BME280_Measurer: public Measurer, public HtmlWidget{

public:

	BME280_Measurer(String _id,String _name)
			: Measurer(_id, _name, FPSTR(MEASURER_NAME), FPSTR(MEASURER_DESCRIPTION), 4, false){
	}

	~BME280_Measurer(){
		Serial.println("destructed");
	}

	String getName(){
		return Measurer::getName();
	}

	String getJson(){
		return "";
	}

	String getHtml(){
		return "";
	}

	boolean init(){
		Serial.print("Init BME-280");
		bool status=bme.begin();
		Serial.print(" status="+String(status));
		Serial.println("...done");

		initialized=true;
		return status;
	}

	void getExternal() override{
		items[0]=Measureable("0",FPSTR(TEMPERATURE),FPSTR(MEASURE_CELSIUS_DEGREES),String(bme.readTemperature()));
		items[1]=Measureable("1",FPSTR(PRESSURE),FPSTR(MEASURE_PASCAL),String(bme.readPressure()));
		items[2]=Measureable("2",FPSTR(HUMIDITY),FPSTR(MEASURE_PERSENT),String(bme.readHumidity()));
		items[3]=Measureable("3",FPSTR(ALTITUDE),FPSTR(MEASURE_METER),String(bme.readAltitude(1013.25)));
	}



private:
	Adafruit_BME280 bme;
};



#endif /* LIBRARIES_MEASURER_BME180MEASURES_H_ */
