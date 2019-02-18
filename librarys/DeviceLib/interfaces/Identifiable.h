/*
 * Identifiable.h
 *
 *  Created on: Feb 4, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_IDENTIFIABLE_H_
#define LIBRARIES_DEVICELIB_IDENTIFIABLE_H_

#include "Arduino.h"

class Identifiable {
public:
	virtual ~Identifiable(){};

	virtual int getId(){
		return id;
	}
protected:
	int id=-1;
};

#endif /* LIBRARIES_DEVICELIB_IDENTIFIABLE_H_ */
