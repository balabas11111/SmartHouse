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
#include "AbstractitemReq.h"

const char FIELD_DESCR[] PROGMEM ="descr";
const char FIELD_MIN_VAL[] PROGMEM ="minVal";
const char FIELD_MAX_VAL[] PROGMEM ="maxVal";
const char FIELD_FIELDID[] PROGMEM ="fieldId";
const char FIELD_QUEUE[] PROGMEM ="queue";

const uint8_t FIELD_DESCR_ID    =1;
const uint8_t FIELD_MIN_ID      =2;
const uint8_t FIELD_MAX_ID      =3;
const uint8_t FIELD_FIELDID_ID  =4;
const uint8_t FIELD_QUEUE_ID    =5;


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
	String queue;
};

typedef struct childRecords SensorValue;

public:

	AbstractItem(uint8_t id,String name,String type,String size,String descr,float val, uint8_t childCount,float minVal,float maxVal){
		this->item.id=id;
		this->item.name=name;
		this->item.type=type;
		this->item.size=size;
		this->item.descr=descr;
		this->item.val=val;
		this->item.fieldId=0;
		this->item.minVal=minVal;
		this->item.maxVal=maxVal;
		this->item.queue="";
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

	String getQueue(){
		return item.queue;
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
				+"\"queue\":\""+item.queue+"\","
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
						+"\"queue\":\""+item.queue+"\","
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

	uint8_t getId(){
		return this->item.id;
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

	void setQueue(String queue){
		this->item.queue=queue;
	}

	void setQueue(uint8_t child,String queue){
		if(childCount>child){
			this->items[child].queue=queue;
		}
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

	boolean setFieldFromRequest(AbstractItemRequest req){

		if(item.id!=req.deviceId){
			return false;
		}
		if(req.itemId!=255 && req.itemId>childCount){
			return false;
		}

		SensorValue* targetItem=&item;

		if(req.itemId!=255){
			targetItem=&items[req.itemId];
		}

		boolean result=false;

		switch(req.fieldId){
			case FIELD_DESCR_ID:
				targetItem->descr=req.fieldVal;
				result=false;
				break;
			case FIELD_MIN_ID:
				targetItem->minVal=req.fieldVal.toFloat();
				result=false;
				break;
			case FIELD_MAX_ID:
				targetItem->maxVal=req.fieldVal.toFloat();
				result=false;
				break;
			case FIELD_FIELDID_ID:
				targetItem->fieldId=req.fieldVal.toInt();
				result=false;
				break;
			case FIELD_QUEUE_ID:
				targetItem->queue=req.fieldVal;
				result=false;
				break;
			default:
				result=false;
		}

		return result;

	}

	//if(ietmId=255 then it is parentItem)
	boolean setField(String fieldName,String fieldVal,uint8_t itemId){
		if(itemId!=255 && itemId>childCount){
			return false;
		}
		SensorValue* targetItem=&item;

		if(itemId!=255){
			targetItem=&items[itemId];
		}

		if(fieldName==FPSTR(FIELD_DESCR)){
			targetItem->descr=fieldVal;
			return true;
		}else
		if(fieldName==FPSTR(FIELD_MIN_VAL)){
			targetItem->minVal=fieldVal.toFloat();
			return true;
		}else
		if(fieldName==FPSTR(FIELD_MAX_VAL)){
			targetItem->maxVal=fieldVal.toFloat();
			return true;
		}else
		if(fieldName==FPSTR(FIELD_FIELDID)){
			targetItem->fieldId=fieldVal.toInt();
			return true;
		}else
		if(fieldName==FPSTR(FIELD_QUEUE)){
			targetItem->queue=fieldVal;
			return true;
		}

		return false;
	}

	static AbstractItemRequest createitemRequest(String argName,String argValue){
		if(argName.startsWith("s_")){
			int8_t ind1=argName.indexOf("_", 2)+1;
			int8_t ind2=argName.indexOf("_",ind1+1);

			uint8_t deviceId=argName.substring(2,ind1-1).toInt();
			uint8_t itemId=argName.substring(ind1,ind2).toInt();
			uint8_t fieldId=argName.substring(ind2+1).toInt();

			/*
			Serial.print("argName=");
			Serial.print(argName);
			Serial.print(" argVal=");
			Serial.print(argVal);
			Serial.print(" deviceId=");
			Serial.print(deviceId);
			Serial.print(" sensorId=");
			Serial.print(sensorId);
			Serial.print(" type=");
			Serial.print(type);
			Serial.print(" ind1=");
			Serial.print(ind1);
			Serial.print(" ind2=");
			Serial.print(ind2);
			Serial.println(";");
			*/

			return {1,deviceId,itemId,fieldId,argValue};
		}

		return {0,0,0,0,""};
	}

protected:

	SensorValue item;

	uint8_t childCount;

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
				+"\"fieldId\":\""+String(m.fieldId)+"\","
				+"\"queue\";\""+m.queue+"\"}";
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
