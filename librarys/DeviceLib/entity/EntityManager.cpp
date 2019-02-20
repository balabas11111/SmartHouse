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

	if(FileUtils::existsAndHasSize(FPSTR(STORAGE_PATH))){
		Serial.print(FPSTR("Load from file"));
		Serial.println(FPSTR(STORAGE_PATH));
		File file=FileUtils::getFile(FPSTR(STORAGE_PATH), "r");
		this->root = this->rootBuf.parse(file);
	}else{
		Serial.println(FPSTR("New data"));
		this->root = this->rootBuf.createObject();

		getRoot().createNestedObject(FPSTR(JSONKEY_model));
		getRoot().createNestedObject(FPSTR(JSONKEY_data));
	}

	Serial.print(FPSTR("Root success="));
	Serial.print(root.success());

	//Validate and init Entity descriptors
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

int EntityManager::constructDataModel() {
}

JsonObject& EntityManager::getDaoModel(const char* daoName) {
}

JsonObject& EntityManager::getDaoData(const char* daoName) {
}

JsonObject& EntityManager::getEntityModel_field(const char* daoName,
		const char* entityName, const char* fieldName) {
}

JsonObject& EntityManager::getEntityData_field(const char* daoName,
		const char* entityName) {
}

JsonObject& EntityManager::getEntityModel(const char* daoName,
		const char* entityName) {
}

JsonObject& EntityManager::getEntityData(const char* daoName,
		const char* entityName) {
}
/*
int EntityManager::initModels() {


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
		JsonObject& target = getEntityModel("",e->getName().c_str());

		copyJsonObjects(from, target, true);
		this->tmpBuf.clear();

		if(!getData().containsKey(e->getName())){
			getData().createNestedObject(e->getName());
		}

		//check if data pass current model

	return 1;
}
*/

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
