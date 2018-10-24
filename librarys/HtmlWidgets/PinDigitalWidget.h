/*
 * ConfigStoragewidget.h
 *
 *  Created on: Oct 23, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_HTMLWIDGETS_PINDIGITALWIDGET_H_
#define LIBRARIES_HTMLWIDGETS_PINDIGITALWIDGET_H_

#include "PinDigital.h"
#include "HtmlWidget.h"

class PinDigitalWidget:public HtmlWidget {
public:
	PinDigitalWidget(PinDigital* _pin,String _ON_img,String _OFF_img){
		//name=_name;
		pin=_pin;
		ON_img=_ON_img;
		OFF_img=_OFF_img;
	}

	virtual ~PinDigitalWidget(){};

	String getName(){
		return pin->getName();
	}

	String executeClientAction(String actionName,String remoteId,String remoteVal, String className, String childClass,String clientData){

		printCommand(actionName, remoteId, remoteVal, className, childClass, clientData);

		if(actionName.equals(ACTION_SUBMIT_WIDGET_GET_VALUE)
				&&remoteVal.equals(REMOTE_VAL_IMAGE)){
			pin->change();
			return getPinsImage();
		}

		if(actionName.equals(FPSTR(ACTION_GET_WIDGETS_CHILDREN_AS_JSON))
				&&className.equals(CLASS_REFRESHABLE_CHILDREN_MeasurerWidgetESPJson)){
			if(pin->isOn()){
				return "{\"src\":\""+ON_img+"\"}";
			}else{
				return "{\"src\":\""+OFF_img+"\"}";
			}
		}

		return getNotAllowed();
	}

	String getPinsImage(){
		if(pin->isOn()){
			return ON_img;
		}else{
			return OFF_img;
		}
	}

	String getWsText(){
		return "{\"wsId\":\""+getName()+"\",\"wsClass\":\"wsItem\",\"wsValue\":\""+getPinsImage()+"\"}";
	}

private:
	//String name;
	PinDigital* pin;
	String ON_img;
	String OFF_img;
};

#endif /* LIBRARIES_HTMLWIDGETS_CONFIGSTORAGEWIDGET_H_ */
