/*
 * AbstractSensorList.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORLIST_H_
#define LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORLIST_H_

#include <Arduino.h>

#include <interfaces/ArrayListSearchAble.h>
#include <interfaces/ChangeDispatchable.h>

#include <sensors/AbstractSensor.h>

class AbstractSensorList: public ArrayListSearchAble<AbstractSensor>, public ChangeDispatchable {
public:

	AbstractSensorList();
	AbstractSensorList(uint8_t initSize);
	virtual ~AbstractSensorList(){};

	AbstractSensor* getItemById(int id) override;
	AbstractSensor* getItemByName(String name) override;
	AbstractSensor* getItemByHash(String hash) override;

	boolean getChanged() override;
};

#endif /* LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORLIST_H_ */
