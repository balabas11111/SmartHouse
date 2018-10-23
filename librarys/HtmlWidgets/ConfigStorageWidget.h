/*
 * ConfigStoragewidget.h
 *
 *  Created on: Oct 23, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HTMLWIDGETS_CONFIGSTORAGEWIDGET_H_
#define LIBRARIES_HTMLWIDGETS_CONFIGSTORAGEWIDGET_H_

#include "ConfigStorage.h"
#include "HtmlWidget.h"

class ConfigStorageWidget:public HtmlWidget {
public:
	ConfigStorageWidget(ConfigStorage* _configStrorage,String _name){
		name=_name;
		configStorage=_configStrorage;
	}

	virtual ~ConfigStorageWidget();

	String getName(){
		return name;
	}

	String executeClientAction(String actionName,String remoteId,String remoteVal, String className, String childClass,String clientData){

		printCommand(actionName, remoteId, remoteVal, className, childClass, clientData);

		if(actionName.equals(FPSTR(ACTION_GET_STATIC_SETTINGS_DATA))
				&&className.equals(CLASS_REFRESHABLE_SettingsWidgetESP)){

			return configStorage->get(remoteVal.c_str());
		}

		return getNotAllowed();
	}

private:
	String name;
	ConfigStorage* configStorage;
};

#endif /* LIBRARIES_HTMLWIDGETS_CONFIGSTORAGEWIDGET_H_ */
