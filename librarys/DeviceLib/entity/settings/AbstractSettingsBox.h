/*
 * AbstractSettingsBox.h
 *
 *  Created on: 24 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ENTITY_SETTINGS_ABSTRACTSETTINGSBOX_H_
#define LIBRARIES_DEVICELIB_ENTITY_SETTINGS_ABSTRACTSETTINGSBOX_H_

#include <entity/model/Entity.h>

class AbstractSettingsBox:public Entity {
public:
	AbstractSettingsBox(const char* groupIn,const char* nameIn,const char* descrIn,const char* jsonConf)
		:Entity(groupIn, nameIn, descrIn){
		this->jsonConf=jsonConf;
	};
	virtual ~AbstractSettingsBox();
protected:
	const char* jsonConf;
};

#endif /* LIBRARIES_DEVICELIB_ENTITY_SETTINGS_ABSTRACTSETTINGSBOX_H_ */
