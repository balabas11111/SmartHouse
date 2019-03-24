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
#include "JsonObjectUtil.h"
#include "EventSender.h"

class EntityModelDataProvider {
public:
	virtual ~EntityModelDataProvider(){};

	virtual JsonObject& getEntityModel(int entityId)=0;
	virtual JsonObject& getEntityData(int entityId)=0;
	virtual JsonObject& getEntityRoot()=0;

	virtual bool saveRootToFileIfChanged()=0;

	virtual JsonArray& getEntityDataFieldsByAction(int entityId,const char* action)=0;

	virtual const char* validateField(int entityId,const char* key,const String& value)=0;

	virtual void sendEntityAsEventSourceNow(int entityId)=0;

	virtual bool hasFieldInt(int entityId,const char* key){return JsonObjectUtil::hasField<int>(getEntityData(entityId), key);}
	virtual bool hasFieldFloat(int entityId,const char* key){return JsonObjectUtil::hasField<float>(getEntityData(entityId), key);}
	virtual bool hasFieldConstChar(int entityId,const char* key){return JsonObjectUtil::hasField<const char*>(getEntityData(entityId), key);}
	virtual bool hasFieldChar(int entityId, const char* key){return JsonObjectUtil::hasField<char*>(getEntityData(entityId), key);}

	virtual bool setField(int entityId,const char* key,int value)=0;
	virtual bool setField(int entityId,const char* key,long value)=0;
	virtual bool setField(int entityId,const char* key,ulong value)=0;
	virtual bool setField(int entityId,const char* key,float value)=0;
	virtual bool setField(int entityId,const char* key,const char* value)=0;
	virtual bool setField(int entityId,const char* key,char* value)=0;
	virtual bool setField(int entityId,const char* key,String value)=0;
#ifdef DEPLOY_TEMPLATES
	virtual String getByTemplateKey(const String& var)=0;
#endif
	virtual int getFieldInt(int entityId,const char* key)=0;
	virtual long getFieldLong(int entityId,const char* key)=0;
	virtual ulong getFieldULong(int entityId,const char* key)=0;
	virtual float getFieldFloat(int entityId,const char* key)=0;
	virtual const char* getFieldConstChar(int entityId,const char* key)=0;
	virtual char* getFieldChar(int entityId,const char* key)=0;

	virtual EventSender* getEventSender()=0;

	virtual void setEventSender(EventSender* eventSender)=0;

	virtual void postDataPosted(int entityId)=0;
};

#endif /* LIBRARIES_ENTITYLIB_ENTITYMODELDATAPROVIDER_H_ */
