/*
 * AbstractCLassDescrBase.h
 *
 *  Created on: 20 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_ABSTRACTCLASSDESCRBASE_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_ABSTRACTCLASSDESCRBASE_H_

#include "AbstractCLassDescr.h"
#include <entity/EntityConsts.h>

class AbstractCLassDescrBase: public AbstractCLassDescr {
public:
	AbstractCLassDescrBase(const char* name,const char* dao,const char* descr){
		this->name=name;
		this->descr = descr;
		this->dao=dao;
		this->tableT=nullptr;
	}
	virtual ~AbstractCLassDescrBase(){};

	virtual const char* getDescription(){

	}

	virtual boolean isTable() override{
		return (getTable()!=nullptr);
	}

	AbstractCLassDescr* getTable(){
		return this->tableT;
	}

	virtual const char* autoincField() override{
		if(!isTable()){
			return nullptr;
		}
		return JSONKEY_id;
	}

protected:
	const char* name;
	const char* dao;
	const char* descr;
	AbstractCLassDescr* tableT;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_ABSTRACTCLASSDESCRBASE_H_ */
