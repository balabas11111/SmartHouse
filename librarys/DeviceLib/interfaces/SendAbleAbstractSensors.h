/*
 * SendAbleAbstractSensors.h
 *
 *  Created on: 19 янв. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_SENDABLEABSTRACTSENSORS_H_
#define LIBRARIES_DEVICELIB_INTERFACES_SENDABLEABSTRACTSENSORS_H_

#include <sensors/AbstractSensorList.h>
#include "Arduino.h"
#include "interfaces/SendAble.h"
#include "StatusMessage/StatusMessage.h"

class SendAbleAbstractSensors: public SendAble {
public:
	SendAbleAbstractSensors(){};
	virtual ~SendAbleAbstractSensors(){};

	void setItems(AbstractSensorList* sensors){
		Serial.println(FPSTR("Set abstract items TH helper"));
		this->sensors=sensors;
	}

	virtual StatusMessage send() override{
		return sendItems(getItems());
	}

	virtual StatusMessage sendItems(AbstractSensorList*)=0;

protected:
	AbstractSensorList* getItems(){
		return this->sensors;
	}

	uint8_t getItemsSize(){
		return sensors->getSize();
	}
private:
	AbstractSensorList* sensors;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_SENDABLEABSTRACTSENSORS_H_ */
