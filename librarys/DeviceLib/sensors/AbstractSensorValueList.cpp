/*
 * AbstractSensorValueList.cpp
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <sensors/AbstractSensorValueList.h>

AbstractSensorValueList::AbstractSensorValueList() {
}

AbstractSensorValueList::AbstractSensorValueList(uint8_t initSize) {
	Serial.print(FPSTR("Init AbstractSensorValueList initSize="));
	Serial.print(initSize);

	if(initSize!=0){
		AbstractSensorValue* sensValues[] = new AbstractSensorValue[initSize];
		constructList(sensValues, initSize);
	}

	Serial.println(FPSTR("...done"));
}
