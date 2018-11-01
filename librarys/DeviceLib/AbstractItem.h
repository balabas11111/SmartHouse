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
};

typedef struct childRecords SensorValue;

public:
	AbstractItem(uint8_t id,String name,String type,String size,String descr,float val, uint8_t childCount,uint8_t fieldId,String queueName){
		this->item.id=id;
		this->item.name=name;
		this->item.type=type;
		this->item.size=size;
		this->item.descr=descr;
		this->item.val=val;
		this->item.fieldId=fieldId;
		this->childCount=childCount;
		this->queueName=queueName;

		if(childCount>0)
		items=new SensorValue[childCount];
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
				+"\"fieldId\":\""+String(m.fieldId)+"\"}";
	}

	String getSensorValueSimpleJson(SensorValue m){
		return "{\"name\":\""+m.name+"\","
				+"\"val\":\""+String(m.val)+"\","
				+"\"fieldId\":\""+String(m.fieldId)+"\"}";
	}

};

#endif /* LIBRARIES_DEVICELIB_AbstractItem_H_ */
