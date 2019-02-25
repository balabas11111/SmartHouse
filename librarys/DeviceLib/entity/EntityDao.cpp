/*
 * EntityDao.cpp
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#include <entity/EntityDao.h>
#include <list>

EntityDao::EntityDao() {
}

EntityDao::~EntityDao() {
	// TODO Auto-generated destructor stub
}

EntityDao::EntityDao(Entity* ent[], int size) {
	for(uint8_t i=0;i<size;i++){
			Serial.println(ent[i]->getName());
			entities.push_back(ent[i]);
			Serial.println(ent[i]->getGroup());
		}

		Serial.print(FPSTR("size="));
		Serial.println(entities.size());
}

void EntityDao::init() {
	Serial.println(FPSTR("Init Json model"));

	Serial.println(FPSTR("Init entities"));
	int id=0;

	if(FileUtils::existsAndHasSize(modelSavePath)){
		/*File f = FileUtils::getFile(modelSavePath, FILE_MODE_READ);
		tmpBuf.parse(f);
		f.close();
		*/
	}else{
		Serial.println(FPSTR("No saves detected"));
	}

	entityFieldDao.setEventTargets([this](EntityField* f){processEntityFieldChangeEvent(f);},
									[this](int eId,const char* key,const char* val){processStaticEntityFieldChangeEvent(eId,key,val);});

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++){
			initEntity(id,*it);

			id++;
	}

	Serial.println(FPSTR("----------ROOT--------------"));
	root.printTo(Serial);
	Serial.println(FPSTR("----------SAVE--------------"));
	tmp.printTo(Serial);

	if(this->getChanged()){
		//save snapshot to file
		JsonObject& loadedModel = tmp.get<JsonObject>(this->modelPath);
/*
		File f = FileUtils::getFile(modelSavePath, FILE_MODE_WRITE);
		loadedModel.printTo(f);
		f.close();
*/
		//tmpBuf.clear();
	}
		//save templates to deploy path

	Serial.println();
	Serial.print(FPSTR("size="));
	Serial.println(entities.size());

	saveTemplateToDeployPath(root);
}

void EntityDao::initEntity(int id,Entity* e) {
	Serial.print(e->getGroup());
	Serial.print(FPSTR("."));
	Serial.print(e->getName());
	Serial.println();

	e->init(id,&entityFieldDao);

	existsOrCreateEntityPath(root,this->modelPath,e);
	existsOrCreateEntityPath(root,this->templateDeployPath,e);

	JsonObject& entityModel = root.get<JsonObject>(this->modelPath)
									.get<JsonObject>(e->getGroup()).get<JsonObject>(e->getName());

	JsonObject& entityTemplate = root.get<JsonObject>(this->templateDeployPath)
									.get<JsonObject>(e->getGroup()).get<JsonObject>(e->getName());;

	e->initModelDefault();
	e->initModel();

	putEntityStatToModelTemplate(entityModel,e);
	putEntityStatToModelTemplate(entityTemplate,e);

	putEntityVarsToModel(entityModel,e);
	putEntityVarsToTemplate(entityTemplate,e);

	Serial.println();
	Serial.println(FPSTR("Load data"));

	existsOrCreateEntityPath(tmp,this->modelPath,e);
	existsOrCreateEntityPath(tmp,this->templateDeployPath,e);

	JsonObject& loadedModel = tmp.get<JsonObject>(this->modelPath)
									.get<JsonObject>(e->getGroup()).get<JsonObject>(e->getName());

	JsonObject& loadedTemplate = tmp.get<JsonObject>(this->templateDeployPath)
									.get<JsonObject>(e->getGroup()).get<JsonObject>(e->getName());;

	loadEntityStatVar(loadedModel,entityModel,e);

	saveEntityStatVar(loadedModel,entityModel,e);

	Serial.println(FPSTR("...done"));
}

void EntityDao::printJson() {
	Serial.println(FPSTR("----------ROOT--------------"));
	root.printTo(Serial);
	Serial.println();
	Serial.println(FPSTR("----------TMP--------------"));
	tmp.printTo(Serial);

	Serial.println();/*
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++){
		Entity* e=*it;
		Serial.print(FPSTR("ENTITY "));
		Serial.print(e->getName());
		std::list<EntityField*>* box=e->getBox();

		Serial.print(FPSTR(" size="));
		Serial.print(box->size());

		for (std::list<EntityField*>::iterator ite = box->begin(); ite != box->end(); ite++){
			EntityField* ef= *ite;
			Serial.print(FPSTR(" "));
			Serial.print(ef->getKey());
			Serial.print(FPSTR(" : "));
			Serial.print(ef->getEntityIndexId());

			int efi=e->getIndexByKeyName(ef->getKey());
			Serial.print(FPSTR(" (indexByKey="));
			Serial.print(efi);
			Serial.print(FPSTR("; valByKey="));
			entityFieldDao.printValByEntityId(efi);
		}

		Serial.println();
	}*/

	//entityFieldDao.print();
}

void EntityDao::getExistsOrCreateNew(JsonObject& parent,const char* key[]) {
	if(!parent.containsKey(FPSTR(key))){
		parent.createNestedObject(FPSTR(key));
	}
}

void EntityDao::getExistsOrCreateNewByEkey(JsonObject& parent,
		const char* key) {
	if(!parent.containsKey(key)){
		parent.createNestedObject(key);
	}
}

String EntityDao::processor(const String& var) {
	return entityFieldDao.getValByEntityFieldIndexString(var.c_str());
}

void EntityDao::existsOrCreateEntityPath(JsonObject& store,const char* rootPath, Entity* entity) {
	existsOrCreateEntityPathByName(store,rootPath, entity->getGroup(), entity->getName());
}

bool EntityDao::getChanged() {
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++){
		Entity* e=*it;
		if(e->isChanged()){
			return true;
		}
	}

	return false;
}

Entity* EntityDao::getEntityByName(const char* groupName,
		const char* entityName) {
	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++){
		Entity* e=*it;
		if(strcmp(e->getGroup(),groupName)==0 && strcmp(e->getName(),entityName)==0){
			return e;;
		}
	}

	return NULL;
}
void EntityDao::processEntityFieldChangeEvent(EntityField* field) {
	/*Entity* e = getEntityById(field);

	JsonObject& entityModel = root.get<JsonObject>(this->modelPath)
										.get<JsonObject>(e->getGroup()).get<JsonObject>(e->getName());
*/
	//entityModel.set(key, entityFieldDao.getValByEntityId(entitityFieldIndex));
}
void EntityDao::processStaticEntityFieldChangeEvent(int entityId,const char* key,const char* val) {
	/*Entity* e = getEntityById(field);

	JsonObject& entityModel = root.get<JsonObject>(this->modelPath)
										.get<JsonObject>(e->getGroup()).get<JsonObject>(e->getName());
*/
	//entityModel.set(key, entityFieldDao.getValByEntityId(entitityFieldIndex));
}

JsonObject& EntityDao::getRootEntityJson(const char* path,const char* group, const char* name) {
	return getEntity(root, path, group, name);
}
JsonObject& EntityDao::getTmpEntityJson(const char* path,const char* group, const char* name) {
	return getEntity(tmp, path, group, name);
}

Entity* EntityDao::getEntityById(int entityId) {
	auto h= std::next(entities.begin(), entityId);
	Entity* result = *h;
	return result;
}

bool EntityDao::updateRootEntity(const char* path,const char* group, const char* name, JsonObject& from) {
	return updateEntity(root,from, path, group, name);
}

bool EntityDao::updateTmpEntity(const char* path,const char* group, const char* name, JsonObject& from) {
	return updateEntity(tmp,from, path, group, name);
}

void EntityDao::existsOrCreateEntityPathByName(JsonObject& store,
		const char* rootPath, const char* group, const char* name) {

	getExistsOrCreateNewByEkey(store,rootPath);
	JsonObject& p = store.get<JsonObject>(rootPath);

	getExistsOrCreateNewByEkey(p,group);
	JsonObject& g = p.get<JsonObject>(group);

	getExistsOrCreateNewByEkey(g,name);
}

bool EntityDao::updateEntity(JsonObject& rootObject, JsonObject& from,const char* path,const char* group, const char* name) {
	JsonObject& target = getEntity(rootObject,path,group,name);
	return JsonObjectUtil::copy(from,target,true)>0;
}

JsonObject& EntityDao::getEntity(JsonObject& rootObject, const char* path,
		const char* group, const char* name) {

	existsOrCreateEntityPathByName(rootObject,path,group, name);
	return rootObject.get<JsonObject>(path).get<JsonObject>(group).get<JsonObject>(name);
}


void EntityDao::saveTemplateToDeployPath(JsonObject& root) {
	JsonObject& entityTemplateByGroup = root.get<JsonObject>(this->templateDeployPath);

	for (JsonObject::iterator it = entityTemplateByGroup.begin(); it != entityTemplateByGroup.end(); ++it ){
		JsonObject& groupTmpl = it->value;

		String fileName = templateSavePath + String(it->key)+".json";
		//groupTmpl.set("fileName", fileName);

		bool doSave=false;
/*
		if(FileUtils::existsAndHasSize(fileName)){
			String actual=FileUtils::loadStringFromFile(fileName);
			String expect;
			groupTmpl.printTo(expect);

			if(expect!=actual){
				doSave=true;
			}
		}
*/
		if(doSave){
			File f = FileUtils::getFile(fileName, "w");
			groupTmpl.printTo(f);
			f.close();
		}
	}
}

void EntityDao::setEntity_all_FromJson(JsonObject& json, std::list<int>& lst) {
	for (std::list<int>::iterator it = lst.begin(); it != lst.end(); it++){
		EntityField* ef = entityFieldDao.getEntityField(*it);
		if(ef!=NULL){
			ef->setToField(json);
		}
	}
}
void EntityDao::setEntity_all_ToJson(JsonObject& json, std::list<int>& lst) {
	for (std::list<int>::iterator it = lst.begin(); it != lst.end(); it++){
		EntityField* ef = entityFieldDao.getEntityField(*it);
		if(ef!=NULL){
			ef->getFromField(json);
		}
	}
}
void EntityDao::setEntity_stat_ToJson(JsonObject& json, std::list<int>& lst) {
	for (std::list<int>::iterator it = lst.begin(); it != lst.end(); it++){
		EntityField* ef = entityFieldDao.getEntityField(*it);
		if(ef!=NULL && !ef->isChangeAble()){
			ef->getFromField(json);
		}
	}
}
void EntityDao::setEntity_var_ToJson(JsonObject& json, std::list<int>& lst) {
	for (std::list<int>::iterator it = lst.begin(); it != lst.end(); it++){
		EntityField* ef = entityFieldDao.getEntityField(*it);
		if(ef!=NULL && ef->isChangeAble()){
			ef->getFromField(json);
		}
	}
}
void EntityDao::setEntity_TemplateKeys_ToJson(JsonObject& json, std::list<int>& lst) {
	for (std::list<int>::iterator it = lst.begin(); it != lst.end(); it++){
		EntityField* ef = entityFieldDao.getEntityField(*it);
		if(ef!=NULL){
			ef->getFromFieldTemplateKey(json);
		}
	}
}
void EntityDao::setEntity_const_ToJson(JsonObject& json, const char* value,std::list<int>& lst) {
	for (std::list<int>::iterator it = lst.begin(); it != lst.end(); it++){
		EntityField* ef = entityFieldDao.getEntityField(*it);
		if(ef!=NULL){
			json.set(ef->getKey(), value);
		}
	}
}

void EntityDao::putEntityStatToModelTemplate(JsonObject& target, Entity* e) {
	std::list<int> lst =e->descriptor().getStatFields();

	setEntity_stat_ToJson(target,lst);
}

void EntityDao::putEntityVarsToModel(JsonObject& target, Entity* e) {
	std::list<int> lst =e->descriptor().getVarFields();

	setEntity_var_ToJson(target,lst);
}

void EntityDao::putEntityVarsToTemplate(JsonObject& target, Entity* e) {
	std::list<int> lst =e->descriptor().getVarFields();
	setEntity_TemplateKeys_ToJson(target,lst);
}
void EntityDao::putHiddenVarsToTemplate(JsonObject& templ, Entity* e) {
	std::list<int> lst =e->descriptor().getHiddenFields();
	setEntity_const_ToJson(templ, HIDDEN_VAlUE_DEFAULT, lst);
}

void EntityDao::loadEntityStatVar(JsonObject& loaded, JsonObject& model, Entity* e) {
	std::list<int> lst =e->descriptor().getLoadFields();

	setEntity_all_FromJson(loaded,lst);
	setEntity_all_ToJson(model,lst);
}

void EntityDao::saveEntityStatVar(JsonObject& save, JsonObject& model, Entity* e) {
	std::list<int> lst =e->descriptor().getSaveFields();
	setEntity_all_ToJson(save,lst);
}

void EntityDao::setEntityStatVar(JsonObject& source, JsonObject& model,Entity* e) {
	std::list<int> lst =e->descriptor().getSetFields();
	setEntity_all_FromJson(source,lst);
	setEntity_all_ToJson(model,lst);
}


