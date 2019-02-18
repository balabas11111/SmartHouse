/*
 * DescribeAble.h
 *
 *  Created on: 16 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_DESCRIBEABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_DESCRIBEABLE_H_

#include <Arduino.h>

#include <Arduino.h>

class DescribeAble {
public:
	DescribeAble();
	virtual ~DescribeAble();

	virtual String getDescr()=0;

	virtual boolean checkDescr(String descr){
		return this->getDescr()==descr;
	}
protected:
	String descr;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_DESCRIBEABLE_H_ */
