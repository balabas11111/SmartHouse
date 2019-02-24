/*
 * EntityDao.h
 *
 *  Created on: 23 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_ENTITYDAO_H_
#define LIBRARIES_DEVICELIB_ENTITY_ENTITYDAO_H_

#include <entity/model/Entity.h>
#include <entity/model/EntityTable.h>
#include <utils/FileUtils.h>
#include <list>

class EntityDao {
public:
	EntityDao(Entity* ent[], int size);
	EntityDao();
	virtual ~EntityDao();

	void init();

	void processDispatchedChange(int entityIdIndex);

	String processor(const String& var);

	Entity* getEntityByName(const char* groupName,const char* entityName);

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

	void saveTemplateToDeployPath(JsonObject& root);

	EntityTable table;

	//constants

	const char* modelPath="model";
	const char* templateDeployPath="template";

	const char* templateSavePath = "/data/entity/deployed/";
	const char* modelSavePath    = "/data/entity/stored/data.json";
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_ENTITYDAO_H_ */
