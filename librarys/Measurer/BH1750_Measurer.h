/*
 * Bme180Measures.h
 *
 *  Created on: 14 îêò. 2018 ã.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_MEASURER_BH1750_MEASURER_H_
#define LIBRARIES_MEASURER_BH1750_MEASURER_H_

#include "Arduino.h"
#include "AbstractItem.h"
#include "Initializable.h"

#include "BH1750.h"

const char MEASURER_BH1750_NAME[] PROGMEM ="BH1750_Sensor";
const char MEASURER_BH1750_DESCRIPTION[] PROGMEM ="LuxMeter";
const char MEASURER_BH1750_SIZE[] PROGMEM ="Lux";
const char MEASURER_BH1750_DESCRIPTION_RU[] PROGMEM ="Освещенность BH1750";

class BH1750_Measurer: public AbstractItem, public Initializable{

public:

	BH1750_Measurer(uint8_t id,String name,uint8_t fieldId,String queueName)
			: AbstractItem(id,name,FPSTR(MEASURER_BH1750_DESCRIPTION),FPSTR(MEASURER_BH1750_SIZE),FPSTR(MEASURER_BH1750_DESCRIPTION_RU),
					0, 1,fieldId,queueName){
	}

	~BH1750_Measurer(){
		Serial.println("destructed");
	}

	boolean initialize(boolean _init){
		if(_init){
			Serial.println("-------BH1750_Measurer initialize------------");
			Serial.print("Init luxmeter");
			lightMeter.begin();
			Serial.println("...done");
			Serial.println("---------------------------------------------");

			initSensor();
			measure();
		}
		initialized=_init;
		return initialized;
	}


	void initSensor(){
		items[0]={0,"light","LuxMeter","Люкс","Освещенность",(float)lightMeter.readLightLevel(),0};
	}
	//----------------------------------------------

	void measure(){
		items[0].val=(float)lightMeter.readLightLevel();
	}

private:
	BH1750 lightMeter;;
};



#endif /* LIBRARIES_MEASURER_BH1750_MEASURER_H_ */
