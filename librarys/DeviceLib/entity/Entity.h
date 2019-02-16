/*
 * IdentifiAbleNameAble.h
 *
 *  Created on: 15 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_INTERFACES_ENTITY_H_
#define LIBRARIES_DEVICELIB_INTERFACES_ENTITY_H_

#include  <Arduino.h>
#include <ArduinoJson.h>

#include  <interfaces/ChangeDispatchable.h>
#include  <interfaces/DescribeAble.h>
#include  <interfaces/HashAble.h>
#include  <interfaces/Identifiable.h>
#include  <interfaces/Nameable.h>
#include  <interfaces/SizeAble.h>
#include  <interfaces/TypeAble.h>

enum FIELD_CLASS: uint8_t {
	fieldCLass_int, fieldCLass_String, fieldCLass_float
};

const char DEFAULT_ENTITY_DESCRIPTOR[]    PROGMEM ="{\
\"object\": { \"id\": {\"val\": \"0\",\"type\": \"0\", \"otm\": \"0\", \"mth\": \"0\", \"htm\": \"0\"},\
 \"name\": {\"val\": \"DefaultName\",  \"type\": \"0\", \"otm\": \"0\", \"mth\": \"0\", \"htm\": \"0\"},\
 \"hash\": {\"val\": \"DefaultHash\",  \"type\": \"1\", \"otm\": \"0\", \"mth\": \"0\", \"htm\": \"0\"},\
 \"type\": {\"val\": \"DefaultType\",  \"type\": \"1\", \"otm\": \"0\", \"mth\": \"0\", \"htm\": \"0\"},\
 \"size\": {\"val\": \"DefaultSize\",  \"type\": \"1\", \"otm\": \"0\", \"mth\": \"0\", \"htm\": \"0\"},\
 \"descr\": {\"val\": \"DefaultDescr\", \"type\": \"1\", \"otm\": \"1\", \"mth\": \"0\", \"htm\": \"0\"}},\
 \"descriptor\": {\"parentPath\": \"DefaultParentPath\"}"
"}";

class Entity: public DescribeAble, public Identifiable, public Nameable, public HashAble, public SizeAble, public TypeAble, public ChangeDispatchable {
public:
	Entity(JsonObject& objectIn,JsonObject& descriptorIn){
		 this->setObject(&objectIn);
		 this->setDescriptor(&descriptorIn);
	};
//----------------------------------------------------------------
	virtual boolean setHardFromModelField(String key){

		return false;
	}

	virtual boolean setModelFieldFromHard(String key){

		return false;
	}

	int begin(){
		int result=0;

		  JsonObject::iterator it;

		  JsonObject& descriptorInObjPart=this->getDescriptor()->get<JsonObject>("object");
		  JsonObject& descriptorInDescrPart=this->getDescriptor()->get<JsonObject>("descriptor");

		  for (it = descriptorInObjPart.begin(); it != descriptorInObjPart.end(); ++it ){

			  String key=it->key;
			  JsonObject& fed=it->value;

			  object->set(key, fed["val"]);

			  result++;
		  }

		String parentPath = "";

		String path=parentPath+"/"+getName();
		String hashStr=HashAble::constructHash(parentPath+String(getId())+getName());

		descriptor->set("path",path);
		descriptor->set("hash",hashStr);

		object->set("path",path);
		object->set("hash",hashStr);
		object->set("id",id);

		return result;
	}
//----------------------------------------------------------------
	virtual uint8_t getId() override{
		return this->getFieldByKey("id").toInt();
	}

	virtual String getName() override{
		return this->getFieldByKey("name");
	}

	virtual String getHash() override{
		return this->getFieldByKey("hash");
	}

	virtual String getType() override{
		return this->getFieldByKey("type");
	}

	virtual String getSize() override{
		return this->getFieldByKey("size");
	}

	virtual String getDescr() override{
		return this->getFieldByKey("descr");
	}

	String getPath(){
		return this->getFieldByKey("path");
	}

	String getFieldByKey(String key){
		if(this->getObject()->containsKey(key)){
			return this->getObject()->get<String>(key);
		}

		return String(NULL);
	}

	boolean setObjectFieldByKey(String key,String value){
		if((this->getObject()->containsKey(key) && this->getObject()->get<String>(key)!=value) || !this->getObject()->containsKey(key)){
			this->getObject()->set(key,value);

			this->setChanged(true);
			return true;
		}

		return false;
	}

	void print(){
		Serial.println(FPSTR("----------------------"));

		object->printTo(Serial);

		Serial.println();

		descriptor->printTo(Serial);

		Serial.println(FPSTR("----------------------"));
	}

	virtual int objectToModel(JsonObject& obj){
		int result=0;

		JsonObject::iterator it;
		  for (it = this->getDescriptor()->begin(); it != this->getDescriptor()->end(); ++it ){
			  String key=it->key;

			  if(getObjectToModelSetKeyAllowed(key)){
				  result+=setObjectFieldByKey(key, it->value);
			  }
		  }

		this->setChanged(result>0);

		return result;
	}


	//gets values from hardware puts to model
	virtual int hardToModel(){
	  uint8_t result=0;

	  JsonObject::iterator it;
	  for (it = this->getObject()->begin(); it != this->getObject()->end(); ++it ){
		  String key=it->key;

		  if(getModelToHardSetKeyAllowed(key)){
			  result+=setHardFromModelField(key);
		  }
	  }

	  this->setChanged(result>0);

	  return result;
	}

	//puts values from model to hardware
	virtual int modelToHard(JsonObject& obj){
		uint8_t result=0;

		  JsonObject::iterator it;
		  for (it = this->getObject()->begin(); it != this->getObject()->end(); ++it ){
			  String key=it->key;

			  if(getHardToModelSetKeyAllowed(key)){
				  result+=setModelFieldFromHard(key);
			  }
		  }

		  this->setChanged(result>0);

		  return result;
	}


	boolean getObjectToModelSetKeyAllowed(String keyName){
		return (descriptor->get<JsonObject>(keyName)).get<boolean>("otm");
	}

	boolean getModelToHardSetKeyAllowed(String keyName){
		return (descriptor->get<JsonObject>(keyName)).get<boolean>("mth");
	}

	boolean getHardToModelSetKeyAllowed(String keyName){
		return (descriptor->get<JsonObject>(keyName)).get<boolean>("htm");
	}

	boolean satisfyDescriptor(JsonObject& obj){
	  JsonObject::iterator it;

	  for (it = descriptor->begin(); it != descriptor->end(); ++it ){
		  if(!obj.containsKey(it->key)){
			  return false;
		  }
	  }
		return true;
	}

	boolean compareObjects(JsonObject& model,JsonObject& obj){
	 JsonObject::iterator it;
	  for (it = model.begin(); it != model.end(); ++it ){
		  String key=String(it->key);
		  String value=String(it->value.as<char*>());

		  if(!obj.containsKey(key)){
			  return false;
		  }

		  if(obj[key]!=value){
			  return false;
		  }
	  }

	  for (it = obj.begin(); it != obj.end(); ++it ){
		  String key=String(it->key);
		  String value=String(it->value.as<char*>());

		  if(!model.containsKey(key)){
			  return false;
		  }

		  if(model[key]!=value){
			  return false;
		  }
	  }

	  return true;
	}

	boolean compareByBaseFields(JsonObject& model,JsonObject& obj){
		return compareObjectsByKey(model, obj, "id")
			&& compareObjectsByKey(model, obj, "name")
			&& compareObjectsByKey(model, obj, "hash")
			&& compareObjectsByKey(model, obj, "type")
			&& compareObjectsByKey(model, obj, "size")
			&& compareObjectsByKey(model, obj, "descr");
	}

	boolean compareObjectsByKey(JsonObject& model,JsonObject& obj,String key){
		if(!model.containsKey(key) || !obj.containsKey(key)){
			return false;
		}

		if(model[key] == obj[key]){
			return true;
		}

		return false;
	}

	//takes some object a.e. income object and puts it's values to model
protected:
	JsonObject* object;
	JsonObject* descriptor;

	String descrText;

	JsonObject* getObject(){
		return this->object;
	}

	JsonObject* getDescriptor(){
		return this->getDescriptor();
	}

	void setObject(JsonObject* obj){
		this->object=obj;
	}

	void setDescriptor(JsonObject* descriptor){
		this->descriptor=descriptor;
	}

	int setFieldIifDiffers(JsonObject& to,JsonObject& from){
		int result=0;

		 JsonObject::iterator it;

		 for (it = from.begin(); it != from.end(); ++it ){
			  String key=String(it->key);

			  if(!to.containsKey(key) || to[key]!=from[key]){
				  to[key]=from[key];
				  result++;
			  }
     	 }

		 return result;
	}


	int setFieldIifDiffers(JsonObject& to,JsonObject& from,String key){
		int result=0;

		if(from.containsKey(key)){
			result+=setFieldIifDiffers(to,from[key],key);
		}

		return result;
	}

	int setEntityFieldIifDiffers(JsonObject& to,String value,String key){
		int result=0;

		if(to.containsKey(key)){
			if(to.set(key, value)){
				result++;
			}
		}else{
			result++;
		}

		if(result>0){
			to[key]=value;
		}

		return result;
	}

	int setEntityFieldIifDiffers(JsonObject& to,int value,String key){
		int result=0;

		if(to.containsKey(key)){
			if(to.set(key, value)){
				result++;
			}
		}else{
			result++;
		}

		if(result>0){
			to[key]=value;
		}

		return result;
	}

	int setEntityFieldIifDiffers(JsonObject& to,float value,String key){
			int result=0;

			if(to.containsKey(key)){
				if(to.set(key, value)){
					result++;
				}
			}else{
				result++;
			}

			if(result>0){
				to[key]=value;
			}

			return result;
		}

};


#endif /* LIBRARIES_DEVICELIB_INTERFACES_IDENTIFIABLENAMEABLE_H_ */
