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
	virtual ~Nameable(){};

	virtual String getName()=0;

	virtual boolean checkName(String name){
		return this->getName()==name;
	}

	virtual boolean checkPage(String page){
		return (page!=NULL);
	}

	virtual boolean checkNamePage(String name,String page){
		return checkName(name) && checkPage(page);
	}
protected:
	String name;
};

#endif /* LIBRARIES_DEVICELIB_NAMEABLE_H_ */
