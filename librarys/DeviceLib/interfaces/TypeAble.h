/*
 * TypeAble.h
 *
 *  Created on: 16 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_TYPEABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_TYPEABLE_H_

#include <Arduino.h>

class TypeAble {
public:
	virtual ~TypeAble();

	virtual String getType()=0;

	virtual boolean checkType(String type){
		return this->getType()==type;
	}
protected:
	String type;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_TYPEABLE_H_ */
