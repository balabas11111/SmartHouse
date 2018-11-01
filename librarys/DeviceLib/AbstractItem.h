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


		init();
	}
	virtual ~AbstractItem(){};

	virtual void update(){};

	virtual bool loop()=0;

	virtual String getJson(){

		String result=
				"{\"id\":\""+String(item.id)+"\","
				+"\"name\":\""+item.name+"\","
				+"\"type\":\""+item.type+"\","
				+"\"size\":\""+item.size+"\","
				+"\"descr\":\""+item.descr+"\","
				+"\"count\":\""+String(item.val)+"\","
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

protected:

	SensorValue item;

	boolean initialized;

	uint8_t childCount;
	String queueName;

	SensorValue items[];

	virtual void init()=0;

	String getSensorValueJson(SensorValue m){
		return "{\"id\":\""+String(m.id)+"\","
				+"\"name\":\""+m.name+"\","
				+"\"type\":\""+m.type+"\","
				+"\"size\":\""+m.size+"\","
				+"\"descr\":\""+m.descr+"\","
				+"\"count\":\""+String(m.val)+"\","
				+"\"fieldId\":\""+String(m.fieldId)+"\"}";
	}

};

#endif /* LIBRARIES_DEVICELIB_AbstractItem_H_ */
