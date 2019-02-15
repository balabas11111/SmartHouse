/*
 * UpdateAble.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_UPDATEABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_UPDATEABLE_H_

#include <Arduino.h>

class UpdateAble {
public:
	virtual ~UpdateAble(){};
	virtual boolean update(){return true;};
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_UPDATEABLE_H_ */
