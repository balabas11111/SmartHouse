/*
 * Bme180Measures.h
 *
 *  Created on: 14 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_BH1750_MEASURER_H_
#define LIBRARIES_MEASURER_BH1750_MEASURER_H_

#include "Arduino.h"
//#include "Wire.h"
#include "Measurer.h"

#include "BH1750.h"

class BH1750_Measurer: public Measurer{

public:

	BH1750_Measurer(String _id,String _name)
			: Measurer(_id, _name, "BH1750_Sensor", "LuxMeter", 1,false){
	}

	~BH1750_Measurer(){
		Serial.println("destructed");
	}

	boolean init(){
		Serial.print("Init luxmeter");
		lightMeter.begin();
		Serial.println("...done");
		initialized=true;
		return true;
	}

	void getExternal() override{
		items[0]=Measureable("0","light","Lux",String(lightMeter.readLightLevel()));
	}
private:
	BH1750 lightMeter;;
};



#endif /* LIBRARIES_MEASURER_BH1750_MEASURER_H_ */
