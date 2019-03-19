/*
 * JsonDao.cpp
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: Vitaliy
 */
#include <Arduino.h>
#include <JsonDao.h>
#include <ArduinoJson.h>
#include <FileUtils.h>
#include <EntityJson.h>
#include <ObjectUtils.h>

JsonDao::JsonDao(EntityJson* entitiesIn[],int count) {
	beginTime = millis();
	long tmp=millis();
	this->incomeEntities = ARRAY_SIZE(entitiesIn);
	for(int i=0;i<count;i++){
		this->entities.push_back(entitiesIn[i]);
	}
	initTime=millis()-tmp;
	this->eventSender=nullptr;
}

void JsonDao::init() {
	Serial.print(FPSTR(" createTime ="));
	Serial.println(initTime);
	initTime=millis();
	FileUtils::dir();
	initTime=millis()-initTime;

	Serial.println(FPSTR("=========================================="));
	Serial.print(FPSTR("JsonDao init. Entity count="));
	Serial.println(this->entities.size());
	Serial.print(FPSTR(" initTime ="));
	Serial.println(initTime);
	Serial.println(FPSTR("------------------------------------------"));

	printEntities();

	Serial.println(FPSTR("------------------------------------------"));
	initTime=millis();

	initEntitiesModelData();

	initTime=millis()-initTime;
	Serial.print(FPSTR(" initEntMD time ="));
	Serial.println(initTime);
	initTime=millis();
#ifdef DEPLOY_TEMPLATES
	initTemplates();
	persistTemplates();

	initTime=millis()-initTime;
	Serial.print(FPSTR(" json depl time ="));
	Serial.println(initTime);
#endif
	initTime=millis()-beginTime;
	Serial.print(FPSTR(" TOTAL time ="));
	Serial.println(initTime);

	Serial.println(FPSTR("JsonDao DONE"));
	//Serial.println(FPSTR("------------------------------------------"));
	//JsonObjectUtil::printAllJson(root);
	Serial.println(FPSTR("=========================================="));
}

std::list<EntityJson*>* JsonDao::getEntities() {
	return &this->entities;
}

EntityJson* JsonDao::getEntity(int i) {
	auto h = std::next(entities.begin(), i);
	EntityJson* entity = static_cast<EntityJson*>(*h);
	return entity;
}

JsonObject& JsonDao::getEntitysJson_ByPath_OrCreateNew(JsonObject& container,const char* path,EntityJson* entity) {
	JsonObject& rootPathObj=JsonObjectUtil::getObjectChildOrCreateNew(container,path);
	JsonObject& groupObj=JsonObjectUtil::getObjectChildOrCreateNew(rootPathObj,entity->getGroup());
	return JsonObjectUtil::getObjectChildOrCreateNew(groupObj,entity->getName());
}

JsonObject& JsonDao::getEntitysJson_ByPath(JsonObject& container,const char* path,EntityJson* entity){
	return container.get<JsonObject>(path)
			.get<JsonObject>(entity->getGroup())
			.get<JsonObject>(entity->getName());
}

JsonObject& JsonDao::getEntitysJson_ByPath_entId(JsonObject& container,const char* path, int entityId) {
	EntityJson* entity = getEntity(entityId);
	return getEntitysJson_ByPath(container,path, entity);
}

bool JsonDao::isDefaultField(const char* key) {
	return (strcmp(key,JSONKEY_id)==0 || strcmp(key,JSONKEY_name)==0 || strcmp(key,JSONKEY_group)==0);
}

JsonObject& JsonDao::loadRootIfExists(){
	const char* fileName = (PATH_MODEL_DATA_JSON_FILE);

	Serial.print(FPSTR("Load root storage ="));
	Serial.print(fileName);

	bufTmp.clear();

	if(!FileUtils::existsAndHasSizeChar(fileName)){
		Serial.println(FPSTR(" - NO Root file. Empty obj returned"));
		return bufTmp.createObject();
	}else{
		Serial.println(FPSTR(" - Root file exists. Load into tmpBuffer"));

		File f =FileUtils::getFile(fileName, FILE_MODE_READ);
		return bufTmp.parse(f).as<JsonObject>();
	}

	//rootTmp.printTo(Serial);

	return rootTmp;
}

bool JsonDao::saveJsonObjectToFile(const char* fileName,JsonObject& json) {
	if(!FileUtils::existsAndHasSizeChar(fileName) || !FileUtils::compareCrs(fileName, json)){

		bool res=FileUtils::saveJsonToFile(PATH_MODEL_DATA_JSON_FILE, json);
		if(res){Serial.println(FPSTR("Root saved OK."));}else{Serial.println(FPSTR("Root saved ERROR"));}
		return res;
	}else{
		Serial.println(FPSTR("Root and file on disc are similar"));
	}
	return true;
}

void JsonDao::initEntitiesModelData(){
	Serial.println(FPSTR("---> Init models"));
	int id=0;

	JsonObjectUtil::getObjectChildOrCreateNew(root,ROOT_PATH_MODEL);
	JsonObject& fromFile = loadRootIfExists();

	for (std::list<EntityJson*>::iterator ent = entities.begin(); ent != entities.end(); ent++){

		EntityJson* entity = (*ent);

		entity->attachParams(id, this);
		entity->init();
		id++;
		initEntityModelData(entity, fromFile);
		entity->postModelDataInit();
	}

	Serial.print(id);
	Serial.println(FPSTR(" - Entities processed"));

	//JsonObjectUtil::print("root=", root);

	saveJsonObjectToFile(PATH_MODEL_DATA_JSON_FILE, root);

	Serial.println(FPSTR("------------------------------------------"));
}

int JsonDao::createEntityDataPrimaryFields(EntityJson* e,JsonObject& data) {
	if(e->hasPrimFields()){
		data.set(JSONKEY_id,e->getId());
		data.set(JSONKEY_group,e->getGroup());
		data.set(JSONKEY_name,e->getName());
		data.set(JSONKEY_descr,e->getDescr());

		return 4;
	}

	return 0;
}

void JsonDao::initEntityModelData(EntityJson* entity,JsonObject& fromFile) {
	Serial.println(entity->getName());

	DynamicJsonBuffer b;
	JsonObject& descriptor = b.parse(entity->getDescriptor());

	JsonObject& modelDescriptor = JsonObjectUtil::getObjectChildOrCreateNew(descriptor,JSONKEY_model);
	JsonObject& modelLoaded = getEntitysJson_ByPath_OrCreateNew(fromFile,ROOT_PATH_MODEL,entity);

	JsonObject& model = getEntitysJson_ByPath_OrCreateNew(root,ROOT_PATH_MODEL,entity);

	mergeModels(modelDescriptor,model);
	mergeModels(modelLoaded,model);

	JsonObject& dataDescriptor = JsonObjectUtil::getObjectChildOrCreateNew(descriptor,JSONKEY_data);
	JsonObject& dataLoaded = getEntitysJson_ByPath_OrCreateNew(fromFile,ROOT_PATH_DATA,entity);

	JsonObject& data = getEntitysJson_ByPath_OrCreateNew(root,ROOT_PATH_DATA,entity);

	//JsonObjectUtil::print("before descr root =",root);
	mergeDatas(dataDescriptor,data);
	//JsonObjectUtil::print("before load root =",root);
	mergeDatas(dataLoaded,data);
	//JsonObjectUtil::print("after load root =",root);

	createEntityDataPrimaryFields(entity, data);
	//JsonObjectUtil::print("DESCR =",descriptor);
	//JsonObjectUtil::print("ent done root =",root);

	b.clear();
}

int JsonDao::mergeModels(JsonObject& from, JsonObject& to) {

	int changed=0;

	for (const auto& kvp : from) {
		if(from.is<JsonArray>(kvp.key)){
			if(!to.containsKey(kvp.key)){
				changed++;
			}
			JsonArray& currArr = JsonObjectUtil::getObjectChildArrayOrCreateNew(to, kvp.key);
			changed+=JsonObjectUtil::copyArray(kvp.value, currArr);
		}
	}

	if(changed>0){
		/*Serial.print(FPSTR("models merged="));
		Serial.println(changed);
		*/
	}

	return changed;
}

JsonArray& JsonDao::getEntityDataFieldsByAction(int entityId,const char* action) {
	EntityJson* entity = getEntity(entityId);
	return getEntityDataFieldsByAction(entity, action);
}

const char* JsonDao::validateField(int entityId, const char* key,const  String& value) {
	EntityJson* entity = getEntity(entityId);
	return entity->validateField(key, value);
}

int JsonDao::mergeDatas(JsonObject& from, JsonObject& to) {

	int changed=0;

	for (const auto& kvp : from) {
		if(!isDefaultField(kvp.key)){
			if(!to.containsKey(kvp.key) || to[kvp.key]!=from[kvp.key]){
				changed++;
				to.set(strdup(kvp.key), strdup(kvp.value));
				/*
				Serial.print(FPSTR("key="));
				Serial.print(kvp.key);
				Serial.print(FPSTR(" value="));
				Serial.println(kvp.value.as<char*>());*/
			}
		}
	}

	if(changed>0){
		/*Serial.print(FPSTR("data merged="));
		Serial.println(changed);*/
	}

	return changed;
}
#ifdef DEPLOY_TEMPLATES
void JsonDao::initTemplates() {
	Serial.println(FPSTR("---> Init Deploy Templates"));

	for (std::list<EntityJson*>::iterator it = entities.begin(); it != entities.end(); it++){

		int entityTstat = 0;
		int entityTvars = 0;
		EntityJson* entity = (*it);

		if(entity->isDeployedAsTemplate()){
			JsonObject& data = getEntitysJson_ByPath_OrCreateNew(root,ROOT_PATH_DATA,entity);
			JsonObject& deployedData = getEntitysJson_ByPath_OrCreateNew(root,ROOT_PATH_DEPLOYED,entity);

			entityTstat+=createEntityDataPrimaryFields(entity, deployedData);

			if(getEntityHasAction(entity,ENTITY_ACTION_tstat)){
				JsonArray& statFields = getEntityDataFieldsByAction(entity, ENTITY_ACTION_tstat);

				for (const auto& kvp : statFields) {
					const char* key = kvp.as<const char*>();

					deployedData.set(key, data.get<JsonVariant>(key));
					entityTstat++;
				}
			}

			if(getEntityHasAction(entity,ENTITY_ACTION_tvar)){
				JsonArray& tmplFields = getEntityDataFieldsByAction(entity, ENTITY_ACTION_tvar);

				if(getEntityDataFieldHasAction(entity, "*", ENTITY_ACTION_tvar)){
					for (const auto& kvp : getEntityData(entity->getId())) {
						const char* key = kvp.key;

						generateTemplateKey(entity, key);
						entityTvars++;
					}
				}else{

					for (const auto& kvp : tmplFields) {
						const char* key = kvp.as<const char*>();

						generateTemplateKey(entity, key);
						entityTvars++;
					}
				}
			}

		}

		Serial.print(entity->getName());
		Serial.print(FPSTR(" Tstat = "));
		Serial.print(entityTstat);
		Serial.print(FPSTR(" Tvars = "));
		Serial.println(entityTvars);
	}

	//JsonObjectUtil::print("root=", root);
	Serial.println(FPSTR("------------------------------------------"));
}

void JsonDao::persistTemplates() {
	Serial.println(FPSTR("Persist templates"));
	saveTemplates();
	cleanTemplates();
	JsonObjectUtil::print("root=", root);
	Serial.println(FPSTR("------------------------------------------"));
}

void JsonDao::saveTemplates() {
	int ok=0;
	int er=0;

	const char* baseFileName = PATH_MODEL_DATA_JSON_FILE;
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

	FileUtils::printFile(baseFileName);

	Serial.println(FPSTR("---> Persist Group Deploy"));

	JsonObject& rootDeploy = root.get<JsonObject>(rootPath);
	//JsonObjectUtil::print(rootPath, rootDeploy);

	for(int j=0;j<ROOT_PATHS_GROUPS_TOTAL;j++){
		const char* groupName = ROOT_PATHS_GROUPS[j];
		String fileNameStr = PATH_MODEL_DATA_JSON_BY_GROUP;
				fileNameStr+=groupName;
				fileNameStr+=JSON_EXT;

		const char* fileName = fileNameStr.c_str();

		Serial.print(groupName);
		Serial.print(FPSTR(" storage ="));
		Serial.print(fileName);

		JsonObject& group = rootDeploy.get<JsonObject>(groupName);

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

		FileUtils::printFile(fileName);
	}

	if(er>0){
		Serial.print(er);
		Serial.println(FPSTR(" total Errors"));
	}

	if(ok>0){
		Serial.print(ok);
		Serial.println(FPSTR(" total Deploys updated"));
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

void JsonDao::generateTemplateKey(EntityJson* entity, const char* key) {
	String result=PERSENT_STR;
	result+=entity->getId();
	result+=":";
	result+=key;
	result+=PERSENT_STR;

	getEntitysJson_ByPath_OrCreateNew(root,ROOT_PATH_DEPLOYED,entity).set(key, result);
}

String JsonDao::getByTemplateKey(const String& key) {
	String str = String(key);
	int ind=str.indexOf(':');

	int entityId = str.substring(0, ind).toInt();
	const char* keyName=strdup(str.substring(ind+1).c_str());

/*
	Serial.print(FPSTR(" key="));
	Serial.print(key);
	Serial.print(FPSTR(" entId="));
	Serial.print(entityId);
	Serial.print(FPSTR(" keyName="));
	Serial.print(keyName);
	Serial.print(FPSTR(" ind="));
	Serial.println(ind);
*/
	EntityJson* entity=getEntity(entityId);
	if(entity!=NULL){
		return JsonObjectUtil::getObjectValueAsString(getEntitysJson_ByPath_OrCreateNew(root,ROOT_PATH_DATA,entity), keyName);
	}
	return FPSTR("noEntity");
}
#endif
JsonObject& JsonDao::getEntityModel(int entityId) {
	return getEntitysJson_ByPath_entId(root,ROOT_PATH_MODEL,entityId);
}

JsonObject& JsonDao::getEntityData(int entityId) {
	return getEntitysJson_ByPath_entId(root,ROOT_PATH_DATA,entityId);
}

bool JsonDao::getEntityHasAction(EntityJson* entity, const char* action) {
	JsonObject& model = getEntitysJson_ByPath(root,ROOT_PATH_MODEL,entity);
	return model.containsKey(action);
}

bool JsonDao::getEntityDataFieldHasAction(EntityJson* entity,const char* dataField, const char* action) {
	return getEntityHasAction(entity, action)
			&& JsonObjectUtil::isInArray(dataField, getEntitysJson_ByPath(root,ROOT_PATH_MODEL,entity).get<JsonArray>(action));
}

JsonArray& JsonDao::getEntityDataFieldsByAction(EntityJson* entity,
		const char* action) {
	JsonObject& model = getEntitysJson_ByPath(root,ROOT_PATH_MODEL,entity);
	return JsonObjectUtil::getObjectChildArrayOrCreateNew(model, action);
}

bool JsonDao::setField(int entityId, const char* key, int value) {	return setField<int>(entityId, key, value); }
bool JsonDao::setField(int entityId, const char* key, float value) {  return setField<float>(entityId, key, value); }
bool JsonDao::setField(int entityId, const char* key, const char* value) {	return setField<const char*>(entityId, key, value); }
bool JsonDao::setField(int entityId, const char* key, char* value) {  return setField<char*>(entityId, key, value); }
bool JsonDao::setField(int entityId, const char* key,String value) {
	Serial.println(FPSTR("Set field"));

	JsonObject& data = getEntityData(entityId);
	if(data.is<int>(key)){
		return setField(entityId, key, value.toInt());
	}
	if(data.is<float>(key)){
		return setField(entityId, key, value.toFloat());
	}
	if(data.is<const char*>(key) || data.is<char*>(key)){
		return setField(entityId, key, strdup(value.c_str()));
	}
	return false;
}

int JsonDao::getFieldInt(int entityId, const char* key) { return getField<int>(entityId, key); }
float JsonDao::getFieldFloat(int entityId, const char* key) { return getField<float>(entityId, key); }
const char* JsonDao::getFieldConstChar(int entityId, const char* key) {	return getField<const char*>(entityId, key);}
char* JsonDao::getFieldChar(int entityId, const char* key) { return strdup(getField<const char*>(entityId, key)); }

void JsonDao::printEntities() {
	for (std::list<EntityJson*>::iterator it = entities.begin(); it != entities.end(); it++){
		(*it)->print();
	}
}

JsonObject& JsonDao::getEntityRoot() {
	root.printTo(Serial);
	return root;
}

bool JsonDao::saveRootToFileIfChanged() {
	//JsonObjectUtil::print("before save root=", root);
	bool res = saveJsonObjectToFile(PATH_MODEL_DATA_JSON_FILE, root);
	//JsonObjectUtil::print("after save root=", root);
	return res;
}
