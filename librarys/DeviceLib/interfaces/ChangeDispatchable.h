/*
 * ChangeDispatchable.h
 *
 *  Created on: Feb 14, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_CHANGEDISPATCHABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_CHANGEDISPATCHABLE_H_

#include <Arduino.h>

class ChangeDispatchable {
public:

	virtual boolean getChanged(){
		return this->changed;
	}
	virtual void setChanged(boolean changed){
		this->changed=changed;
	}

private:
	boolean changed=false;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_CHANGEDISPATCHABLE_H_ */
