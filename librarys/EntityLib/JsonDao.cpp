/*
 * JsonDao.cpp
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: Vitaliy
 */
#include <Arduino.h>
#include <JsonDao.h>
#include <ArduinoJson.h>
//#include <FS.h>
#include <FileUtils.h>
#include <EntityJson.h>

JsonDao::JsonDao(EntityJson* entitiesIn[],int count) {
	for(int i=0;i<count;i++){
		this->entities.push_back(entitiesIn[i]);
	}
}

void JsonDao::init() {
	if(FileUtils::init()){
		FileUtils::dir();
	}

	Serial.println(FPSTR("=========================================="));
	Serial.print(FPSTR("JsonDao init. Entity count="));
	Serial.println(this->entities.size());
	Serial.println(FPSTR("------------------------------------------"));
	printEntities();
	Serial.println(FPSTR("------------------------------------------"));

	initModels();
	persistModels();

	//initDatas();
	//persistDatas();

	initTemplates();
	persistTemplates();

	Serial.println(FPSTR("JsonDao DONE"));
	//Serial.println(FPSTR("------------------------------------------"));
	//JsonObjectUtil::printAllJson(root);
	Serial.println(FPSTR("=========================================="));
}

std::list<EntityJson*>* JsonDao::getEntities() {
	return &this->entities;
}

JsonObject& JsonDao::getRoot() {
	return this->root;
}

void JsonDao::createEntityJson(const char* rootPath, EntityJson* ent) {
	JsonObjectUtil::getObjectChildOrCreateNew(root,rootPath);
	JsonObjectUtil::getObjectChildOrCreateNew(root.get<JsonObject>(rootPath),ent->getGroup());
	JsonObjectUtil::getObjectChildOrCreateNew(root.get<JsonObject>(rootPath).get<JsonObject>(ent->getGroup()),ent->getName());
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

EntityJson* JsonDao::getEntity(int i) {
	auto h = std::next(entities.begin(), i);
	EntityJson* entity = static_cast<EntityJson*>(*h);
	return entity;
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

void JsonDao::initModels(){
	Serial.println(FPSTR("---> Init models"));
	int id=0;

	JsonObjectUtil::getObjectChildOrCreateNew(root,ROOT_PATH_MODEL);

	for (std::list<EntityJson*>::iterator ent = entities.begin(); ent != entities.end(); ent++){

		createEntityJson(ROOT_PATH_MODEL,(*ent));
		//JsonObjectUtil::getObjectChildOrCreateNew(root.get<JsonObject>(ROOT_PATH_MODEL),(*it)->getGroup());
		//JsonObjectUtil::getObjectChildOrCreateNew(root.get<JsonObject>(ROOT_PATH_MODEL).get<JsonObject>((*it)->getGroup()),(*it)->getName());

		(*ent)->attachParams(id, this);
		(*ent)->init();
		id++;
		initEntityModel((*ent));
	}

	Serial.print(id);
	Serial.println(FPSTR(" - Entities processed"));

	//JsonObjectUtil::print("root=", root);
	Serial.println(FPSTR("------------------------------------------"));
}

void JsonDao::initEntityModel(EntityJson* ent) {
	Serial.println(ent->getName());

	createEntityJson(ROOT_PATH_MODEL,ent);
	JsonObject& json = getEntityModel(ent);

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

	//JsonObjectUtil::print("result =",json);
	b.clear();
}

void JsonDao::persistModels() {
	Serial.println(FPSTR("---> Persist Models"));
	const char* fileName = PATH_MODEL_file;
	const char* rootPath = ROOT_PATH_MODEL;

	Serial.print(rootPath);
	Serial.print(FPSTR(" storage ="));
	Serial.print(fileName);

	if(!FileUtils::existsAndHasSizeChar(fileName)){
		FileUtils::saveRootJson(fileName, rootPath, root);
	}else{
		bool fileEqModel=FileUtils::compareCrs(fileName, root.get<JsonObject>(rootPath));

		if(!fileEqModel){
			Serial.println(FPSTR(" File different rewrite"));
			bufTmp.clear();
			File f =FileUtils::getFile(fileName, FILE_MODE_READ);

			JsonObject& fromFile = bufTmp.parse(f).as<JsonObject>();

			int t=0;
			for (std::list<EntityJson*>::iterator it = entities.begin(); it != entities.end(); it++){
				t+=loadedModelToModel((*it),fromFile);
			}

			f.close();
			bufTmp.clear();

			if(t!=0){
				Serial.print(t);
				Serial.println(FPSTR(" changes done"));

				saveModels();
			}else{
				Serial.println(FPSTR(" no model changes"));
			}
		}else{
			Serial.println(FPSTR(" file is actual"));
		}
	}

	JsonObjectUtil::print("root=", root);
	Serial.println(FPSTR("------------------------------------------"));
}

void JsonDao::saveModels() {
	FileUtils::saveRootJson(PATH_MODEL_file, ROOT_PATH_MODEL, root);
}

int JsonDao::loadedModelToModel(EntityJson* ent,JsonObject& loadedModel) {
	Serial.print(FPSTR("Loading "));
	Serial.println(ent->getName());
	int i=0;
	JsonObject& modelFields = getEntityModelAllFields(ent);
	JsonObject& modelActions = getEntityModelActions(ent);
	JsonObject& loadedFields = loadedModel.get<JsonObject>(ent->getGroup()).get<JsonObject>(ent->getName()).get<JsonObject>(JSONKEY_fields);
	JsonObject& loadedActions = loadedModel.get<JsonObject>(ent->getGroup()).get<JsonObject>(ent->getName()).get<JsonObject>(JSONKEY_actions);

	for (const auto& kvp : loadedFields) {
		if(!isDefaultField(kvp.key) && strcmp(kvp.value,modelFields.get<const char*>(kvp.key))!=0){
			modelFields.set(strdup(kvp.key), strdup(kvp.value));
			i++;
		}
	}
	for (const auto& kvp : loadedActions) {
		if(!modelActions.containsKey(kvp.key)){
			i++;
		}

		JsonArray& currArr =
		(!modelActions.containsKey(kvp.key))?
			modelActions.createNestedArray(kvp.key):
			modelActions.get<JsonArray>(kvp.key);

		i+=JsonObjectUtil::copyArray(kvp.value, currArr);
	}

	if(i>0){
		Serial.print(FPSTR(" updated="));
		Serial.println(i);
	}else{
		Serial.println(FPSTR(" All values on place"));
	}
	return i;
}

void JsonDao::initDatas() {
	Serial.println(FPSTR("---> Init Data"));

	for (std::list<EntityJson*>::iterator ent = entities.begin(); ent != entities.end(); ent++){
		createEntityJson(ROOT_PATH_DATA,(*ent));

		EntityJson* entity=(*ent);

		JsonArray& varFields = getEntityModelVarFields(entity);
		JsonObject& allFields = getEntityModelAllFields(entity);
		JsonObject& dataFields = getEntityData(entity);

		int add=0;
		for (const auto& kvp : varFields) {
			const char* key = kvp.as<const char*>();
			dataFields.set(key, allFields.get<JsonVariant>(key));
			add++;
		}
		Serial.print(entity->getName());
		Serial.print(FPSTR(" var ="));
		Serial.println(add);
	}

	//JsonObjectUtil::print("root=", root);
	Serial.println(FPSTR("------------------------------------------"));
}

void JsonDao::persistDatas() {
	Serial.println(FPSTR("---> Persist Data"));
	const char* fileName = PATH_DATA_file;
	const char* rootPath = ROOT_PATH_DATA;

	Serial.print(rootPath);
	Serial.print(FPSTR(" storage ="));
	Serial.print(fileName);

	if(!FileUtils::existsAndHasSizeChar(fileName)){
		Serial.println(FPSTR(" - new "));
		FileUtils::saveRootJson(fileName, rootPath, root);
	}else{
		bool fileEqModel=FileUtils::compareCrs(fileName, root.get<JsonObject>(rootPath));

		if(!fileEqModel){
			Serial.println(FPSTR(" - recreate "));
			bufTmp.clear();
			File f =FileUtils::getFile(fileName, FILE_MODE_READ);

			JsonObject& fromFile = bufTmp.parse(f).as<JsonObject>();

			int t=0;
			for (std::list<EntityJson*>::iterator it = entities.begin(); it != entities.end(); it++){
				t+=loadedDataToData((*it),fromFile);
			}

			f.close();
			bufTmp.clear();

			if(t!=0){
				Serial.print(t);
				Serial.println(FPSTR(" changes done"));

				FileUtils::saveRootJson(fileName, rootPath, root);
			}else{
				Serial.println(FPSTR(" - no changes "));
			}
		}else{
			Serial.println(FPSTR(" - actual"));
		}
	}

	Serial.println(FPSTR("------------------------------------------"));
}

void JsonDao::saveDatas() {
	FileUtils::saveRootJson(PATH_DATA_file,ROOT_PATH_DATA, root);
}

int JsonDao::loadedDataToData(EntityJson* ent, JsonObject& loaded) {
	Serial.print(FPSTR("Loading "));
	Serial.println(ent->getName());
	int i=0;

	JsonArray& loadFields = getEntityDataLoadFields(ent);
	JsonObject& dataFields = getEntityData(ent);

	for (const auto& kvp : loadFields) {
		const char* key = kvp;
		if(!isDefaultField(kvp) && loaded.containsKey(key) &&
				strcmp(loaded.get<const char*>(key),dataFields.get<const char*>(key))!=0){
			dataFields.set(strdup(key), strdup(loaded.get<const char*>(key)));
			i++;
		}
	}

	if(i>0){
		Serial.print(i);
		Serial.println(FPSTR(" - updated "));

	}else{
		Serial.println(FPSTR(" - data is actual"));
	}

	return i;
}

void JsonDao::initTemplates() {
	Serial.println(FPSTR("---> Init Deploy Templates"));
	int add=0;
	int templCount = 0;

	for (std::list<EntityJson*>::iterator ent = entities.begin(); ent != entities.end(); ent++){
		createEntityJson(ROOT_PATH_DEPLOYED,(*ent));
		//JsonObjectUtil::getObjectChildOrCreateNew(root.get<JsonObject>(ROOT_PATH_DEPLOYED),(*ent)->getGroup());
		//JsonObjectUtil::getObjectChildOrCreateNew(root.get<JsonObject>(ROOT_PATH_DEPLOYED).get<JsonObject>((*ent)->getGroup()),(*ent)->getName());

		int entityTemplateVars = 0;
		int entityStat = 0;
		EntityJson* entity = (*ent);

		JsonObject& allFields = getEntityModelAllFields(entity);
		JsonArray& statFields = getEntityModelStatFields(entity);
		JsonArray& tmplFields = getEntityModelTemplateVarFields(entity);

		JsonObject& deployedFields = getEntityDeployed(entity);

		for (const auto& kvp : statFields) {
			const char* key = kvp.as<const char*>();

			deployedFields.set(key, allFields.get<JsonVariant>(key));
			entityStat++;
			add++;
		}

		for (const auto& kvp : tmplFields) {
			const char* key = kvp.as<const char*>();

			generateTemplateKey(entity, key);
			templCount++;
			entityTemplateVars++;
		}

		Serial.print(entity->getName());
		Serial.print(FPSTR(" stat = "));
		Serial.print(entityStat);
		Serial.print(FPSTR(" templVars = "));
		Serial.println(entityTemplateVars);
	}
		/*Serial.println();
		Serial.print(FPSTR("Total stat ="));
		Serial.print(add);
		Serial.print(FPSTR(" templVars ="));
		Serial.println(templCount);

	JsonObjectUtil::print("root=", root);
	*/
	Serial.println(FPSTR("------------------------------------------"));
}

void JsonDao::persistTemplates() {
	saveTemplates();
	JsonObjectUtil::print("root=", root);
	cleanTemplates();
	Serial.println(FPSTR("------------------------------------------"));
}

void JsonDao::saveTemplates() {
	int ok=0;
	int er=0;

	const char* baseFileName = PATH_DEPLOY_file;
	const char* rootPath = ROOT_PATH_DEPLOYED;

	Serial.println(FPSTR("---> Persist Deploy Templates"));
	Serial.print(rootPath);
	Serial.print(FPSTR(" storage ="));
	Serial.print(baseFileName);

	if(!FileUtils::existsAndHasSizeChar(baseFileName)){
		Serial.println(FPSTR(" - new "));
		if(FileUtils::saveRootJson(baseFileName, rootPath, root)){
			ok++;
		}else{er++;};

	}else{
		if(!FileUtils::compareCrs(baseFileName, root.get<JsonObject>(rootPath))){
			Serial.println(FPSTR(" - recreate"));
			if(FileUtils::saveRootJson(baseFileName, rootPath, root)){
				ok++;
			}else{er++;};
		}else{
			Serial.println(FPSTR(" - actual"));
		}
	}

	Serial.println(FPSTR("---> Persist Group Deploy"));

	JsonObject& rootDeploy = root.get<JsonObject>(rootPath);
	//JsonObjectUtil::print(rootPath, rootDeploy);

	for(int j=0;j<ROOT_PATHS_GROUPS_TOTAL;j++){
		const char* groupName = ROOT_PATHS_GROUPS[j];
		String fileNameStr = PATH_DEPLOY_folder;
				fileNameStr+=groupName;
				fileNameStr+=JSON_EXT;

		const char* fileName = fileNameStr.c_str();

		Serial.print(groupName);
		Serial.print(FPSTR(" storage ="));
		Serial.print(fileName);

		JsonObject& group = rootDeploy.get<JsonObject>(groupName);
		//JsonObjectUtil::print(groupName, group);


		if(!FileUtils::existsAndHasSizeChar(fileName)){
			Serial.println(FPSTR(" - new "));
			if(FileUtils::saveRootJson(fileName, groupName, rootDeploy)){
				ok++;
			}else{er++;};
		}else{
			if(!FileUtils::compareCrs(fileName, group)){
				Serial.println(FPSTR(" - recreate"));
				if(FileUtils::saveRootJson(fileName, groupName, rootDeploy)){
					ok++;
				}else{er++;};
			}else{
				Serial.println(FPSTR(" - actual"));
			}
		}
	}

	if(er>0){
		Serial.print(er);
		Serial.println(FPSTR(" save Errors"));
	}

	if(ok>0){
		Serial.print(ok);
		Serial.println(FPSTR(" deploys updated"));
	}else{
		Serial.println(FPSTR(" All saved templates are Up to date"));
	}
}

void JsonDao::cleanTemplates() {
	uint32_t heapBefore = ESP.getFreeHeap();
	Serial.print(FPSTR("Clean In memory deployments heap before = "));
	Serial.print(heapBefore);

	root.remove(ROOT_PATH_DEPLOYED);

	uint32_t heapAfter = ESP.getFreeHeap();
	int32_t diff= heapAfter - heapBefore;

	Serial.print(FPSTR(" after = "));
	Serial.print(heapAfter);
	Serial.print(FPSTR("   DIFF = "));
	Serial.println(String(diff));
}

void JsonDao::generateTemplateKey(EntityJson* ent, const char* key) {
	String result=PERSENT_STR;
	result+=ent->getId();
	result+=":";
	result+=key;
	result+=PERSENT_STR;

	getEntityDeployed(ent).set(key, result);
}

String JsonDao::getByTemplateKey(const char* key) {
	String str = String(key);
	int ind=str.indexOf(':');

	int entityId = str.substring(0, ind-1).toInt();
	const char* keyName=str.substring(ind).c_str();

	EntityJson* entity=getEntity(entityId);
	if(entity!=NULL){
		return JsonObjectUtil::getObjectValueAsString(getEntityData(entity), keyName);
	}
	return FPSTR("N/A");
}

JsonObject& JsonDao::getEntityModel(EntityJson* e) {
	return getEntitysJson(ROOT_PATH_MODEL,e);
}

JsonObject& JsonDao::getEntityData(EntityJson* e) {
	return getEntitysJson(ROOT_PATH_DATA,e);
}

JsonObject& JsonDao::getEntityDeployed(EntityJson* e) {
	return getEntitysJson(ROOT_PATH_DEPLOYED,e);
}

JsonObject& JsonDao::getEntitysJson(const char* rootPathJson, EntityJson* entity) {
	return root.get<JsonObject>(rootPathJson)
			.get<JsonObject>(entity->getGroup())
			.get<JsonObject>(entity->getName());
}

JsonObject& JsonDao::getEntityModelAllFields(EntityJson* e) {
	return getEntityModel(e).get<JsonObject>(JSONKEY_fields);
}

JsonArray& JsonDao::getEntityModelStatFields(EntityJson* e) {
	return getEntityModel(e).get<JsonObject>(JSONKEY_actions).get<JsonArray>(JSONKEY_stat);
}

JsonArray& JsonDao::getEntityModelVarFields(EntityJson* e) {
	return getEntityModel(e).get<JsonObject>(JSONKEY_actions).get<JsonArray>(JSONKEY_var);
}

JsonArray& JsonDao::getEntityModelTemplateVarFields(EntityJson* e) {
	return getEntityModel(e).get<JsonObject>(JSONKEY_actions).get<JsonArray>(JSONKEY_templateVar);
}

JsonArray& JsonDao::getEntityDataLoadFields(EntityJson* e) {
	return getEntityModel(e).get<JsonObject>(JSONKEY_actions).get<JsonArray>(JSONKEY_load);
}

JsonArray& JsonDao::getEntityDataSaveFields(EntityJson* e) {
	return getEntityModel(e).get<JsonObject>(JSONKEY_actions).get<JsonArray>(JSONKEY_save);
}

JsonArray& JsonDao::getEntityDataSetFields(EntityJson* e) {
	return getEntityModel(e).get<JsonObject>(JSONKEY_actions).get<JsonArray>(JSONKEY_set);
}

JsonObject& JsonDao::getEntityModelActions(EntityJson* e) {
	return getEntityModel(e).get<JsonObject>(JSONKEY_actions);
}

bool JsonDao::getFieldHasAction(EntityJson* e,const char* key, const char* action) {
	return JsonObjectUtil::isInArray(key, getEntityModelActions(e).get<JsonArray>(action));
}

JsonObject& JsonDao::getEntityModel(int entityId) {
	return getEntityModel(getEntity(entityId));
}

JsonObject& JsonDao::getEntityData(int entityId) {
	return getEntityData(getEntity(entityId));
}

bool JsonDao::hasFieldInt(int entityId, const char* key) {
	return hasField<int>(entityId, key);
}

bool JsonDao::hasFieldFloat(int entityId, const char* key) {
	return hasField<float>(entityId, key);
}

bool JsonDao::hasFieldConstChar(int entityId, const char* key) {
	return hasField<const char*>(entityId, key);
}

bool JsonDao::hasFieldChar(int entityId, const char* key) {
	return hasField<char*>(entityId, key);
}

bool JsonDao::setField(int entityId, const char* key, int value) {
	return setField<int>(entityId, key, value);
}

bool JsonDao::setField(int entityId, const char* key, float value) {
	return setField<float>(entityId, key, value);
}

bool JsonDao::setField(int entityId, const char* key, const char* value) {
	return setField<const char*>(entityId, key, value);
}

bool JsonDao::setField(int entityId, const char* key, char* value) {
	return setField<char*>(entityId, key, value);
}

int JsonDao::getFieldInt(int entityId, const char* key) {
	return getField<int>(entityId, key);
}

float JsonDao::getFieldFloat(int entityId, const char* key) {
	return getField<float>(entityId, key);
}

const char* JsonDao::getFieldConstChar(int entityId, const char* key) {
	return getField<const char*>(entityId, key);
}

char* JsonDao::getFieldChar(int entityId, const char* key) {
	return strdup(getField<const char*>(entityId, key));
}

void JsonDao::printEntities() {
	for (std::list<EntityJson*>::iterator it = entities.begin(); it != entities.end(); it++){
		(*it)->print();
	}
}

JsonObject& JsonDao::getEntitysJson(const char* rootPathJson, int entityId) {
	EntityJson* entity = getEntity(entityId);
	return getEntitysJson(rootPathJson, entity);
}
