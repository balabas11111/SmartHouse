/*
 * EntityManager.cpp
 *
 *  Created on: Feb 19, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <entity/EntityManager.h>
#include <entity/EntityConsts.h>

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


	Serial.println(FPSTR("Create memory storage"));
	this->root = this->rootBuf.createObject();

	getRoot().createNestedObject(FPSTR(JSONKEY_model));
	getRoot().createNestedObject(FPSTR(JSONKEY_data));

	Serial.print(FPSTR("Root success="));
	Serial.print(root.success());

	init();

	return entities.size();
}

JsonObject& EntityManager::getRoot() {
	return this->root;
}

DynamicJsonBuffer& EntityManager::getRootBuf() {
	return this->rootBuf;
}

JsonObject& EntityManager::getModel() {
	return getRoot().get<JsonObject>(FPSTR(JSONKEY_model));
}

JsonObject& EntityManager::getData() {
	return getRoot().get<JsonObject>(FPSTR(JSONKEY_data));
}

JsonObject& EntityManager::getDaoModel(const char* daoName) {
	return getModel().get<JsonObject>(daoName);
}

JsonObject& EntityManager::getDaoData(const char* daoName) {
	return getData().get<JsonObject>(daoName);
}

JsonObject& EntityManager::getEntityModel(const char* daoName,const char* entityName) {
	return getDaoModel(daoName).get<JsonObject>(entityName);
}

JsonObject& EntityManager::getEntityData(const char* daoName,const char* entityName) {
	return getDaoData(daoName).get<JsonObject>(entityName);
}

JsonObject& EntityManager::getEntityModel_node(const char* daoName,const char* entityName, const char* nodeName) {
	return getEntityModel(daoName, entityName).get<JsonObject>(nodeName);
}

JsonObject& EntityManager::getEntityData_node(const char* daoName,const char* entityName, const char* nodeName) {
	return getEntityData(daoName, entityName).get<JsonObject>(nodeName);
}

int EntityManager::init() {
	int result=0;
	Serial.println(FPSTR("Init entity models"));

	for (std::list<Entity*>::iterator it=this->entities.begin(); it != this->entities.end(); ++it){
		Entity* e=*it;

		//Init Model. Create models and Data keys in root.
		initEntitysModel(e);

		//load overrided Entity Model details from file. Process Model and Entity here
		loadEntitysModel(e);

		//load data values from file
		loadEntitysData(e);

		//fields which should be included as placeHolders. Should be saved by directLink into Json by full path key
		prepareTemplateFields(e);
	}

	return result;
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
