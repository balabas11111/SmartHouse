/*
 * EntityConsts.h
 *
 *  Created on: 3 мар. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIB_ENTITYCONSTS_H_
#define LIBRARIES_ENTITYLIB_ENTITYCONSTS_H_

#include <Arduino.h>

#define PERSENT_STR "%"

#define ROOT_PATH_MODEL "model"
#define ROOT_PATH_DATA "data"
#define ROOT_PATH_DEPLOYED "deploy"
#define ROOT_PATH_TEMP "temp"

#define ROOT_GROUP_SENSORS "sensors"
#define ROOT_GROUP_SETTINGS "settings"
#define ROOT_GROUP_SERVICES "services"

#define JSONKEY_stat "stat"
#define JSONKEY_var "var"
#define JSONKEY_templateVar "templateVar"
#define JSONKEY_load "load"
#define JSONKEY_save "save"
#define JSONKEY_deploy "deploy"
#define JSONKEY_set "set"

#define JSONKEY_id      "id"
#define JSONKEY_name    "name"
#define JSONKEY_group   "group"
#define JSONKEY_data    "data"
#define JSONKEY_fields  "fields"
#define JSONKEY_actions "actions"
#define JSONKEY_model   "model"
#define JSONKEY_descr   "descr"
#define JSONKEY_path    "path"
#define JSONKEY_dao     "dao"
#define JSONKEY_class   "class"
#define JSONKEY_open    "open"

#define JSONKEY_declaredFields  "declaredFields"
#define JSONKEY_declaredActions "declaredActions"

#define PATH_ROOT_file   "/data/entity/root.json"
#define PATH_MODEL_file   "/data/entity/model.json"
#define PATH_DATA_file    "/data/entity/data.json"
#define PATH_DEPLOY_file   "/data/entity/deploy.json"
#define PATH_DEPLOY_folder "/deploy/"

#define JSON_EXT              ".json"
#define JSON_DEFAULT_FILENAME "_data.json"

const char* const ROOT_PATHS[] PROGMEM = {
		ROOT_PATH_MODEL,
		ROOT_PATH_DEPLOYED
};

const int ROOT_PATHS_TOTAL PROGMEM = 2;

const char* const ROOT_PATHS_GROUPS[] PROGMEM = {
		ROOT_GROUP_SENSORS,
		ROOT_GROUP_SETTINGS,
		ROOT_GROUP_SERVICES
};

const int ROOT_PATHS_GROUPS_TOTAL PROGMEM = 3;

class EntityConsts {
public:
	virtual ~EntityConsts(){};
};

#endif /* LIBRARIES_ENTITYLIB_ENTITYCONSTS_H_ */
