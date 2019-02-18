/*
 * StartModelAble.h
 *
 *  Created on: 17 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_MODELABLE_H_
#define LIBRARIES_DEVICELIB_INTERFACES_MODELABLE_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <utils/HashUtils.h>

#include "interfaces/Nameable.h"

#include <utils/FileUtils.h>

const PROGMEM char MODEL_TAG[] = "mod:";
const PROGMEM char DESCRIPTOR_TAG[] = "descr:";

const PROGMEM char NAME_TAG[]  = "name";
const PROGMEM char HASH_TAG[]  = "hash";

const PROGMEM char PROVIDE_TAG[] = "provide";
const PROGMEM char HOLDER_TAG[]  = "holder";
const PROGMEM char PERSIST_TAG[]  = "persist";
	const PROGMEM char SET_TAG[]  = "set";
	const PROGMEM char PUT_TAG[]  = "put";
	const PROGMEM char LOAD_TAG[] = "load";
	const PROGMEM char SAVE_TAG[] = "save";
	const PROGMEM char CACHE_TAG[] = "cache";
	const PROGMEM char DICT_TAG[] = "dict";
	const PROGMEM char DESCR_TAG[] = "descr";
	const PROGMEM char INIT_TAG[] = "init";
const PROGMEM char COMPARE_TAG[]  = "compare";
const PROGMEM char DEFAULT_TAG[]  = "default";
const PROGMEM char FIELDS_TAG[]  = "fields";
const PROGMEM char LIST_TAG[]  = "list";
const PROGMEM char PATH_TAG[]  = "path";
const PROGMEM char MODEL_TAG[]  = "model";

const PROGMEM char DEFAULT_MODEL_JSON[] = "{\"mod:\":{}}";
const PROGMEM char DEFAULT_DESCRIPTOR_JSON[] = "{\
\"name\": \"\",\
\"hash\": \"\",\
\"descr\": \"Default Model\",\
\"path\": \"\",\ 
\"class\": \"ModelAble\",\
       \"hashInfo\": { \"keys\":{\"id\":\"\",\"name\":\"\"},\"val\": \"hash\"},\
	   \"provide\": {\"thingSpeakService\":\"\",\"timeIntervalService\": \"\"},\
	   \"holder\": {\"keys\": {\"id\":\"\",\"name\":\"\"}},\
       \"persist\": {\
   				\"set\": {\"descr\":\"\"},\
   				\"put\": {\"val\":\"\"},\
   				\"load\": {\"descr\": \"\"},\
   				\"save\": {\"id\": \"\",\"descr\": \"\"},\
   				\"cache\": {\"id\": \"\",\"name\": \"\",\"descr\": \"\"},\
   				\"dict\": {\"descr\": \"name\"},\
   				\"descr\": {\"save\": \"1\", \"load\": \"1\", \"cache\": \"0\", \"init\": \"0\"}\
   			},\
   \"compare\": {\"id\":\"\", \"name\":\"\", \"hash\":\"\" },\
   \"dict\": {\"descr\": \"name\"},\
   \"default\": {\"descr\": \"defaultDescr\", \"name\": \"\"},\
   \"fields\": {\
				\"id\": \"int\",\
				\"name\": \"String\",\
				\"descr\": \"String\",\
				\"val\": \"float\",\
				\"hash\": \"String\"\
			  },\
	\"render\": {},\
	\"list\": {}\
  }\
 ";

const PROGMEM char DEFAULT_EMPTY_JSON[] = "{}";

const PROGMEM char DEFAULT_PATH_TAG_VAL[]  = "/services/sensors/";

class ModelAble: public Nameable {
public:
	ModelAble(String name){
		this->name = name;
		this->model=nullptr;
		this->descriptor=nullptr;
		this->started=false;
	};
	virtual ~ModelAble(){};

	//current model path, current descriptor
	int startModel(JsonObject& model,JsonObject& descriptor){
		this->model=model;
		this->descriptor=descriptor;

		JsonObject& fields=descriptor.get<JsonObject>(FPSTR(FIELDS_TAG));

		if(!model.containsKey(FPSTR(FIELDS_TAG))){
			model.createNestedObject(FPSTR(FIELDS_TAG));
		}

		JsonObject& modelFields = descriptor.get<JsonObject>(FPSTR(FIELDS_TAG));

		JsonObject::iterator it;

		for (it = fields.begin(); it != fields.end(); ++it ){
			if(!modelFields.containsKey(it->key)){

			}
		}

		return 1;
	}

	boolean hasDescriptor(){
		return this->descriptor!=nullptr
				&& this->descriptor->success();
	}

	boolean hasModel(){
		return this->model!=nullptr
				&& this->model->success();
	}

	JsonObject& getDescriptor(){
		return this->holder->get<JsonObject>(FPSTR(DESCRIPTOR_TAG));
	}

	JsonObject& getModel(){
		return this->holder->get<JsonObject>(FPSTR(MODEL_TAG));
	}

	boolean cache(){

	}

	boolean save(){
		DynamicJsonBuffer buf;
		JsonObject& tmp = buf.parse(FPSTR(DEFAULT_EMPTY_JSON));


		return true;
	}

	boolean load(){

	}

	boolean set(JsonObject& json){

	}

	boolean put(JsonObject& json){

	}

	boolean dict(){

	}

	boolean updateDict(){

	}

	bool isStarted(){
		return this->started;
	}

protected:
	JsonObject* model;
	JsonObject* descriptor;

	bool buildModelByDescriptor(JsonObject& holder){
		if(!hasDescriptor()){
			return false;
		}

	}

	bool buildDescr(JsonObject& d){
		if(!d.success() || !hasDescriptor()){
			JsonObject& tmp = holder->createNestedObject(FPSTR(DESCRIPTOR_TAG));


			//JsonObject& from =
		}

		String descrHash = d.get<String>(FPSTR(NAME_TAG))
							+d.get<String>(FPSTR(PROVIDE_TAG))
							+d.get<String>(FPSTR(DESCR_TAG))
							+d.get<String>(FPSTR(PERSIST_TAG))
							+d.get<String>(FPSTR(FIELDS_TAG));

		if(!d.containsKey(FPSTR(HASH_TAG)) || d.get<String>(FPSTR(HASH_TAG))!=descrHash){
			d.set(FPSTR(HASH_TAG), descrHash );
		}

		if(d.containsKey(FPSTR(NAME_TAG)) && d.get<String>(FPSTR(NAME_TAG))!=""){
			this->name=d.get<String>(FPSTR(NAME_TAG));
		}

		if(!d.containsKey(FPSTR(PATH_TAG))){
			String path = FPSTR(DEFAULT_PATH_TAG_VAL);
				path+=getName()+"/";
				path+="/"+descrHash+"/";
			d.set(FPSTR(PATH_TAG),path);
		}

		return true;
	}

	bool saveDescriptor(JsonObject& descr){

	}

	bool copyJsonObject(JsonObject& from,JsonObject& to,String withSetCheck){
		bool notChanged=true;

		JsonObject::iterator it;
		for (it = from.begin(); it != from.end(); ++it ){
		  String key=String(it->key);

		  if(from.is<JsonObject,String>(key)){
			  //copy Object
			  JsonObject& newFrom=from.get<JsonObject>(key);

			  if(!to.containsKey(key)){
				  to.createNestedObject(key);
			  }

			  JsonObject& newTo=to.get<JsonObject>(key);

			  notChanged = copyJsonObject(newFrom, newTo,withSetCheck) && notChanged;
		  }else{
			  //copy field
			  if(isFieldTrue(key,withSetCheck)){
				  if(to.containsKey(key)){
					 notChanged = to[key]==it->value;
				  }

				  to.set(key, it->value);
			  }
		  }
		}

		return !notChanged;
	}

	bool isFieldTrue(String descrFieldName,String withCheckKey){
		return getDescriptor().get<JsonObject>("fields")
				.get<JsonObject>(descrFieldName).get<bool>(withCheckKey);
	}

	bool started;

	DynamicJsonBuffer buf;
};

#endif /* LIBRARIES_DEVICELIB_INTERFACES_MODELABLE_H_ */
