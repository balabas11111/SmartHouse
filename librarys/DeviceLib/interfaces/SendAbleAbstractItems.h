/*
 * SendAbleAbstractItems.h
 *
 *  Created on: 19 янв. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_SENDABLEABSTRACTITEMS_H_
#define LIBRARIES_DEVICELIB_INTERFACES_SENDABLEABSTRACTITEMS_H_

#include "Arduino.h"
#include "interfaces/SendAble.h"
#include "AbstractItem.h"
#include "StatusMessage/StatusMessage.h"

class SendAbleAbstractItems: public SendAble {
public:
	SendAbleAbstractItems(){};
	virtual ~SendAbleAbstractItems(){};

	void setItems(AbstractItem** items,uint8_t size){
		this->abstrItems=items;
		this->abstrItemsSize=size;
	}

	virtual StatusMessage send() override{
		return sendItems(getItems(), getItemsSize());
	}

	virtual StatusMessage sendItems(AbstractItem** items,uint8_t size)=0;

protected:
	AbstractItem** getItems(){
		return this->abstrItems;
	}

	uint8_t getItemsSize(){
		return abstrItemsSize;
	}
private:
	AbstractItem** abstrItems;
	uint8_t abstrItemsSize;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_SENDABLEABSTRACTITEMS_H_ */
