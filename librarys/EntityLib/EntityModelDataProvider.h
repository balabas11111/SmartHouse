/*
 * EntityModelDataProvider.h
 *
 *  Created on: 3 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_ENTITYMODELDATAPROVIDER_H_
#define LIBRARIES_ENTITYLIB_ENTITYMODELDATAPROVIDER_H_

#include <Arduino.h>
#include <ArduinoJson.h>

class EntityModelDataProvider {
public:
	virtual ~EntityModelDataProvider(){};

	virtual JsonObject& getEntityModel(int entityId)=0;
	virtual JsonObject& getEntityData(int entityId)=0;

	virtual bool hasFieldInt(int entityId,const char* key)=0;
	virtual bool hasFieldFloat(int entityId,const char* key)=0;
	virtual bool hasFieldConstChar(int entityId,const char* key)=0;
	virtual bool hasFieldChar(int entityId, const char* key)=0;

	virtual bool setField(int entityId,const char* key,int value)=0;
	virtual bool setField(int entityId,const char* key,float value)=0;
	virtual bool setField(int entityId,const char* key,const char* value)=0;
	virtual bool setField(int entityId,const char* key,char* value)=0;

	virtual int getFieldInt(int entityId,const char* key)=0;
	virtual float getFieldFloat(int entityId,const char* key)=0;
	virtual const char* getFieldConstChar(int entityId,const char* key)=0;
	virtual char* getFieldChar(int entityId,const char* key)=0;
};

#endif /* LIBRARIES_ENTITYLIB_ENTITYMODELDATAPROVIDER_H_ */
