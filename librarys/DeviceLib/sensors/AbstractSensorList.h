/*
 * AbstractSensorList.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORLIST_H_
#define LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORLIST_H_

#include <Arduino.h>

#include <interfaces/ArrayList.h>
#include <interfaces/ChangeDispatchable.h>

#include <sensors/AbstractSensor.h>

class AbstractSensorList: public ArrayList<AbstractSensor>, public ChangeDispatchable {
public:

	AbstractSensorList();
	AbstractSensorList(uint8_t initSize);
	virtual ~AbstractSensorList(){};

	AbstractSensor* getItemById(int id);
	AbstractSensor* getItemByName(String name);
	AbstractSensor* getItemByHash(String hash);

	boolean getChanged() override;
};

#endif /* LIBRARIES_DEVICELIB_SENSORS_ABSTRACTSENSORLIST_H_ */
