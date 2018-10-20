/*
 * DeviceHelper.h
 *
 *  Created on: 13 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINDIGITAL_DeviceHelper_H_
#define LIBRARIES_PINDIGITAL_DeviceHelper_H_

#include <Loopable.h>
#include <Initializable.h>
#include <Arduino.h>

class DeviceHelper:public Loopable{

public:
	DeviceHelper(Loopable** loopItems,uint8_t loopItemsSize);
	String displayDetails();
	boolean loop();

	void startDevice(String deviceId);
	boolean init(Initializable** initItems,uint8_t initItemsSize);

	void printDeviceDiagnostic();
	String getWidgetsJson();
private:
	Loopable** loopItems;
	uint8_t loopItemsSize;
};

#endif /* LIBRARIES_PINDIGITAL_DeviceHelper_H_ */
