/*
 * Bme180Measures.h
 *
 *  Created on: 14 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_BME280_MEASURER_H_
#define LIBRARIES_MEASURER_BME280_MEASURER_H_

#include "Arduino.h"
//#include "Wire.h"
#include "Measurer.h"

#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"

class BME280_Measurer: public Measurer{

public:

	BME280_Measurer(String _id,String _name)
			: Measurer(_id, _name, "Bme180Sensor", "Temp/Press/Humid/Att", 4, false){
	}

	~BME280_Measurer(){
		Serial.println("destructed");
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
		items[0]=Measureable("0","Temp-re","C",String(bme.readTemperature()));
		items[1]=Measureable("1","Pressure","Pa",String(bme.readPressure()));
		items[2]=Measureable("2","Humidity","%",String(bme.readHumidity()));
		items[3]=Measureable("3","Altitude","C",String(bme.readAltitude(1013.25)));
	}



private:
	Adafruit_BME280 bme;
};



#endif /* LIBRARIES_MEASURER_BME180MEASURES_H_ */
