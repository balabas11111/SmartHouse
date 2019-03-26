/*
 * AMock.h
 *
 *  Created on: 26 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBMOCK_AMOCK_H_
#define LIBRARIES_ENTITYLIBMOCK_AMOCK_H_

#include "Arduino.h"

class AMock {
public:
	AMock(){};
	virtual ~AMock(){};

	virtual void begin(){
		//Serial.print(typeid(this).name());
		Serial.println(FPSTR("Mocking->begun"));
	};

	virtual int random(int min,int max){
		return min + (rand() % static_cast<int>(max - min + 1));
	}
};

#endif /* LIBRARIES_ENTITYLIBMOCK_AMOCK_H_ */
