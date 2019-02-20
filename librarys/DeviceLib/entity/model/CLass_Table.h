/*
 * CLass_Table.h
 *
 *  Created on: 20 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_CLASS_TABLE_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_CLASS_TABLE_H_

#include "AbstractCLassDescrBase.h"

class CLass_Table:public AbstractCLassDescrBase {
public:
	CLass_Table(const char* name,const char* dao,const char* descr):
		AbstractCLassDescrBase(name,dao,descr){
	}
	virtual ~CLass_Table(){};

	virtual const char* getClass(){
		return ENTITY_CLASS_table;
	}
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_CLASS_TABLE_H_ */
