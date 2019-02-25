/*
 * EntityDao.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_ENTITYDAO_H_
#define LIBRARIES_DEVICELIB_ENTITY_ENTITYDAO_H_

#include <entity/model/Entity.h>
#include <entity/model/EntityFieldDao.h>
#include <utils/FileUtils.h>
#include <utils/JsonObjectUtil.h>
#include <list>

class EntityDao {
public:
	EntityDao(Entity* ent[], int size);
	EntityDao();
	virtual ~EntityDao();

	void init();
	void initEntity(int id,Entity* e);

	void processEntityFieldChangeEvent(EntityField* field);
	void processStaticEntityFieldChangeEvent(int entityId,const char* key,const char* val);

	String processor(const String& var);

	Entity* getEntityByName(const char* groupName,const char* entityName);

	Entity* getEntityById(int entityId);

	JsonObject& getRootEntityJson(const char* path,const char* group, const char* name);
	JsonObject& getTmpEntityJson(const char* path,const char* group, const char* name);

	bool updateRootEntity(const char* path,const char* group, const char* name,JsonObject& from);
	bool updateTmpEntity(const char* path,const char* group, const char* name,JsonObject& from);

	void printJson();

	std::list<Entity*> entities;
	DynamicJsonBuffer objBuff;

	JsonObject& root = this->objBuff.parse("{\"root\":{}}").asObject().get<JsonObject>("root");

	DynamicJsonBuffer tmpBuf;
	JsonObject& tmp = this->objBuff.parse("{\"root\":{}}").asObject().get<JsonObject>("root");

	bool getChanged();

protected:
	void getExistsOrCreateNewByEkey(JsonObject& parent,const char* key);
	void getExistsOrCreateNew(JsonObject& parent,const char* key[]);
	void existsOrCreateEntityPath(JsonObject& store,const char* rootPath,Entity* entity);
	void existsOrCreateEntityPathByName(JsonObject& store,const char* rootPath,const char* group,const char* name);

	JsonObject& getEntity(JsonObject& rootObject,const char* path,const char* group, const char* name);
	bool updateEntity(JsonObject& rootObject, JsonObject& from,const char* path,const char* group, const char* name);

	void saveTemplateToDeployPath(JsonObject& root);

	void setEntity_all_FromJson(JsonObject& json,std::list<int>& lst);
	void setEntity_all_ToJson(JsonObject& json,std::list<int>& lst);
	void setEntity_stat_ToJson(JsonObject& json,std::list<int>& lst);
	void setEntity_var_ToJson(JsonObject& json,std::list<int>& lst);
	void setEntity_TemplateKeys_ToJson(JsonObject& json,std::list<int>& lst);

	void setEntity_const_ToJson(JsonObject& json,const char* constVal,std::list<int>& lst);

	void putEntityStatToModelTemplate(JsonObject& json,Entity* e);
	void putEntityVarsToModel(JsonObject& model,Entity* e);
	void putEntityVarsToTemplate(JsonObject& templ,Entity* e);

	void putHiddenVarsToTemplate(JsonObject& templ,Entity* e);

	void loadEntityStatVar(JsonObject& loaded,JsonObject& model,Entity* e);

	void saveEntityStatVar(JsonObject& save,JsonObject& model,Entity* e);

	void setEntityStatVar(JsonObject& set,JsonObject& model,Entity* e);

	EntityFieldDao entityFieldDao;

	//constants
	const char* modelPath="model";
	const char* templateDeployPath="template";

	const char* templateSavePath = "/data/entity/deployed/";
	const char* modelSavePath    = "/data/entity/stored/data.json";
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_ENTITYDAO_H_ */
