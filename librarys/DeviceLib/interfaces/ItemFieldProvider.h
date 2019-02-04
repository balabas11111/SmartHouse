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
#include "interfaces/ItemFieldDescriptor.h"

const uint8_t ITEM_FIELDS_EMPTY[]      PROGMEM = {};

const uint8_t ITEM_FIELDS_THINGSPEAK[] PROGMEM = {IFP_ThingSpeak_ID};
const uint8_t ITEM_FIELDS_MINMAX_ALARM[] PROGMEM = {IFP_MinMaxAlarm_ID};

const uint8_t ITEM_FIELDS_THINGSP_MINMAX_ALARM[] PROGMEM = {IFP_ThingSpeak_ID,IFP_MinMaxAlarm_ID};

class ItemFieldProvider {
public:
	ItemFieldProvider(){};
	virtual ~ItemFieldProvider(){};

	void setItemFieldProviderService(ItemFieldProviderService* provideService){
		if(!hasItemFields()){
			Serial.println(FPSTR("No extra fields"));
			return;
		}
		this->provideService=provideService;
	}

	virtual String getItemFieldProviderJson(uint8_t pId,String aName,String sName){
		if(provideService==nullptr){
			Serial.print(FPSTR("ItemFIeldProvider service not specified for "));
			Serial.println(aName);
			return "";
		}
		return provideService->loadItemFieldProviderJsonById(pId, aName, sName);
	}

	virtual String getItemFieldProviderJson(String aName,String sName){
		if(provideService==nullptr){
			Serial.print(FPSTR("ItemFIeldProvider service not spefified for "));
			Serial.println(aName);
			return "";
		}
		return provideService->getItemFieldProviderJson(getItemFieldsIds(),aName, sName);
	}

	virtual ItemFieldDescriptor** getItemFieldDescriptors(){
		const uint8_t* ids=getItemFieldsIds();
		uint8_t size=ARRAY_SIZE(ids);

		if(size==0){
			return nullptr;
		}

		ItemFieldDescriptor* res[size];

		for(uint8_t i=0;i<size;i++){
			res[0]=provideService->getItemFieldsDescriptorById(ids[i]);
		}

		return res;
	}

	boolean hasItemFields(){
		return getItemFieldsCount()>0;
	}

	virtual uint8_t getItemFieldsCount(){
		return ARRAY_SIZE(getItemFieldsIds());
	}

	virtual const uint8_t* getItemFieldsIds()=0;

protected:
	ItemFieldProviderService* provideService;
};

#endif /* LIBRARIES_DEVICELIB_ITEMFIELDPROVIDER_H_ */
