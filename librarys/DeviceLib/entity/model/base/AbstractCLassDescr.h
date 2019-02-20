/*
 * AbstractCLassDescr.h
 *
 *  Created on: 20 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_BASE_ABSTRACTCLASSDESCR_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_BASE_ABSTRACTCLASSDESCR_H_

#include <Arduino.h>
#include <interfaces/HashableItem.h>

class AbstractCLassDescr: public HashableItem {
public:
	virtual ~AbstractCLassDescr(){};

	virtual const char* getName() = 0;
	virtual const char* getDao() = 0;
	virtual const char* getClass() = 0;

	//description and variables
	virtual const char* const*  fields() =0;
	virtual const char* const* fields_Var() =0;
	virtual const char* const* fields_TVar() =0;

	virtual const char* const* fields_Persist() =0;
	virtual const char* const* fields_WithDefault() =0;
	virtual const char* const* fields_DefaultValues() =0;

	virtual const uint8_t fields_Size()=0;
	virtual const uint8_t fields_Var_Size()=0;
	virtual const uint8_t fields_TVar_Size()=0;
	virtual const uint8_t fields_Persist_Size()=0;
	virtual const uint8_t fields_WithDefault_Size()=0;

	virtual boolean isTable() = 0;

	//security part
/*
    virtual const char* autoincField() = 0;
    virtual const char** getHashAbleFields()  =0;

	virtual const char** getList_Fields() =0;
	virtual const char** getSave_Fields() =0;
	virtual const char** getSaveTemplate_Fields() =0;
	virtual const char** getLoad_Fields() =0;
	virtual const char** getSet_Fields() =0;
	virtual const char** getPut_Fields() =0;
	virtual const char** getGet_Fields() =0;
*/

};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_ABSTRACTCLASSDESCR_H_ */
