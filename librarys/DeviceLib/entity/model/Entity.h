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
#include <entity/model/EntityTable.h>
#include <entity/model/EntityField.h>
/*
const char* ENTITYKEY_ID         = "id";
const char* ENTITYKEY_NAME       = "name";
const char* ENTITYKEY_DESCR      = "descr";
const char* ENTITYKEY_GROUP      = "group";
*/
class Entity {
public:

	Entity(const char* group,const char* name,const char* descr){
		this->group=group;
		this->name=name;
		this->descr=descr;
	};

	virtual ~Entity(){};

	virtual void init(int id,EntityTable* table){
		Serial.print(FPSTR(" id="));
		Serial.println(id);
		this->id=id;
		this->table=table;
	};

	virtual int init_Default_fields(JsonObject& entityModel,JsonObject& entityTemplate){
		Serial.println(FPSTR("Init template and dataModel Default"));

		registerStaticField(entityModel,entityTemplate,Key_id,id);
		registerVarBase<const char*>(entityModel,entityTemplate,Key_descr,descr);

		return box.size();
	}

	virtual int save_Default_fields(JsonObject& entityTemplate, JsonObject& saveModel){
		addStaticField(entityTemplate, saveModel, Key_id);
		addVariableField(entityTemplate, saveModel, Key_descr);

		return box.size();
	}

	virtual bool isInit(){
		return this->isinit;
	}

	const char* getName(){
		return this->name;
	}

	const char* getGroup(){
		return this->group;
	}

	template<typename T>
	void setEntityVarValue(JsonObject& obj,const char* key,T value){
		obj.set(key, value);
	}

	template<typename T>
	int registerStaticField(JsonObject& entityModel,JsonObject& entityTemplate,const char* key,T value){
		entityTemplate.set<T>(key, value);
		entityModel.set<T>(key, value);
		return 0;
	}


	template<typename T>
	int registerVarBase(JsonObject& entityModel,JsonObject& entityTemplate,const char* key,T value){
		int ind = table->registerEntityField(id, value);
		table->setValByEntityId(ind, value);

		EntityField* f=new EntityField(key,ind);
		box.push_back(f);

		String tmplParamKey=getTemplateKey(ind);

		entityModel.set(key, value);
		entityTemplate.set<String>(key, tmplParamKey);

		f->print();
		Serial.print(FPSTR(" ; getVal="));
		Serial.println(getVal<T>(key));


		return 1;
	}

	template<typename T>
	T getVal(const char* key){

		int ind = getIndexByKeyName(key);
		if(ind==-1){
			return NULL;
		}

		return table->getValByEntityId<T>(ind);
	}

	String getValStr(const char* key){

		int ind = getIndexByKeyName(key);
		if(ind==-1){
			return String(NULL);
		}

		return table->getTemplateValueByEntityId(ind);
	}

	template<typename T>
	void setVar(const char* key,T value){

		int ind = getIndexByKeyName(key);
		if(ind==-1){
			return;
		}

		if(getVal<T>(key)!=value){
			changed=true;
		}
		if(changed){
			table->setValByEntityId(ind, value);
		}
	}

	String getTemplateKey(int value){
		return String("%")+String(value)+String("%");
	}
	//get value to insert into deployed template
	int getIndexByKeyName(const char* key){
		for (std::list<EntityField*>::iterator it = box.begin(); it != box.end(); it++){
			EntityField* f =*it;
			if(strcmp(f->getKey(),key)==0){
				return f->getEntityIndexId();
			}
		}
		Serial.println();

		return -1;
	}

	bool isFieldVar(const char* key){
		return getIndexByKeyName(key)>-1;
	}


	std::list<EntityField*>* getBox(){
		return &box;
	}

	bool isChanged() const {
		return changed;
	}

	void setChanged(bool changed = false) {
		this->changed = changed;
	}

	virtual void init_fields(JsonObject& entityModel,JsonObject& entityTemplate){

	}

	virtual void prepare_fields_load(JsonObject& loadModel, JsonObject& entityTemplate){

	}
	virtual void prepare_fields_save(JsonObject& entityTemplate, JsonObject& saveModel){

	}
	virtual void prepare_fields_set(JsonObject& setTemplate, JsonObject& entityModel){

	}


	int getVarCount(){
		return box.size();
	}

	virtual int addStaticField(JsonObject& from,JsonObject& to,const char* key){
		if(from.containsKey(key) && !isFieldVar(key)){
			if(from.is<int>(key)){
				int val = from.get<int>(key);
				to.set(key, val);

				Serial.print(FPSTR(" key="));
				Serial.print(key);
				Serial.print(FPSTR(" val="));
				Serial.println(val);

			}else if(from.is<const char*>(key)){
					const char* val = from.get<const char*>(key);
					to.set(key, val);

					Serial.print(FPSTR(" key="));
					Serial.print(key);
					Serial.print(FPSTR(" val="));
					Serial.println(val);

				}else{
					String val = from.get<String>(key);
					to.set(key, val);

					Serial.print(FPSTR(" key="));
					Serial.print(key);
					Serial.print(FPSTR(" val="));
					Serial.println(val);
				}
		}else{
			Serial.print(FPSTR(" ignored key="));
			Serial.println(key);
		}

		return 1;
	}

	virtual int addVariableField(JsonObject& from,JsonObject& to,const char* key){
		if(from.containsKey(key) && isFieldVar(key)){
			if(from.is<int>(key)){
				int val = getVal<int>(key);
				to.set(key, val);
			}else if(from.is<const char*>(key)){
				const char* val = getVal<const char*>(key);
				to.set(key, val);
			}else{
				String val=getValStr(key);
				to.set(key, val);
			}
		}
		return 1;
	}
	/*
	virtual int tempWeb_Json(JsonObject& obj)=0;

	virtual int save_Json(JsonObject& obj)=0;
	virtual int load_Json(JsonObject& obj)=0;

	virtual int get_Json(JsonObject& obj)=0;
*/
	protected:
		boolean inMemory=true;
		int id=-1;
		const char*  name;
		const char* group;
		const char*  descr;

		bool isinit=false;
		bool hasJson=false;
		bool changed=false;

		EntityTable* table;

		std::list<EntityField*> box;

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
