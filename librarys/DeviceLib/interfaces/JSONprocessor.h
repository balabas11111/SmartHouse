/*
 * JSONprocessor.h
 *
 *  Created on: Jan 17, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_JSONPROCESSOR_H_
#define LIBRARIES_DEVICELIB_JSONPROCESSOR_H_

#include "Arduino.h"
#include "interfaces/Nameable.h"
#include "StatusMessage/StatusMessage.h"

class JSONprocessor:public Nameable {
public:
	JSONprocessor(){};
	virtual ~JSONprocessor(){};

	virtual StatusMessage processJson(String page,String json)=0;

	void printProcessParams(String page,String json){
		Serial.print(getName());
		Serial.print(FPSTR(" - process Json"));
		Serial.print(FPSTR("page="));
		Serial.print(page);
		Serial.print(FPSTR(" json="));
		Serial.println(json);
	}
};

#endif /* LIBRARIES_DEVICELIB_JSONPROCESSOR_H_ */
