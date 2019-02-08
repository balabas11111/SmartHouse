/*
 * SendAbleAbstractSensors.h
 *
 *  Created on: 19 янв. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_SENDABLEABSTRACTSENSORS_H_
#define LIBRARIES_DEVICELIB_INTERFACES_SENDABLEABSTRACTSENSORS_H_

#include "Arduino.h"
#include "interfaces/SendAble.h"
#include "AbstractSensor.h"
#include "StatusMessage/StatusMessage.h"

class SendAbleAbstractSensors: public SendAble {
public:
	SendAbleAbstractSensors(){};
	virtual ~SendAbleAbstractSensors(){};

	void setItems(AbstractSensor** items,uint8_t size){
		Serial.println(FPSTR("Set abstract items TH helper"));
		this->abstrItems=items;
		this->abstrItemsSize=size;
	}

	virtual StatusMessage send() override{
		return sendItems(getItems(), getItemsSize());
	}

	virtual StatusMessage sendItems(AbstractSensor** items,uint8_t size)=0;

protected:
	AbstractSensor** getItems(){
		return this->abstrItems;
	}

	uint8_t getItemsSize(){
		return abstrItemsSize;
	}
private:
	AbstractSensor** abstrItems;
	uint8_t abstrItemsSize;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_SENDABLEABSTRACTSENSORS_H_ */
