/*
 * AbstractSensorValueList.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORVALUELIST_H_
#define LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORVALUELIST_H_

#include <Arduino.h>

#include <list/ArrayListSearchAble.h>

#include <sensors/AbstractSensorValue.h>

class AbstractSensorValueList: public ArrayListSearchAble<AbstractSensorValue> {
public:
	AbstractSensorValueList();
	AbstractSensorValueList(uint8_t initSize);
	//virtual ~AbstractSensorValueList(){};
};

#endif /* LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORVALUELIST_H_ */
