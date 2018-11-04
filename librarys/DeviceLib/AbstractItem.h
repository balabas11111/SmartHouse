/*
 * AbstractSensor.h
 *
 *  Created on: 30 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_AbstractItem_H_
#define LIBRARIES_DEVICELIB_AbstractItem_H_

#include "Arduino.h"
#include "AbstractConstants.h"

class AbstractItem {

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
};

typedef struct childRecords SensorValue;

public:
	AbstractItem(uint8_t id,String name,String type,String size,String descr,float val, uint8_t childCount,uint8_t fieldId,String queueName,float minVal,float maxVal){
		this->item.id=id;
		this->item.name=name;
		this->item.type=type;
		this->item.size=size;
		this->item.descr=descr;
		this->item.val=val;
		this->item.fieldId=fieldId;
		this->item.minVal=minVal;
		this->item.maxVal=maxVal;
		this->queueName=queueName;
		this->childCount=childCount;

		initializeChildren();

		Serial.println("AbstractItem ="+name+" count="+sizeof(items));
	}
	virtual ~AbstractItem(){};

	SensorValue getItem(){
		return item;
	}

	SensorValue* getItems(){
		return items;
	}

	String getQueueName(){
		return queueName;
	}

	virtual void update(){};

	virtual boolean loop(){return false;};

	virtual String getJson(){
		String result=
				"{\"id\":\""+String(item.id)+"\","
				+"\"name\":\""+item.name+"\","
				+"\"type\":\""+item.type+"\","
				+"\"size\":\""+item.size+"\","
				+"\"descr\":\""+item.descr+"\","
				+"\"val\":\""+String(item.val)+"\","
				+"\"fieldId\":\""+String(item.fieldId)+"\","
				+"\"childCount\":\""+String(childCount)+"\","
				+"\"queueName\":\""+queueName+"\","
				+"\"minVal\":\""+String(item.minVal)+"\","
				+"\"maxVal\":\""+String(item.maxVal)+"\","
				+"\"items\":[";

			for(uint8_t i=0;i<childCount;i++){
				result+=getSensorValueJson(items[i]);
				if(i!=childCount-1){
					result+=",";
				}
			}

			result+="]}";

		return result;
	}

	virtual String getSimpleJson(){
		String result="{\"name\":\""+item.name+"\","
						+"\"val\":\""+String(item.val)+"\","
						+"\"fieldId\":\""+String(item.fieldId)+"\","
						+"\"queueName\":\""+queueName+"\","
						+"\"minVal\":\""+String(item.minVal)+"\","
						+"\"maxVal\":\""+String(item.maxVal)+"\","
						+"\"items\":[";

					for(uint8_t i=0;i<childCount;i++){
						result+=getSensorValueSimpleJson(items[i]);
						if(i!=childCount-1){
							result+=",";
						}
					}

					result+="]}";

			return result;
	}

	virtual String getJson(uint8_t id){
		return getSensorValueJson(items[id]);
	}

	String getName(){
		return this->item.name;
	}

	uint8_t getChildCount(){
		return childCount;
	}
	uint8_t getChildItemIndexByName(String name){
		for(uint8_t i=0;i<childCount;i++){
			if(items[i].name==name){
				return i;
			}
		}
		return -1;
	}

	void setDescr(String descr){
		this->item.descr=descr;
	}
	void setDescr(uint8_t child,String descr){
		if(childCount>child){
			this->items[child].descr=descr;
		}
	}

	void setMinVal(float minVal){
		this->item.minVal=minVal;
	}
	void setMinVal(uint8_t child,float minVal){
		if(childCount>child){
			this->items[child].minVal=minVal;
		}
	}

	void setMaxVal(float maxVal){
		this->item.maxVal=maxVal;
	}
	void setMaxVal(uint8_t child,float maxVal){
		if(childCount>child){
			this->items[child].maxVal=maxVal;
		}
	}

	void setFieldId(int8_t fieldId){
		this->item.fieldId=fieldId;
	}

	void setFieldId(uint8_t child,int8_t fieldId){
		if(childCount>child){
			this->items[child].fieldId=fieldId;
		}
	}

	void setQueueName(String queueName){
		this->queueName=queueName;
	}

	void printValues(){
		Serial.print("NAME=");
		Serial.print(item.name);
		Serial.print("; ");
		for(uint8_t i=0;i<childCount;i++){
			Serial.print(items[i].name);
			Serial.print("=");
			Serial.print(items[i].val);
			Serial.print("; ");
		}
		Serial.println();
	}

	void setFieldIds(uint8_t* fieldsArray){
		item.fieldId=fieldsArray[0];

		for(uint8_t i=1;i<=childCount;i++){
			items[i-1].fieldId=fieldsArray[i];
		}
	}

	void setDescriptions(String* descrArray){
		item.descr=descrArray[0];

		for(uint8_t i=1;i<=childCount;i++){
			items[i-1].descr=descrArray[i];
		}
	}

	String getJsonPublishUrl(){
		return "/"+getName()+"/getJson";
	}

	String getSetValueUrl(){
		return "/"+getName()+"/setValue";
	}

protected:

	SensorValue item;

	uint8_t childCount;
	String queueName;

	SensorValue* items;

	String getSensorValueJson(SensorValue m){
		return "{\"id\":\""+String(m.id)+"\","
				+"\"name\":\""+m.name+"\","
				+"\"type\":\""+m.type+"\","
				+"\"size\":\""+m.size+"\","
				+"\"descr\":\""+m.descr+"\","
				+"\"val\":\""+String(m.val)+"\","
				+"\"minVal\":\""+String(m.minVal)+"\","
				+"\"maxVal\":\""+String(m.maxVal)+"\","
				+"\"fieldId\":\""+String(m.fieldId)+"\"}";
	}

	String getSensorValueSimpleJson(SensorValue m){
		return "{\"name\":\""+m.name+"\","
				+"\"val\":\""+String(m.val)+"\","
				+"\"fieldId\":\""+String(m.fieldId)+"\"}";
	}

	void initializeChildren(){
		if(childCount>0)
			items=new SensorValue[childCount];
	}

};

#endif /* LIBRARIES_DEVICELIB_AbstractItem_H_ */
