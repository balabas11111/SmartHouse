/*
 * AbstractSensorValueList.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORVALUELIST_H_
#define LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORVALUELIST_H_

#include <Arduino.h>

#include <interfaces/ArrayList.h>
#include <interfaces/ArrayListSearchAble.h>
#include <interfaces/ChangeDispatchable.h>

#include <sensors/AbstractSensorValue.h>

class AbstractSensorValueList: public ArrayListSearchAble<AbstractSensorValue>, public ChangeDispatchable {
public:
	AbstractSensorValueList();
	AbstractSensorValueList(uint8_t initSize);
	virtual ~AbstractSensorValueList(){};

	AbstractSensorValue* getItemById(int id) override;
	AbstractSensorValue* getItemByName(String name) override;
	AbstractSensorValue* getItemByHash(String hash) override;

	boolean getChanged() override;
};

#endif /* LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORVALUELIST_H_ */
