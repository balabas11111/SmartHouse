/*
 * Entity.h
 *
 *  Created on: Feb 20, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_BASE_ENTITY_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_BASE_ENTITY_H_

#include "AbstractCLassDescr.h"

class Entity: public AbstractCLassDescr {
public:
	Entity(const char* name,
			const char* dao,
			const char* descr,
			const char* const* fields,
			const char* const* fields_Var,
			const char* const* fields_TVar,
			const char* const* fields_Persist,
			const char* const* fields_WithDefault,
			const char* const* fields_DefaultValues,
			uint8_t _fields_Size,
			uint8_t _fields_Var_Size,
			uint8_t _fields_TVar_Size,
			uint8_t _fields_Persist_Size,
			uint8_t _fields_WithDefault_Size,
			const char* clazz){

		this->name=name;
		this->dao=dao;
		this->descr=descr;
		this->clazz=clazz;
		this->_fields=fields;
		this->_fields_Var=fields_Var;
		this->_fields_TVar=fields_TVar;
		this->_fields_Persist=fields_Persist;
		this->_fields_WithDefault=fields_WithDefault;
		this->_fields_DefaultValues=fields_DefaultValues;

		this->_fields_Size=_fields_Size;
		this->_fields_Var_Size=_fields_Var_Size;
		this->_fields_TVar_Size=_fields_TVar_Size;
		this->_fields_Persist_Size=_fields_Persist_Size;
		this->_fields_WithDefault_Size=_fields_WithDefault_Size;

		this->isinit=true;
	}

	Entity( const char* json){
		int size=sizeof(json);
		this->json=json;
		this->hasJson=true;

		this->isinit=true;
	}

	virtual ~Entity(){};

		virtual void init(){};

		virtual bool isInit(){
			return this->isinit;
		}

		virtual bool isJson(){
			return this->hasJson;
		}

		virtual const char* getJson(){
			return this->json;
		}

		virtual const char* const* fields() override{ return this->_fields;}
		virtual const char* const* fields_Var() override{ return this->_fields_Var;}
		virtual const char* const* fields_TVar() override{ return this->_fields_TVar;}

		virtual const char* const* fields_Persist() override{ return this->_fields_Persist;}
		virtual const char* const* fields_WithDefault() override{ return this->_fields_WithDefault;}
		virtual const char* const* fields_DefaultValues() override{ return this->_fields_DefaultValues;}

		virtual const uint8_t fields_Size() override{ return this->_fields_Size;}
		virtual const uint8_t fields_Var_Size() override{ return this->_fields_Var_Size;}
		virtual const uint8_t fields_TVar_Size() override{ return this->_fields_TVar_Size;}
		virtual const uint8_t fields_Persist_Size() override{ return this->_fields_Persist_Size;}
		virtual const uint8_t fields_WithDefault_Size() override{ return this->_fields_WithDefault_Size;}

	protected:
		const char* const* _fields=nullptr;
		const char* const* _fields_Var=nullptr;
		const char* const* _fields_TVar=nullptr;
		const char* const* _fields_Persist=nullptr;
		const char* const* _fields_WithDefault=nullptr;
		const char* const* _fields_DefaultValues=nullptr;

		uint8_t _fields_Size=0;
		uint8_t _fields_Var_Size=0;
		uint8_t _fields_TVar_Size=0;
		uint8_t _fields_Persist_Size=0;
		uint8_t _fields_WithDefault_Size=0;

		const char* const json=nullptr;

		bool isinit=false;
		bool hasJson=false;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_BASE_ENTITY_H_ */
