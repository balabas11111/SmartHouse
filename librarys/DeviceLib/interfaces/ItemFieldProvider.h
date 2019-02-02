/*
 * ItemFieldProvider.h
 *
 *  Created on: 2 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ITEMFIELDPROVIDER_H_
#define LIBRARIES_DEVICELIB_ITEMFIELDPROVIDER_H_

#include "Arduino.h"
#include "ItemFieldProviderService.h"

class ItemFieldProvider {
public:
	ItemFieldProvider(){};
	virtual ~ItemFieldProvider(){};

	void setProvider(ItemFieldProviderService* provider){
		this->provider=provider;
	}

	virtual String getItemFieldProviderJson(uint8_t pId,String aName,String sName){
		return provider->loadItemFieldProviderJsonById(pId, aName, sName);
	}

	virtual String getItemFieldProviderJson(String aName,String sName){
		return provider->getItemFieldProviderJson(getItemFieldsProviders(),aName, sName);
	}

	boolean hasItemFields(){
		return getItemFieldsProvidersCount()>0;
	}

	virtual uint8_t getItemFieldsProvidersCount(){
		return ARRAY_SIZE(getItemFieldsProviders());
	}

	virtual const uint8_t* getItemFieldsProviders()=0;

protected:
	ItemFieldProviderService* provider;
};

#endif /* LIBRARIES_DEVICELIB_ITEMFIELDPROVIDER_H_ */
