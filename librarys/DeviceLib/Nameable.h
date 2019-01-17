/*
 * Nameable.h
 *
 *  Created on: Jan 17, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_NAMEABLE_H_
#define LIBRARIES_DEVICELIB_NAMEABLE_H_

#include "Arduino.h"

class Nameable {
public:
	Nameable(){};
	virtual ~Nameable(){};

	virtual String getName()=0;

	virtual boolean checkName(String name,String page){
		return this->getName()==name;
	}
};

#endif /* LIBRARIES_DEVICELIB_NAMEABLE_H_ */
