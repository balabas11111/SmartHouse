/*
 * AbstractItemReq.h
 *
 *  Created on: Nov 5, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ABSTRACTITEMREQ_H_
#define LIBRARIES_DEVICELIB_ABSTRACTITEMREQ_H_

#include "Arduino.h"

struct AbstractItemRequest{
	bool valid;
	uint8_t deviceId;
	uint8_t itemId;
	uint8_t fieldId;
	String fieldVal;
};

#endif /* LIBRARIES_DEVICELIB_ABSTRACTITEMREQ_H_ */
