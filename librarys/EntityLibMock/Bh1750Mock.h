/*
 * Bh1750Mock.h
 *
 *  Created on: 26 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBMOCK_BH1750MOCK_H_
#define LIBRARIES_ENTITYLIBMOCK_BH1750MOCK_H_

#include "Arduino.h"
#include "AMock.h"
#include "ObjectUtils.h"

class Bh1750Mock:public AMock {
public:
	Bh1750Mock(){};
	virtual ~Bh1750Mock(){};

	uint16_t readLightLevel(){
		return 1000+random(1,500);
	}
};

#endif /* LIBRARIES_ENTITYLIBMOCK_BH1750MOCK_H_ */
