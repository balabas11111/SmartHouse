/*
 * EntityTable.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELDDAO_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYFIELDDAO_H_

#include <list>
#include <functional>

#include <Arduino.h>
#include <ArduinoJson.h>
#include <entity/model/EntityFieldHolder.h>

class EntityFieldDao {
public:

	typedef enum {
	  Class_int, Class_char, Class_float
	} EntityFieldClass;

	EntityFieldDao(){};
	virtual ~EntityFieldDao(){};

	bool isExistsEntityField(int entityFieldIndex){
		if(entityFields.size()>entityFieldIndex){
			return true;
		}
		return false;
	}
	bool isExistsEntityFieldByEntityIdKey(int entityId,const char* key){
		return getEntityFieldByEntityIdKey(entityId,key)!=NULL;
	}

	template<typename T>
	EntityFieldHolder<T>* getEntityFieldHolder(int entityFieldInd){
		return getEntityFieldHolderByEntityFieldId<T>(entityFieldInd);
	}

	EntityField* getEntityField(int entityFieldInd){
		return getEntityFieldByEntityFieldId(entityFieldInd);
	}

	template<typename T>
	EntityField* getField(int entityId,const char* key,bool isChangeAble){
		for (std::list<EntityField*>::iterator it = entityFields.begin(); it != entityFields.end(); it++){
			if(strcmp((*it)->getKey(),key)==0 && (*it)->isChangeAble()==isChangeAble){
				EntityFieldHolder<T>* holder = static_cast<EntityFieldHolder<T>*>(*it);

				if(holder!=NULL && holder->getEntityId() == entityId){
					return *it;
				}
			}
		}

		return NULL;
	}

	template<typename T>
	EntityField* findField(int entityId,const char* key,T value,bool isChangeAble){
		for (std::list<EntityField*>::iterator it = entityFields.begin(); it != entityFields.end(); it++){
			if(strcmp((*it)->getKey(),key)==0 && (*it)->isChangeAble()==isChangeAble){
				EntityFieldHolder<T>* holder = static_cast<EntityFieldHolder<T>*>(*it);

				if(holder!=NULL){
					if((!isChangeAble && holder->compare(value))
							||(isChangeAble && holder->getEntityId() == entityId)){
						return *it;
					}

				}
			}
		}

		return NULL;
	}

	template<typename T>
	EntityField* registerEntityField(int entityId,const char* key,T value,bool isChangeAble){
		EntityField* holder = findField(entityId, key, value, isChangeAble);
		if(holder == NULL){
			holder = createEntityField(entityId, key, value, isChangeAble);
			entityFields.push_back(holder);
		}

		return holder;
	}

	template<typename T>
	bool setValByEntityIdKey(int entityId,const char* key,T value){
		EntityFieldHolder<T>* h = getEntityFieldHolderByEntityIdKey<T>(entityId,key);
		if(!h->isChangeAble()){
			return false;
		}

		return setEntityFieldValue(h,value);
	}

	template<typename T>
	bool setValByEntityFieldIndex(int entityFieldInd,T value){
		EntityFieldHolder<T>* h = getEntityFieldHolderByEntityFieldId<T>(entityFieldInd);

		return setEntityFieldValue(h,value);
	}

	template<typename T>
	T getValByEntityIdKey(int entityId,const char* key){
		EntityFieldHolder<T>* h = getEntityFieldHolderByEntityIdKey<T>(entityId,key);
		return (h!=NULL)?h->getValue():NULL;
	}

	template<typename T>
	T getValByEntityFieldIndex(int entityFieldInd){
		EntityFieldHolder<T>* h = getEntityFieldHolderByEntityFieldId<T>(entityFieldInd);
		return (h!=NULL)?h->getValue():NULL;
	}

	String getValByEntityIdKeyString(int entityId,const char* key){
		EntityField* h = getEntityFieldByEntityIdKey(entityId,key);
		return (h!=NULL)?h->getValStr():String(NULL);
	}

	String getValByEntityFieldIndexString(const char* entityFieldIndStr,bool returnNotFound=true){
		char* p;
		long entityFieldInd = strtol(entityFieldIndStr, &p, 10);
		if (*p) {
		    return FPSTR("Bad index");
		}

		EntityField* h = getEntityFieldByEntityFieldId(entityFieldInd);
		return (h!=NULL)?h->getValStr():((!returnNotFound)?String(NULL):FPSTR("Not found"));
	}

	String getValByEntityFieldIndexString(int entityFieldInd){
		EntityField* h = getEntityFieldByEntityFieldId(entityFieldInd);
		return (h!=NULL)?h->getValStr():String(NULL);
	}

	void print(){
		Serial.println(FPSTR("EntityTable"));
		for (std::list<EntityField*>::iterator it = entityFields.begin(); it != entityFields.end(); it++){
			(*it)->print();
		}

		Serial.println();
	}

	void printEntityField(int entityFieldInd){
		getEntityField(entityFieldInd)->print();
	}

	void setEventTargets(std::function<void(EntityField*)> fieldChangedFunction,std::function<void(int,const char*,const char*)> staticChangedFunction){
		this->fieldChangedFunction=fieldChangedFunction;
		this->staticChangedFunction=staticChangedFunction;
	}

	void dispatchEntityFieldChange(EntityField* field){
		if(this->fieldChangedFunction!=nullptr){
			fieldChangedFunction(field);
		}
	}
	void dispatchEntityStaticChange(int entityId,const char* key,const char* value){
		if(this->fieldChangedFunction!=nullptr){
			staticChangedFunction(entityId,key,value);
		}
	}

protected:
	EntityField* getEntityFieldByEntityFieldId(int entityFieldInd){
		if(!isExistsEntityField(entityFieldInd)){
			return NULL;
		}
		auto h = std::next(entityFields.begin(), entityFieldInd);
		EntityField* holder = static_cast<EntityField*>(*h);

		return holder;
	}

	EntityField* getEntityFieldByEntityIdKey(int entityId,const char* key){
		for (std::list<EntityField*>::iterator it = entityFields.begin(); it != entityFields.end(); it++){
			if((*it)->getEntityId()==entityId && strcmp((*it)->getKey(),key)==0){
				return (*it);
			}
		}
		return NULL;
	}

	template<typename T>
	EntityField* createEntityField(int entityId,const char* key,T value,bool isChangeAble){
		int ind = entityFields.size();

		EntityField* holder = new EntityFieldHolder<T>(ind,entityId,key,value,isChangeAble);
		entityFields.push_back(holder);

		return holder;
	}

	template<typename T>
	EntityFieldHolder<T>* getEntityFieldHolderByEntityFieldId(int entityFieldInd){
		return castEntityFieldHolder<T>(
					getEntityFieldByEntityFieldId(entityFieldInd));
	}

	template<typename T>
	EntityFieldHolder<T>* getEntityFieldHolderByEntityIdKey(int entityId,const char* key){
		return castEntityFieldHolder<T>(
					getEntityFieldByEntityIdKey(entityId,key));
	}

	template<typename T>
	EntityFieldHolder<T>* castEntityFieldHolder(EntityField* f){
		return (f!=NULL)?static_cast<EntityFieldHolder<T>*>(f):NULL;
	}

	template<typename T>
	bool setEntityFieldValue(EntityFieldHolder<T>* h,T value){
		if(h==NULL){
			return false;
		}
		bool changed = h->setValue(value);

		if(changed){
			dispatchEntityFieldChange(h);
		}
		return changed;
	}

	std::function<void(EntityField*)> fieldChangedFunction;
	std::function<void(int,const char*,const char*)> staticChangedFunction;

	std::function<void(const char*,int,const char*,const char*)> updateJsonValue;

	std::list<EntityField*> entityFields;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_ENTITYTABLE_H_ */


