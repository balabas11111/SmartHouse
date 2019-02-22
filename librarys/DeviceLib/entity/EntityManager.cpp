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
	getRoot().createNestedObject(FPSTR(JSONKEY_web));

	Serial.print(FPSTR("Root success="));
	Serial.print(root.success());

	init();

	return entities.size();
}

JsonObject& EntityManager::getRoot() {
	return this->root;
}
JsonObject& EntityManager::getTmp() {
	return this->tmp;
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
JsonObject& EntityManager::getWebTempl(){
	return getRoot().get<JsonObject>(FPSTR(JSONKEY_web));
}

JsonObject& EntityManager::getTmpModel() {
	return getTmp().get<JsonObject>(FPSTR(JSONKEY_model));
}

JsonObject& EntityManager::getTmpData() {
	return getTmp().get<JsonObject>(FPSTR(JSONKEY_data));
}

int EntityManager::init() {
	int result=0;
	Serial.println(FPSTR("Init entity models"));

	if(FileUtils::existsAndHasSize(FPSTR(ENTITY_MANAGER_STORAGE_FILE_PATH))){
		File store=FileUtils::getFile(FPSTR(ENTITY_MANAGER_STORAGE_FILE_PATH), FILE_MODE_READ);
		tmpBuf.clear();

		tmp=tmpBuf.parse(store);
		entFileExists=tmp.success()
						&& getTmp().containsKey(FPSTR(JSONKEY_model))
						&& getTmp().containsKey(FPSTR(JSONKEY_data));

		Serial.println(FPSTR("Entities file exists"));
	}else{
		Serial.println(FPSTR("Entities file not found. Default"));
	}

	for (std::list<Entity*>::iterator it=this->entities.begin(); it != this->entities.end(); ++it){
		Entity* e=*it;

		initEntity(e);
		loadEntity(e);

		//fields which should be included as placeHolders. Should be saved by directLink into Json by full path key
		prepareTemplateFields(e);
	}

	return result;
}

int EntityManager::initEntity(Entity* entity) {
	//Init Model. Create default models and Data keys in root.
	int result=0;
	const char* daoName= entity->getDao();
	const char* entityName= entity->getName();

	if(!entity->isInit()){
		entity->init();
	}

	Serial.print(FPSTR("init dao="));
	Serial.print(daoName);

	JsonObject& model = getModel();
	JsonObject& data  = getData();
	JsonObject& webTempl  = getWebTempl();

	if(!model.containsKey(FPSTR(daoName))){
		Serial.print(FPSTR(".m"));
		model.createNestedObject(FPSTR(daoName));
	}
	if(!data.containsKey(FPSTR(daoName))){
		Serial.print(FPSTR(".d"));
		data.createNestedObject(FPSTR(daoName));
	}
	if(!webTempl.containsKey(FPSTR(daoName))){
		Serial.print(FPSTR(".t"));
		webTempl.createNestedObject(FPSTR(daoName));
	}

	JsonObject& modelDao = model.get<JsonObject>(FPSTR(daoName));
	JsonObject& dataDao  = data.get<JsonObject>(FPSTR(daoName));
	JsonObject& templDao  = webTempl.get<JsonObject>(FPSTR(daoName));

	if(!modelDao.containsKey(FPSTR(entityName))){
		Serial.print(FPSTR(".m dao"));
		modelDao.createNestedObject(FPSTR(entityName));
	}
	if(!dataDao.containsKey(FPSTR(entityName))){
		Serial.print(FPSTR(".d dao"));
		dataDao.createNestedObject(FPSTR(entityName));
	}
	if(!templDao.containsKey(FPSTR(entityName))){
		Serial.print(FPSTR(".t dao"));
		templDao.createNestedObject(FPSTR(entityName));
	}

	Serial.println(FPSTR("."));

	JsonObject& modelEntity = modelDao.get<JsonObject>(FPSTR(entityName));
	JsonObject& dataEntity  = dataDao.get<JsonObject>(FPSTR(entityName));
	JsonObject& tempEntity  = templDao.get<JsonObject>(FPSTR(entityName));

	Serial.print(FPSTR("m d structure"));

	modelEntity.set(FPSTR(JSONKEY_name),FPSTR(entity->getName()));
	modelEntity.set(FPSTR(JSONKEY_descr),FPSTR(entity->getDescr()));
	modelEntity.set(FPSTR(JSONKEY_class),FPSTR(entity->getClass()));

	dataEntity.set(FPSTR(JSONKEY_name),FPSTR(entity->getName()));
	dataEntity.set(FPSTR(JSONKEY_descr),FPSTR(entity->getDescr()));
	dataEntity.set(FPSTR(JSONKEY_class),FPSTR(entity->getClass()));

	tempEntity.set(FPSTR(JSONKEY_name),FPSTR(entity->getName()));
	tempEntity.set(FPSTR(JSONKEY_descr),FPSTR(entity->getDescr()));
	tempEntity.set(FPSTR(JSONKEY_class),FPSTR(entity->getClass()));

	JsonObject& fieldsM=modelEntity.createNestedObject(FPSTR(JSONKEY_fields));
	JsonObject& varM=modelEntity.createNestedObject(FPSTR(JSONKEY_var));
	JsonObject& tvarM=modelEntity.createNestedObject(FPSTR(JSONKEY_tvar));
	JsonObject& persistM=modelEntity.createNestedObject(FPSTR(JSONKEY_persist));
	JsonObject& defaulTM=modelEntity.createNestedObject(FPSTR(JSONKEY_default));

	JsonObject& dataD=modelEntity.createNestedObject(FPSTR(JSONKEY_fields));

	//fields specified as json
	if(entity->isJson()){
		DynamicJsonBuffer bufJson;

		JsonObject& prototype=bufJson.parse(entity->getJson());
		if(!prototype.success()){
			return 0;
		}

	}

//fields specified as arrays
	if(!entity->isJson() && entity->fields_Size()>0){
		for(uint8_t i=0;i<entity->fields_Size();i++){
			fieldsM.set(FPSTR(entity->fields()[i]),JSONKEY_null);
			dataD.set(FPSTR(entity->fields()[i]),JSONKEY_null);
		}
		for(uint8_t i=0;i<entity->fields_Var_Size();i++){
			varM.set(FPSTR(entity->fields_Var()[i]),JSONKEY_null);
		}
		for(uint8_t i=0;i<entity->fields_Var_Size();i++){
			tvarM.set(FPSTR(entity->fields_Var()[i]),JSONKEY_null);
		}
		for(uint8_t i=0;i<entity->fields_Persist_Size();i++){
			persistM.set(FPSTR(entity->fields_Persist()[i]),JSONKEY_null);
		}
		for(uint8_t i=0;i<entity->fields_WithDefault_Size();i++){
			defaulTM.set(FPSTR(entity->fields_WithDefault()[i]),FPSTR(entity->fields_DefaultValues()[i]));
			dataD.set(FPSTR(entity->fields()[i]),FPSTR(entity->fields_DefaultValues()[i]));
		}

		Serial.println(FPSTR("Entity default="));
		getRoot().prettyPrintTo(Serial);
	}
	return entity->fields_Size();
}

int EntityManager::loadEntity(Entity* entity) {
	//load overrided Entity Model details from temp . Process Model and Entity here
	if(!entFileExists){	return 0;}

	int result = 0;

	const char* daoName= entity->getDao();
	const char* entityName= entity->getName();

	if(!(getTmpModel().containsKey(FPSTR(daoName)) && getTmpData().containsKey(FPSTR(daoName)))){return 0;}
	if(!(getTmpModel().get<JsonObject>(FPSTR(daoName)).containsKey(FPSTR(entityName))
			&& getTmpData().get<JsonObject>(FPSTR(daoName)).containsKey(FPSTR(entityName)))){return 0;}

	Serial.println(FPSTR("Compare base <--> save"));

	JsonObject& modelEntity = getModel().get<JsonObject>(FPSTR(daoName)).get<JsonObject>(FPSTR(entityName));
	JsonObject& dataEntity  = getData().get<JsonObject>(FPSTR(daoName)).get<JsonObject>(FPSTR(entityName));

	JsonObject& modelTmp = getTmpModel().get<JsonObject>(FPSTR(daoName)).get<JsonObject>(FPSTR(entityName));
	JsonObject& dataTmp  = getTmpData().get<JsonObject>(FPSTR(daoName)).get<JsonObject>(FPSTR(entityName));

	result=copyObjFields(modelTmp.get<JsonObject>(JSONKEY_fields),modelEntity.get<JsonObject>(JSONKEY_fields));
	result=copyObjFields(modelTmp.get<JsonObject>(JSONKEY_var),modelEntity.get<JsonObject>(JSONKEY_var));
	result=copyObjFields(modelTmp.get<JsonObject>(JSONKEY_tvar),modelEntity.get<JsonObject>(JSONKEY_tvar));
	result=copyObjFields(modelTmp.get<JsonObject>(JSONKEY_persist),modelEntity.get<JsonObject>(JSONKEY_persist));

	result=copyObjFields(dataTmp.get<JsonObject>(JSONKEY_fields),dataEntity.get<JsonObject>(JSONKEY_fields));

	Serial.print(FPSTR("Entity change="));
	Serial.println(result);
	Serial.print(FPSTR(" root="));

	getRoot().prettyPrintTo(Serial);

	return result;
}

int EntityManager::prepareTemplateFields(Entity* entity) {
	Serial.println(FPSTR("webTemplate"));
	int result=0;
	const char* daoName= entity->getDao();
	const char* entityName= entity->getName();

	JsonObject& webTempl  = getRoot().get<JsonObject>(FPSTR(JSONKEY_web))
								.get<JsonObject>(FPSTR(daoName))
								.get<JsonObject>(FPSTR(entityName));

	JsonObject& model  = getRoot().get<JsonObject>(FPSTR(JSONKEY_model))
									.get<JsonObject>(FPSTR(daoName))
									.get<JsonObject>(FPSTR(entityName));

	JsonObject& data  = getRoot().get<JsonObject>(FPSTR(JSONKEY_data))
										.get<JsonObject>(FPSTR(daoName))
										.get<JsonObject>(FPSTR(entityName));

	JsonObject& fields=data.get<JsonObject>(FPSTR(JSONKEY_fields));
	JsonObject& tvar=model.get<JsonObject>(FPSTR(JSONKEY_tvar));

	if(tvar.size()>0){
		data.createNestedObject(FPSTR(JSONKEY_web));
	}

	webTempl.set(FPSTR(JSONKEY_descr), model.get<const char*>(FPSTR(JSONKEY_descr)));

	JsonObject& dataTmpl=webTempl.createNestedObject(FPSTR(JSONKEY_fields));

	for(JsonObject::iterator it = fields.begin(); it!=fields.end(); ++it){
		if(!tvar.containsKey(it->key)){
			JsonObject& mapping=data.get<JsonObject>(FPSTR(JSONKEY_web));
			mapping.set(it->key, templFields);
			String templVal=JSONKEY_PERCENT+String(templFields)+JSONKEY_PERCENT;

			//const char* templVal=FPSTR("%");
			dataTmpl.set(it->key, templVal);
			result++;
		}else{
			dataTmpl.set(it->key, it->value);
		}
	}

	Serial.print(FPSTR("Entity fields="));
	Serial.println(result);
	Serial.print(FPSTR(" root="));

	getRoot().prettyPrintTo(Serial);

	return result;
}

int EntityManager::copyObjFields(JsonObject& from, JsonObject& to){
	int result=0;

	for(JsonObject::iterator it = from.begin(); it!=from.end(); ++it){
		if(!to.containsKey(it->key)){
			to.set(it->key,it->value);
			result++;
		}
	}

	return result;
}

const char* EntityManager::getProgmemKey(String key){
	//get
	for(uint8_t i=0;i<KEY_SIZE;i++){
		if(strcmp(key.c_str(),JSONKEY_ALL_KEYS[i])==0){
			return JSONKEY_ALL_KEYS[i];
		}
	}

	return key.c_str();
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
