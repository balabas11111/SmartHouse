/*
 * EntityManager.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <entity/EntityManager.h>

EntityManager::EntityManager(Entity* items[]) {
	this->entities = std::list<Entity*>(items, items + sizeof(items)/sizeof(Entity*));

	FileUtils::init();
}

std::list<Entity*>& EntityManager::getEntities() {
	return this->entities;
}

int EntityManager::begin(){

	Serial.println(FPSTR("-----Begin EntityManager-----"));
	Serial.print(FPSTR("entities="));
	Serial.print(this->entities.size());

	if(FileUtils::existsAndHasSize(FPSTR(STORAGE_PATH))){
		Serial.print(FPSTR("Load from file"));
		Serial.println(FPSTR(STORAGE_PATH));
		File file=FileUtils::getFile(FPSTR(STORAGE_PATH), "r");
		this->root = this->rootBuf.parse(file);
	}else{
		Serial.println(FPSTR("New data"));
		this->root = this->rootBuf.createObject();

		getRoot().createNestedObject(FPSTR(MODEL_KEY));
		getRoot().createNestedObject(FPSTR(DATA_KEY));
	}

	Serial.print(FPSTR("Root success="));
	Serial.print(root.success());

	//Validate and init Entity descriptors
	initModels();

	return entities.size();
}

JsonObject& EntityManager::getRoot() {
	return this->root;
}

DynamicJsonBuffer& EntityManager::getRootBuf() {
	return this->rootBuf;
}

JsonObject& EntityManager::getModel() {
	return getRoot().get<JsonObject>(FPSTR(MODEL_KEY));
}

JsonObject& EntityManager::getData() {
	return getRoot().get<JsonObject>(FPSTR(DATA_KEY));
}

JsonObject& EntityManager::getEntityModel(String name) {
	return getModel().get<JsonObject>(name);
}

JsonObject& EntityManager::getEntityData(String name) {
	return getData().get<JsonObject>(name);
}

int EntityManager::copyJsonObjects(JsonObject& from, JsonObject& to,bool over) {
	int result=0;

	for(JsonObject::iterator it = from.begin(); it!=from.end(); ++it){
		bool noToKey = !to.containsKey(it->key);
		if(it->value.is<JsonObject>()){

			if(noToKey){
				to.createNestedObject(it->key);
			}
			result +=copyJsonObjects(it->value.asObject(), to.get<JsonObject>(it->key), over);
		}/*else if(it->value.is<JsonArray>()){
			if(noToKey){
				to.createNestedArray(it->key);
			}
			else if(from.size()!=to.size()){
				to.remove(it->key);
				to.createNestedArray(it->key);
			}

			JsonArray& tar = to.get<JsonArray>(it->key);
			JsonArray& far = it->value;

			result+=copyJsonArrays(far, tar, over);
		}*/else{

				if(noToKey || over || to.get<String>(it->key)!=from.get<String>(it->key)){
					to.set(it->key, it->value);
					result++;
				}
			}
	}

	return result;
}

int EntityManager::initModels() {
	Serial.println(FPSTR("Init entity models"));

	for (std::list<Entity*>::iterator it=this->entities.begin(); it != this->entities.end(); ++it){
		Entity* e=*it;

		Serial.print(e->getDao());
		Serial.print(FPSTR("."));
		Serial.print(e->getName());
		Serial.println(FPSTR(" m->"));

		bool modelExists = getModel().containsKey(e->getName());
		bool dataExists  = getModel().containsKey(e->getName());

		if(!modelExists){
			Serial.print(FPSTR(" created"));
			getModel().createNestedObject(e->getName());
			this->tmpBuf.clear();
		}

		this->tmpBuf.clear();
		JsonObject& from=this->tmpBuf.parse(e->getModel());
		JsonObject& target = getEntityModel(e->getName());

		copyJsonObjects(from, target, true);
		this->tmpBuf.clear();

		if(!getData().containsKey(e->getName())){
			getData().createNestedObject(e->getName());
		}

		//check if data pass current model

	}

	return 1;
}
