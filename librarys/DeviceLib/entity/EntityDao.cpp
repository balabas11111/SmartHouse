/*
 * EntityDao.cpp
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#include <entity/EntityDao.h>

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

	table.setFuncEvent([this](int ind){ processDispatchedChange(ind);});

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++){
			Entity* e=*it;
			Serial.print(e->getGroup());
			Serial.print(FPSTR("."));
			Serial.print(e->getName());
			Serial.println();

			e->init(id,&table);

			existsOrCreateEntityPath(root,this->modelPath,e);
			existsOrCreateEntityPath(root,this->templateDeployPath,e);

			JsonObject& entityModel = root.get<JsonObject>(this->modelPath)
											.get<JsonObject>(e->getGroup()).get<JsonObject>(e->getName());

			JsonObject& entityTemplate = root.get<JsonObject>(this->templateDeployPath)
											.get<JsonObject>(e->getGroup()).get<JsonObject>(e->getName());;

			e->init_Default_fields(entityModel, entityTemplate);
			e->init_fields(entityModel, entityTemplate);

			Serial.println();
			Serial.println(FPSTR("Load data"));

			existsOrCreateEntityPath(tmp,this->modelPath,e);
			existsOrCreateEntityPath(tmp,this->templateDeployPath,e);

			JsonObject& loadedModel = tmp.get<JsonObject>(this->modelPath)
											.get<JsonObject>(e->getGroup()).get<JsonObject>(e->getName());

			JsonObject& loadedTemplate = tmp.get<JsonObject>(this->templateDeployPath)
											.get<JsonObject>(e->getGroup()).get<JsonObject>(e->getName());;

			//current values can be overriden even in template
			e->prepare_fields_load(loadedModel,entityTemplate);

			e->save_Default_fields(entityTemplate,loadedModel);
			e->prepare_fields_save(entityTemplate, loadedModel);

			Serial.println(FPSTR("...done"));
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
			table.printValByEntityId(efi);
		}

		Serial.println();
	}*/

	//table.print();
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
	int entityIndexId=var.toInt();

	if(table.containsEntityId(entityIndexId)){
		return table.getTemplateValueByEntityId(entityIndexId);
	}

    return FPSTR("Not found");
}

void EntityDao::existsOrCreateEntityPath(JsonObject& store,const char* rootPath, Entity* entity) {
	getExistsOrCreateNewByEkey(store,rootPath);
	JsonObject& parent = store.get<JsonObject>(rootPath);

	getExistsOrCreateNewByEkey(parent,entity->getGroup());
	JsonObject& group = parent.get<JsonObject>(entity->getGroup());

	getExistsOrCreateNewByEkey(group,entity->getName());
	JsonObject& entityJson = group.get<JsonObject>(entity->getName());
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

void EntityDao::processDispatchedChange(int entityIdIndex) {
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
