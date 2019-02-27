/*
 * Entity.h
 *
 *  Created on: Feb 20, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_MODEL_BASE_ENTITY_H_
#define LIBRARIES_DEVICELIB_ENTITY_MODEL_BASE_ENTITY_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "EntityConsts.h"

#include "Entity_Descriptor.h"
#include <entity/model/EntityFieldDao.h>
#include <list>

#define HIDDEN_VAlUE_DEFAULT "**********"

class Entity {
public:

	Entity(const char* group,const char* name,const char* descr){
		this->group=group;
		this->name=name;
		this->description=descr;
	};

	virtual ~Entity(){};

	virtual void init(int id,EntityFieldDao* entityFieldDao){
		Serial.print(FPSTR(" id="));
		Serial.println(id);
		this->id=id;
		this->entityFieldDao=entityFieldDao;

	};

	virtual void initModel()=0;

	virtual void initModelDefault(){
		Serial.println(FPSTR("Init template and dataModel Default"));

		registerStatField<int>(Key_id,id);
		registerVariableField<const char*>(Key_descr,description,true);

		registerField_loadable(Key_descr);

		registerField_saveable(Key_id);
		registerField_saveable(Key_descr);
	}

	virtual bool isInit(){
		return this->isinit;
	}

	const char* getName(){	return this->name;	}

	const char* getGroup(){	return this->group;	}

	bool isChanged() const { return changed; }

	void setChanged(bool changed = true) {	this->changed = changed; }

	template<typename T>
	void registerStatField(const char* key,T value){
		EntityField* f = entityFieldDao->registerEntityField(id, key, value, false);
		this->descr.getStatFields().push_back(f->getEntityFieldIndex());
	}

	template<typename T>
	void registerVariableField(const char* key,T value,bool loadable = false,bool saveable = false){
		EntityField* f = entityFieldDao->registerEntityField(id, key, value, true);
		this->descr.getVarFields().push_back(f->getEntityFieldIndex());

		if(loadable){
			this->descr.getLoadFields().push_back(f->getEntityFieldIndex());
		}
		if(saveable){
			this->descr.getSaveFields().push_back(f->getEntityFieldIndex());
		}
	}

	template<typename T>
	EntityFieldHolder<T>* getRegisteredFieldHolderByKeyName(const char* key){
		for (std::list<int>::iterator it = descr.getVarFields().begin(); it != descr.getVarFields().end(); it++){
			EntityField* ef = entityFieldDao->getEntityField((*it));
			if(ef!=NULL && strcmp(ef->getKey(),key)==0){
				EntityFieldHolder<T>* res = static_cast<EntityFieldHolder<T>*>(ef);
				if(res!=NULL){
					return res;
				}
			}
		}

		return NULL;
	}

	EntityField* getRegisteredFieldByKeyName(const char* key){
		for (std::list<int>::iterator it = descr.getVarFields().begin(); it != descr.getVarFields().end(); it++){
			EntityField* ef = entityFieldDao->getEntityField(*it);
			if(strcmp(ef->getKey(),key)==0){
				return ef;
			}
		}

		return NULL;
	}

	void registerField_loadableAllVar(){
		for (std::list<int>::iterator it = descr.getVarFields().begin(); it != descr.getVarFields().end(); it++){
			this->descr.getLoadFields().push_back((*it));
		}
	}

	void registerField_saveableAllVar(){
		for (std::list<int>::iterator it = descr.getVarFields().begin(); it != descr.getVarFields().end(); it++){
			this->descr.getSaveFields().push_back((*it));
		}
	}
	void registerField_setableAllVar(){
		for (std::list<int>::iterator it = descr.getVarFields().begin(); it != descr.getVarFields().end(); it++){
			this->descr.getSetFields().push_back((*it));
		}
	}

	void registerField_templable(const char* key){
		EntityField* f = getRegisteredFieldByKeyName(key);
		if(f!=NULL){
			this->descr.getTemplFields().push_back(f->getEntityFieldIndex());
		}
	}

	void registerField_loadable(const char* key){
		EntityField* f = getRegisteredFieldByKeyName(key);
		if(f!=NULL){
			this->descr.getLoadFields().push_back(f->getEntityFieldIndex());
		}
	}

	void registerField_hidden(const char* key){
		EntityField* f = getRegisteredFieldByKeyName(key);
		if(f!=NULL){
			this->descr.getHiddenFields().push_back(f->getEntityFieldIndex());
		}
	}

	void registerField_saveable(const char* key){
		EntityField* f = getRegisteredFieldByKeyName(key);
		if(f!=NULL){
			this->descr.getSaveFields().push_back(f->getEntityFieldIndex());
		}
	}

	void registerField_setable(const char* key){
		EntityField* f = getRegisteredFieldByKeyName(key);
		if(f!=NULL){
			this->descr.getSetFields().push_back(f->getEntityFieldIndex());
		}
	}

	template<typename T>
	T getVal(const char* key){
		//return entityFieldDao->getValByEntityIdKey<T>(id, key);
		return getRegisteredFieldHolderByKeyName<T>(key)->getValue();
	}

	String getValString(const char* key){
		return entityFieldDao->getValByEntityIdKeyString(id, key);
	}

	template<typename T>
	void setVar(const char* key,T value){
		entityFieldDao->setValByEntityIdKey(id, key, value);
	}

	const Entity_Descriptor& descriptor() const {
		return descr;
	}
/*
	bool isFieldVar(const char* key){
		return entityFieldDao->isExistsEntityFieldByEntityIdKey(id, key);
	}
*/
protected:
		int id=-1;
		const char*  name;
		const char* group;
		const char*  description;

		bool isinit=false;
		bool changed=false;

		EntityFieldDao* entityFieldDao;
		Entity_Descriptor descr;

		/* field name constants*/
		const char* Key_id="id";
		const char* Key_name="name";
		const char* Key_descr="descr";
		const char* Key_group="group";

		const char* Key_light="light";
		const char* Key_temperature="temperature";
		const char* Key_humidity="humidity";
		const char* Key_pressure="pressure";
		const char* Key_coordinate="coordinate";
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_MODEL_BASE_ENTITY_H_ */
