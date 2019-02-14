/*
 * AbstractSensorList.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <sensors/AbstractSensorList.h>

AbstractSensorList::AbstractSensorList(AbstractSensor* sensors[]) {
	constructList(sensors, ARRAY_SIZE(sensors));
}

AbstractSensorList::AbstractSensorList(uint8_t initSize) {
	Serial.print(FPSTR("Init AbstractSensorList initSize="));
	Serial.print(initSize);

	if(initSize!=0){
		AbstractSensor* sensValues[] = new AbstractSensor[initSize];
		constructList(sensValues, initSize);
	}

	Serial.println(FPSTR("...done"));
}
