/*
 * SendAble.h
 *
 *  Created on: 18 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_SENDABLE_H_
#define LIBRARIES_DEVICELIB_SENDABLE_H_

#include "Arduino.h"
#include "StatusMessage/StatusMessage.h"
#include "AbstractItem.h"

#include "interfaces/Nameable.h"

class SendAble:public Nameable {
public:
	SendAble(){};
	virtual ~SendAble(){};

	virtual StatusMessage send()=0;

	virtual StatusMessage sendItems(String sender,String receiver,String json){
		return StatusMessage(STATUS_NOT_IMPLEMENTED_INT);
	}

};

#endif /* LIBRARIES_DEVICELIB_SENDABLE_H_ */
