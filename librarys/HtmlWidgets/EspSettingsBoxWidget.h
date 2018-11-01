/*
 * ConfigStoragewidget.h
 *
 *  Created on: Oct 23, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HTMLWIDGETS_ESPSETTINGSBOXWIDGET_H_
#define LIBRARIES_HTMLWIDGETS_ESPSETTINGSBOXWIDGET_H_

#include "EspSettingsBox.h"
#include "HtmlWidget.h"

class EspSettingsBoxWidget:public HtmlWidget {
public:
	EspSettingsBoxWidget(String _id,EspSettingsBox* _configStrorage,String _name){
		id=_id;
		name=_name;
		espSettingsBox=_configStrorage;
	}

	virtual ~EspSettingsBoxWidget(){};

	String getName(){
		return name;
	}

	String executeClientAction(String actionName,String remoteId,String remoteVal, String className, String childClass,String clientData){

		printCommand(actionName, remoteId, remoteVal, className, childClass, clientData);

		if(actionName==(FPSTR(ACTION_GET_STATIC_SETTINGS_DATA))
				&&className==(FPSTR(CLASS_REFRESHABLE_SettingsWidgetESP))){

			String result="";

			if(remoteVal=="DeviceId"){
				result=String(espSettingsBox->DeviceId);
			}else
			if(remoteVal=="DeviceLocation"){
				result=String(espSettingsBox->DeviceLocation);
			}else
			if(remoteVal=="thSkChId"){
				result=String(espSettingsBox->thSkChId);
			}

			if(result!=""){
				return result;
			}
		}

		if(actionName==FPSTR(ACTION_GET_WIDGET_JSON)){
			String result="{\"m.id\":\""+String(id)+"\",\"m.name\":\""+name+"\",\"m.val\":\"espSettingsBox\",\"m.kind\":\"espSettingsBox\",\"m.descr\":\"espSettingsBox\",\
					\"itemsCount\":3,\"time\":\""+String(millis())+"\",\"items\":[";

				result+="{\"m.id\":\"DeviceId\",\"m.name\":\"DeviceId\",\"m.val\":\""+espSettingsBox->DeviceId+"\",\"m.kind\":\"espSettingsBox\",\"m.descr\":\"Идентификатор устройства Мак\"},";
				result+="{\"m.id\":\"DeviceLocation\",\"m.name\":\"DeviceLocation\",\"m.val\":\""+espSettingsBox->DeviceLocation+"\",\"m.kind\":\"espSettingsBox\",\"m.descr\":\"Размещение устройства\"},";
				result+="{\"m.id\":\"thSkChId\",\"m.name\":\"thSkChId\",\"m.val\":\""+String(espSettingsBox->thSkChId)+"\",\"m.kind\":\"espSettingsBox\",\"m.descr\":\"Идентификатор канала ThingSpeak\"}";

				result+="]}";


			return result;
		}

		return getNotAllowed();
	}

	String getWsText(){
		return "";
	}

private:
	String id;
	String name;
	EspSettingsBox* espSettingsBox;
};

#endif /* LIBRARIES_HTMLWIDGETS_CONFIGSTORAGEWIDGET_H_ */
