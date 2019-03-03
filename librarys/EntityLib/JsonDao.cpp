/*
 * JsonDao.cpp
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: Vitaliy
 */
#include <Arduino.h>
#include <JsonDao.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <FileUtils.h>

JsonDao::JsonDao(EntityJson* entitiesIn[],int count) {
	for(int i=0;i<count;i++){
		this->entities.push_back(entitiesIn[i]);
	}
}

void JsonDao::init() {
	Serial.println(FPSTR("JsonDao init"));
	Serial.print(FPSTR("entities = "));
	Serial.println(this->entities.size());
	Serial.println(FPSTR("JsonDao init Model"));
	Serial.println(FPSTR("------------------------------------------"));

	registerModels();
	loadEntityModelFile();
	initDataByModel();
	initTemplateByModel();

	Serial.println(FPSTR("JsonDao init DONE"));
	Serial.println(FPSTR("------------------------------------------"));
	JsonObjectUtil::print("ROOT=", root);
}

std::list<EntityJson*>* JsonDao::getEntities() {
	return &this->entities;
}

JsonObject& JsonDao::getRoot() {
	return this->root;
}

void JsonDao::createEntityJson(const char* rootPath, EntityJson* ent) {
	getObjectOrCreate(root,rootPath);
	getObjectOrCreate(root.get<JsonObject>(rootPath),ent->getGroup());
	getObjectOrCreate(root.get<JsonObject>(rootPath).get<JsonObject>(ent->getGroup()),ent->getName());
}

JsonObject& JsonDao::getRootPath(const char* rootPath) {
	return root.get<JsonObject>(rootPath);
}

JsonObject& JsonDao::getRootPathGroup(const char* rootPath, const char* group) {
	return root.get<JsonObject>(rootPath).get<JsonObject>(group);
}

JsonObject& JsonDao::getRootPathGroupEntity(const char* rootPath,const char* group, const char* entName) {
	return root.get<JsonObject>(rootPath).get<JsonObject>(group).get<JsonObject>(entName);
}

JsonObject& JsonDao::getObjectOrCreateChar(JsonObject& parent, char* key) {
	if(!parent.containsKey(key)){
		parent.createNestedObject(key);
	}

	return parent.get<JsonObject>(key);
}

JsonArray& JsonDao::getArrayOrCreate(JsonObject& parent, const char* key) {
	if(!parent.containsKey(key)){
			parent.createNestedArray(key);
		}

		return parent.get<JsonArray>(key);
}

EntityJson* JsonDao::getEntity(int i) {
	auto h = std::next(entities.begin(), i);
	EntityJson* entity = static_cast<EntityJson*>(*h);
	return entity;
}

JsonObject& JsonDao::getObjectOrCreate(JsonObject& parent, const char* key) {
	if(!parent.containsKey(key)){
		parent.createNestedObject(key);
	}

	return parent.get<JsonObject>(key);
}

JsonObject& JsonDao::getObjectOrCreateString(JsonObject& parent, String key) {
	if(!parent.containsKey(key)){
		parent.createNestedObject(key);
	}

	return parent.get<JsonObject>(key);
}

void JsonDao::addFieldToActions(JsonObject& actions, const char* kind, const char* key) {
	JsonArray& currArr = !actions.containsKey(kind)?
			actions.createNestedArray(kind):
			actions.get<JsonArray>(kind);
		currArr.add(key);
}

bool JsonDao::isDefaultField(const char* key) {
	return (strcmp(key,JSONKEY_id)==0 || strcmp(key,JSONKEY_name)==0 || strcmp(key,JSONKEY_group)==0);
}

void JsonDao::registerModels(){
	int id=0;

	for(int i=0;i<ROOT_PATHS_TOTAL;i++){
		bool isModel = strcmp(ROOT_PATHS[i],ROOT_PATH_MODEL)==0;

		getObjectOrCreate(root,ROOT_PATHS[i]);

		for(int j=0;j<ROOT_PATHS_GROUPS_TOTAL;j++){
			getObjectOrCreate(root.get<JsonObject>(ROOT_PATHS[i]),ROOT_PATHS_GROUPS[j]);

			for (std::list<EntityJson*>::iterator it = entities.begin(); it != entities.end(); it++){
				if(MathUtils::compareChar((*it)->getGroup(), ROOT_PATHS_GROUPS[j])){
					getObjectOrCreate(root.get<JsonObject>(ROOT_PATHS[i]).get<JsonObject>(ROOT_PATHS_GROUPS[j]),(*it)->getName());

					if(isModel){
						(*it)->setId(id);
						id++;
						registerEntityModel((*it));
						Serial.println(FPSTR("------------------------------------------"));
					}
				}
			}
	}
}
	Serial.print(FPSTR("Total model ="));
	Serial.print(id+1);

	JsonObjectUtil::print("ROOT=", root);
}

void JsonDao::registerEntityModel(EntityJson* ent) {
	Serial.print(FPSTR("Register Entity model"));
	Serial.println(ent->getName());

	createEntityJson(ROOT_PATH_MODEL,ent);
	JsonObject& json = root.get<JsonObject>(ROOT_PATH_MODEL).get<JsonObject>(ent->getGroup()).get<JsonObject>(ent->getName());

	DynamicJsonBuffer b;

	JsonObject& modelFields = (json.containsKey(JSONKEY_fields))?json.get<JsonObject>(JSONKEY_fields):json.createNestedObject(JSONKEY_fields);
	JsonObject& modelActions = (json.containsKey(JSONKEY_actions))?json.get<JsonObject>(JSONKEY_actions):json.createNestedObject(JSONKEY_actions);

	modelFields.set(JSONKEY_id,ent->getId());
	modelFields.set(JSONKEY_group,ent->getGroup());
	modelFields.set(JSONKEY_name,ent->getName());
	modelFields.set(JSONKEY_descr,ent->getDescr());

	for (const auto& kvp : b.parse(ent->getDescriptor()).as<JsonObject>().get<JsonObject>(JSONKEY_fields)) {
		if(!isDefaultField(kvp.key)){
			modelFields.set(strdup(kvp.key), strdup(kvp.value));
		}
	}

	addFieldToActions(modelActions,JSONKEY_stat,JSONKEY_id);
	addFieldToActions(modelActions,JSONKEY_stat,JSONKEY_name);
	addFieldToActions(modelActions,JSONKEY_stat,JSONKEY_group);

	addFieldToActions(modelActions,JSONKEY_var,JSONKEY_descr);
	addFieldToActions(modelActions,JSONKEY_load,JSONKEY_descr);
	addFieldToActions(modelActions,JSONKEY_save,JSONKEY_descr);

	for (const auto& kvp : b.parse(ent->getDescriptor()).as<JsonObject>().get<JsonObject>(JSONKEY_actions)) {
		JsonArray& currArr =
		(!modelActions.containsKey(kvp.key))?
			modelActions.createNestedArray(strdup(kvp.key)):
			modelActions.get<JsonArray>(kvp.key);

		JsonObjectUtil::copyArray(kvp.value, currArr);
	}

	JsonObjectUtil::print("model =",json);
	b.clear();
}

void JsonDao::loadEntityModelFile() {
	if(!FileUtils::existsAndHasSizeChar(PATH_MODEL_file)){
		saveEntityModelFile();
	}else{
		bufTmp.clear();
		File f =FileUtils::getFile(PATH_MODEL_file, FILE_MODE_READ);

		JsonObject& loadedModel = bufTmp.parse(f).as<JsonObject>();

		int t=0;
		for (std::list<EntityJson*>::iterator it = entities.begin(); it != entities.end(); it++){
			t+=loadEntityModel((*it),loadedModel);
		}

		f.close();
		bufTmp.clear();
	}

	JsonObjectUtil::print("ROOT=", root);
}

void JsonDao::saveEntityModelFile() {
	Serial.print(FPSTR("Saving model..."));
	bool res=FileUtils::saveJsonToFile(PATH_MODEL_file, root.get<JsonObject>(ROOT_PATH_MODEL));
	Serial.print(FPSTR(" status="));
	Serial.println(res);

	JsonObjectUtil::print("ROOT=", root);
}

int JsonDao::loadEntityModel(EntityJson* ent,JsonObject& loadedModel) {
	Serial.print(FPSTR("Load Entity "));
	Serial.print(ent->getName());
	int i=0;
	JsonObject& modelFields = root.get<JsonObject>(ROOT_PATH_MODEL).get<JsonObject>(ent->getGroup()).get<JsonObject>(ent->getName()).get<JsonObject>(JSONKEY_fields);
	JsonObject& modelActions = root.get<JsonObject>(ROOT_PATH_MODEL).get<JsonObject>(ent->getGroup()).get<JsonObject>(ent->getName()).get<JsonObject>(JSONKEY_actions);
	JsonObject& loadedFields = loadedModel.get<JsonObject>(ent->getGroup()).get<JsonObject>(ent->getName()).get<JsonObject>(JSONKEY_fields);
	JsonObject& loadedActions = loadedModel.get<JsonObject>(ent->getGroup()).get<JsonObject>(ent->getName()).get<JsonObject>(JSONKEY_actions);

	for (const auto& kvp : loadedFields) {
		if(!isDefaultField(kvp.key) && strcmp(kvp.value,modelFields.get<const char*>(kvp.key))!=0){
			modelFields.set(strdup(kvp.key), strdup(kvp.value));
			i++;
		}
	}
	for (const auto& kvp : loadedActions) {
		JsonArray& currArr =
		(!modelActions.containsKey(kvp.key))?
			modelActions.createNestedArray(kvp.key):
			modelActions.get<JsonArray>(kvp.key);

		JsonObjectUtil::copyArray(kvp.value, currArr);
	}

	if(i>0){
		Serial.print(FPSTR(" from file="));
		Serial.println(i);
	}else{
		Serial.println(FPSTR("No entity load is expected. All values on place"));
	}

	return i;
}

void JsonDao::initDataByModel() {
	Serial.println(FPSTR("Init Data by model"));

	for (std::list<EntityJson*>::iterator ent = entities.begin(); ent != entities.end(); ent++){
		JsonArray& varFields = root.get<JsonObject>(ROOT_PATH_MODEL)
				.get<JsonObject>((*ent)->getGroup())
				.get<JsonObject>((*ent)->getName())
				.get<JsonObject>(JSONKEY_actions)
				.get<JsonVariant>(JSONKEY_var);

		JsonObject& allFields = root.get<JsonObject>(ROOT_PATH_MODEL)
						.get<JsonObject>((*ent)->getGroup())
						.get<JsonObject>((*ent)->getName())
						.get<JsonObject>(JSONKEY_fields);

		JsonObject& dataFields = root.get<JsonObject>(ROOT_PATH_DATA)
							.get<JsonObject>((*ent)->getGroup())
							.get<JsonObject>((*ent)->getName());

		int add=0;
		for (const auto& kvp : varFields) {
			const char* key = kvp.as<const char*>();
			Serial.print(FPSTR("key="));
			Serial.println(key);

			dataFields.set(key, allFields.get<JsonVariant>(key));
		}

		Serial.print(FPSTR("Data updated fields ="));
		Serial.println(add);
	}

	JsonObjectUtil::print("ROOT=", root);
}

void JsonDao::initTemplateByModel() {
	Serial.println(FPSTR("Init deploy templates"));
	int add=0;
	int templCount = 0;

	for (std::list<EntityJson*>::iterator ent = entities.begin(); ent != entities.end(); ent++){

		EntityJson* entity = (*ent);

		JsonArray& statFields = root.get<JsonObject>(ROOT_PATH_MODEL)
						.get<JsonObject>(entity->getGroup())
						.get<JsonObject>(entity->getName())
						.get<JsonObject>(JSONKEY_actions)
						.get<JsonArray>(JSONKEY_stat);

		JsonArray& tmplFields = root.get<JsonObject>(ROOT_PATH_MODEL)
				.get<JsonObject>(entity->getGroup())
				.get<JsonObject>(entity->getName())
				.get<JsonObject>(JSONKEY_actions)
				.get<JsonArray>(JSONKEY_templateVar);

		JsonObject& allFields = root.get<JsonObject>(ROOT_PATH_MODEL)
						.get<JsonObject>((*ent)->getGroup())
						.get<JsonObject>((*ent)->getName())
						.get<JsonObject>(JSONKEY_fields);

		JsonObject& dataFields = root.get<JsonObject>(ROOT_PATH_DEPLOYED)
							.get<JsonObject>((*ent)->getGroup())
							.get<JsonObject>((*ent)->getName());

		Serial.println();
		tmplFields.printTo(Serial);
			Serial.println();
			allFields.printTo(Serial);
			Serial.println();
			dataFields.printTo(Serial);
			Serial.println();

		for (const auto& kvp : statFields) {
			const char* key = kvp.as<const char*>();

			dataFields.set(key, allFields.get<JsonVariant>(key));
			add++;
		}
		Serial.println(FPSTR("Stat completed"));

		for (const auto& kvp : tmplFields) {
			const char* key = kvp.as<const char*>();

			generateTemplateKey(entity, key);

			//dataFields.set(key, templKey);
			templCount++;
		}
		Serial.println(FPSTR("Tmpl completed"));
	}
		Serial.print(FPSTR("Deploy total static ="));
		Serial.println(add);
		Serial.print(FPSTR("Deploy total vars ="));
		Serial.println(templCount);

	JsonObjectUtil::print("ROOT=", root);
}

void JsonDao::generateTemplateKey(EntityJson* ent, const char* key) {
	String result=PERSENT_STR;
	result+=ent->getId();
	result+=":";
	result+=key;
	result+=PERSENT_STR;

	Serial.println(result);

	root.get<JsonObject>(ROOT_PATH_DEPLOYED)
		.get<JsonObject>(ent->getGroup())
		.get<JsonObject>(ent->getName())
		.set(key, result);

}

String JsonDao::getByTemplateKey(const char* key) {
	String str = String(key);
	int ind=str.indexOf(':');

	int entityId = str.substring(0, ind-1).toInt();
	const char* keyName=str.substring(ind).c_str();

	EntityJson* entity=getEntity(entityId);
	if(entity!=NULL){

		JsonObject& obj = root.get<JsonObject>(ROOT_PATH_DATA)
									.get<JsonObject>(entity->getGroup())
									.get<JsonObject>(entity->getName());

		if(obj.is<char*>(keyName)){
			str = obj.get<char*>(keyName);
		}else if(obj.is<int>(keyName)){
			str = obj.get<int>(keyName);
		}else if(obj.is<float>(keyName)){
			str = obj.get<float>(keyName);
		}

		return str;

	}
	return FPSTR("N/A");
}
