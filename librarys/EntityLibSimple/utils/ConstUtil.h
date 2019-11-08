/*
 * Constants container
 *
 *  Created on: 3 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_UTILS_CONSTUTIL_H_
#define LIBRARIES_ENTITYLIBSIMPLE_UTILS_CONSTUTIL_H_

#include <Arduino.h>

const char HORIZONTAL_LINE[]    PROGMEM="---------------------------------------";
const char HORIZONTAL_LINE_EQ[] PROGMEM="=======================================";

class ConstUtil {
public:
	ConstUtil(){}
	virtual ~ConstUtil(){}

	static void printHorLine(){
		Serial.println(FPSTR(HORIZONTAL_LINE));
	}
	static void printHorLineEq(){
		Serial.println(FPSTR(HORIZONTAL_LINE_EQ));
	}
};

#endif /* LIBRARIES_UTILS_CONSTUTIL_H_ */
