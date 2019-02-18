/*
 * SizeAble.h
 *
 *  Created on: 16 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_SIZEABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_SIZEABLE_H_

#include <Arduino.h>

class SizeAble {
public:
	virtual ~SizeAble();

	virtual String getSize()=0;

	virtual boolean checkSize(String type){
		return this->getSize()==type;
	}
protected:
	String size;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_SIZEABLE_H_ */
