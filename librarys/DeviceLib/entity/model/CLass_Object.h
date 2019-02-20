/*
 * CLass_Object.h
 *
 *  Created on: 20 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_CLASS_OBJECT_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_CLASS_OBJECT_H_

#include "AbstractCLassDescrBase.h"

class CLass_Object:public AbstractCLassDescrBase {
public:
	CLass_Object(const char* name,const char* dao,const char* descr):
		AbstractCLassDescrBase(name,dao,descr){
	}
	virtual ~CLass_Object(){};

	virtual const char* getClass(){
		return ENTITY_CLASS_object;
	}
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_CLASS_OBJECT_H_ */
