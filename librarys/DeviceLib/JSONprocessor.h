/*
 * JSONprocessor.h
 *
 *  Created on: Jan 17, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_JSONPROCESSOR_H_
#define LIBRARIES_DEVICELIB_JSONPROCESSOR_H_

#include "Arduino.h"
#include "Nameable.h"
#include "StatusMessage.h"

class JSONprocessor:public Nameable {
public:
	JSONprocessor(){};
	virtual ~JSONprocessor(){};

	virtual StatusMessage processJson(String page,String json)=0;
};

#endif /* LIBRARIES_DEVICELIB_JSONPROCESSOR_H_ */
