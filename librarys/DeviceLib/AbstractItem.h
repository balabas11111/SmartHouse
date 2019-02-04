/*
 * AbstractSensor.h
 *
 *  Created on: 30 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_AbstractItem_H_
#define LIBRARIES_DEVICELIB_AbstractItem_H_

#include <ItemFieldProviderService.h>
#include "Arduino.h"
#include "ESP_Consts.h"
#include "interfaces/JSONprovider.h"
#include "interfaces/DeviceLibable.h"
//#include <interfaces/ItemFieldDescriptor.h>
#include "interfaces/ItemFieldProvider.h"

class AbstractItem: public JSONprovider, public ItemFieldProvider {

	struct childRecords{
			uint8_t id;
			String name;
			String type;
			String size;
			String descr;
			float val;

			uint8_t fieldId;
			float minVal;
			float maxVal;
			String queue;
			boolean setAllowed;
		};
	typedef struct childRecords SensorValue;

public:

	AbstractItem(uint8_t id,String name,String type,String size,String descr, uint8_t itemCount){
		this->id=id;
		this->name=name;
		this->type=type;
		this->size=size;
		this->descr=descr;
		this->itemCount=itemCount;

		initializeChildren();
	}
	virtual ~AbstractItem(){};

	virtual String getKind()=0;

	virtual const uint8_t* getItemFieldsIds() override{
		return ITEM_FIELDS_THINGSPEAK;
	}

	SensorValue* getItems(){
		return items;
	}

	SensorValue getItem(uint8_t index){
		return items[index];
	}

	virtual void update(){};

	virtual boolean loop(){return false;};

	virtual String getJsonForSave(){
		return getJson();
	}

	virtual String getJson(){
		String result="{"+getItemJson()+",\"items\":[";

			for(uint8_t i=0;i<itemCount;i++){
				result+=getSensorValueJson(items[i],i);
				if(i!=itemCount-1){
					result+=",";
				}
			}

			result+="]}";

		return result;
	}

	virtual String getSimpleJson(){
		String result="{"+getItemJson()+",\"items\":[";

					for(uint8_t i=0;i<itemCount;i++){
						result+=getSensorValueSimpleJson(items[i]);
						if(i!=itemCount-1){
							result+=",";
						}
					}

					result+="]}";

			return result;
	}

	virtual String getJson(uint8_t id){
		return getSensorValueJson(items[id],id);
	}

	uint8_t getId(){
		return this->id;
	}

	virtual String getName() override{
		return this->name;
	}

	boolean hasNoItems(){
		return itemCount==0;
	}
	uint8_t getItemCount(){
		return itemCount;
	}
	int8_t getChildItemIndexByName(String name){
		for(uint8_t i=0;i<itemCount;i++){
			if(items[i].name==name){
				return i;
			}
		}
		return -1;
	}

	void setDescr(String descr){
		this->descr=descr;
	}
	void setDescr(uint8_t child,String descr){
		if(itemCount>child){
			this->items[child].descr=descr;
		}
	}

	void setMinVal(uint8_t child,float minVal){
		if(itemCount>child){
			this->items[child].minVal=minVal;
		}
	}

	void setMaxVal(uint8_t child,float maxVal){
		if(itemCount>child){
			this->items[child].maxVal=maxVal;
		}
	}

	void setFieldId(uint8_t child,int8_t fieldId){
		if(itemCount>child){
			this->items[child].fieldId=fieldId;
		}
	}

	String getName(uint8_t index){
		return this->items[index].name;
	}

	uint8_t getFieldId(uint8_t index){
		return 	this->items[index].fieldId;
	}

	String getFieldIdStr(uint8_t index){
		return 	String(this->items[index].fieldId);
	}

	String getDescr(uint8_t index){
		return 	String(this->items[index].descr);
	}

	String getValStr(uint8_t index){
		return 	String(this->items[index].val);
	}

	uint8_t getValInt(){
		return 	this->items[0].val;
	}

	uint8_t getValInt(uint8_t index){
		return 	this->items[index].val;
	}

	boolean getSetAllowed(uint8_t index){
		return 	this->items[index].setAllowed;
	}

	void setQueue(uint8_t child,String queue){
		if(itemCount>child){
			this->items[child].queue=queue;
		}
	}

	String getQueue(uint8_t index){
		if(itemCount>index){
			return this->items[index].queue;
		}
		return "";
	}

	void setNonActiveSensorValue(String name,String descr,uint8_t fieldId,float minVal,float maxVal,String queue){
		Serial.println(FPSTR("Non Active value set not activated"));
	}

	boolean getPeriodicSend(){
		return periodicSend;
	}

	boolean getProcessValueFromMqtt(){
		return processValueFromMqtt;
	}

	boolean getAutoCreateChannel(){
		return autoCreateChannel;
	}

	String constructGetUrl(String baseUrl,String paramVal){
		if(!this->periodicSend){
				return "";
		}
			String result="";

			for(uint8_t i=0;i<this->itemCount;i++){
				uint8_t fieldId=this->getFieldId(i);
				if(fieldId!=0){
					result+=paramVal+this->getFieldIdStr(i)+"="+this->getValStr(i);
				}
			}

			if(result!=""){
				return baseUrl+result;
			}

			return "";
	}

	boolean processMqValue(String topic,String message){
		for(int i=0;i<itemCount;i++){
			if(items[i].queue==topic){
				if(items[i].setAllowed){
					return processMqVal(i,message);
				}else{
					return false;
				}
			}
		}

		return false;
	}

	void printValues(){
		Serial.print(FPSTR(MESSAGE_ABSTRACT_ITEM_NAME_EQ));
		Serial.print(name);
		Serial.print(FPSTR(MESSAGE_DOT_COMMA_SPACE));
		for(uint8_t i=0;i<itemCount;i++){
			Serial.print(items[i].name);
			Serial.print(FPSTR(MESSAGE_EQUALS));
			Serial.print(items[i].val);
			Serial.print(FPSTR(MESSAGE_DOT_COMMA_SPACE));
		}
		Serial.println();
	}

	boolean checkForAlarm(){
		boolean result=false;

		for(uint8_t i=0;i<itemCount;i++){
			result=checkItemForAlarm(i) || result;
		}

		return result;
	}

	String generateAlarmText(){
		if(!checkForAlarm()){
			return "";
		}

		String result="";

		for(uint8_t i=0;i<itemCount;i++){
			if(items[i].val>=items[i].maxVal){
				result+=constructAlarmMessage(" "+items[i].descr,FPSTR(MESSAGE_ABSTRACT_ITEM_CURRENT_VAL_EQ),items[i].val,FPSTR(MESSAGE_ABSTRACT_ITEM_MAX_SPECIFIED_VAL_EQ),items[i].maxVal);
			}
			if(items[i].val<=items[i].minVal){
				result+=constructAlarmMessage(" "+items[i].descr,FPSTR(MESSAGE_ABSTRACT_ITEM_CURRENT_VAL_EQ),items[i].val,FPSTR(MESSAGE_ABSTRACT_ITEM_MIN_SPECIFIED_VAL_EQ),items[i].maxVal);
			}
		}

		return result;
	}

	boolean getStatus(){
		return status;
	}

	int16_t getIntValByIndex(uint8_t index){
		return (int16_t)items[index].val;
	}

	String getIntStringValByIndex(uint8_t index){
		return String(getIntValByIndex(index));
	}

	String getIntStringValFromFloat(float valFlt){
		uint16_t intVal=(int16_t)valFlt;
		return String(intVal);
	}

protected:

	uint8_t id;
	String name;
	String type;
	String size;
	String descr;
	uint8_t itemCount;

	boolean periodicSend=true;
	boolean processValueFromMqtt=false;
	boolean autoCreateChannel=true;

	SensorValue* items;
	boolean status=false;

	virtual boolean processMqVal(uint8_t index,String value){
		Serial.print(FPSTR(MESSAGE_ABSTRACT_ITEM_INDEX_EQ));
		Serial.print(index);
		Serial.print(FPSTR(MESSAGE_ABSTRACT_ITEM_VAL_EQ));
		Serial.print(value);
		Serial.println(FPSTR(MESSAGE_ABSTRACT_ITEM_METHOD_NOT_IMPLEMENTED));
		return true;
	}

	String constructAlarmMessage(String preffix,String currValueStr,float val,String overValueStr,float overVal){
		return " "+preffix+currValueStr+String(val)+overValueStr+String(overVal)+"<br>";
	}

	boolean checkItemForAlarm(uint8_t index){
		if(items[index].val>=items[index].maxVal
				|| items[index].val<=items[index].minVal){
			return true;
		}
		return false;
	}

	void setSetAllowed(uint8_t index,boolean val){
		items[index].setAllowed=val;
	}

	String getItemJson(){
		return "\"id\":\""+String(id)+"\","
				+"\"name\":\""+name+"\","
				+"\"kind\":\""+getKind()+"\","
				+"\"type\":\""+type+"\","
				+"\"size\":\""+size+"\","
				+"\"descr\":\""+descr+"\","
				+"\"itemCount\":\""+String(itemCount)+"\""+getExtraJsonItem();
	}

	String getSensorValueJson(SensorValue m,uint8_t ind){
		return "{\"id\":\""+String(m.id)+"\","
				+"\"name\":\""+m.name+"\","
				+"\"type\":\""+m.type+"\","
				+"\"size\":\""+m.size+"\","
				+"\"descr\":\""+m.descr+"\","
				+"\"val\":\""+getSensorValForJson(m.val)+"\","
				+"\"minVal\":\""+String(m.minVal)+"\","
				+"\"maxVal\":\""+String(m.maxVal)+"\","
				+"\"fieldId\":\""+String(m.fieldId)+"\","
				+"\"queue\":\""+m.queue+"\""
				+getExtraJsonChild(ind)
				+getItemFieldProviderJson(getName(),m.name)+"}";
	}

	String getSensorValueSimpleJson(SensorValue m){
		return "{\"name\":\""+m.name+"\","
				+"\"val\":\""+getSensorValForJson(m.val)+"\","
				+"\"fieldId\":\""+String(m.fieldId)+"\"}";
	}

	virtual String getSensorValForJson(float val){
		return String(val);
	}

	virtual String getExtraJsonItem(){
		return "";
	}

	virtual String getExtraJsonChild(uint8_t ind){
		return "";
	}

	void initializeChildren(){
		if(itemCount>0)
			items=new SensorValue[itemCount];
	}


};

#endif /* LIBRARIES_DEVICELIB_AbstractItem_H_ */
