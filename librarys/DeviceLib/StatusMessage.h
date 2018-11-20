/*
 * AbstractItemReq.h
 *
 *  Created on: Nov 5, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_STATUSMESSAGE_H_
#define LIBRARIES_DEVICELIB_STATUSMESSAGE_H_

#include "Arduino.h"

struct StatusMessage{
	String status;
	String message;

	void setVals(String st,String msg){
		status=st;
		message=msg;
	}
};

#endif /* LIBRARIES_DEVICELIB_STATUSMESSAGE_H_ */
