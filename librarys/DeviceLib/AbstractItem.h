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

		Serial.println("AbstractItem ="+name+" count="+sizeof(items));
	}
	virtual ~AbstractItem(){};

	SensorValue* getItems(){
		return items;
	}

	SensorValue getItem(uint8_t index){
		return items[index];
	}



	virtual void update(){};

	virtual boolean loop(){return false;};

	virtual String getJson(){
		String result="{"+getItemJson()+",\"items\":[";

			for(uint8_t i=0;i<itemCount;i++){
				result+=getSensorValueJson(items[i]);
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
		return getSensorValueJson(items[id]);
	}

	uint8_t getId(){
		return this->id;
	}

	String getName(){
		return this->name;
	}

	uint8_t getItemCount(){
		return itemCount;
	}
	uint8_t getChildItemIndexByName(String name){
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

	boolean getPeriodicSend(){
		return periodicSend;
	}
/*
	boolean getPostValueOnChanged(){
		return postValueOnChanged;
	}

	boolean getPostValueToMqHttp(){
		return postValueToMqHttp;
	}
*/
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
		Serial.print("NAME=");
		Serial.print(name);
		Serial.print("; ");
		for(uint8_t i=0;i<itemCount;i++){
			Serial.print(items[i].name);
			Serial.print("=");
			Serial.print(items[i].val);
			Serial.print("; ");
		}
		Serial.println();
	}

	String getJsonPublishUrl(){
		return "/"+getName()+"/getJson";
	}

	String getSetValueUrl(){
		return "/"+getName()+"/setValue";
	}

	boolean setFieldFromRequest(AbstractItemRequest req){

		if(this->id!=req.deviceId || !req.valid){
			return false;
		}
		if(req.itemId!=255 && req.itemId>itemCount){
			return false;
		}

		if(req.itemId==255){
			//we can set only abstractItem desciption. All other values belong to its items values
			if(req.fieldId==FIELD_DESCR_ID){
				setDescr(req.fieldVal);
				return true;
			}
			return false;
		}

		SensorValue* targetItem=&items[req.itemId];

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
	boolean setField(String fieldName,String fieldVal){
		AbstractItemRequest req=AbstractItem::createitemRequest(fieldName,fieldVal);
		return setFieldFromRequest(req);
	}

	static AbstractItemRequest createitemRequest(String argName,String argValue){
		if(argName.startsWith("s_")){
			int8_t ind1=argName.indexOf("_", 2)+1;
			int8_t ind2=argName.indexOf("_",ind1+1);

			uint8_t deviceId=argName.substring(2,ind1-1).toInt();
			uint8_t itemId=argName.substring(ind1,ind2).toInt();
			uint8_t fieldId=argName.substring(ind2+1).toInt();

			return {1,deviceId,itemId,fieldId,argValue};
		}

		return {0,0,0,0,""};
	}

	boolean checkForAlarm(){
		boolean result=false;

		for(uint8_t i=0;i<itemCount;i++){
			result=checkItemForAlarm(i) || result;
		}

		alarmMode=result;

		return result;
	}

	String generateAlarmText(){
		checkForAlarm();

		if(!alarmMode){
			return "";
		}

		String result="";

		for(uint8_t i=0;i<itemCount;i++){
			if(items[i].val>=items[i].maxVal){
				result+=constructAlarmMessage(" "+items[i].descr," поточное значение =",items[i].val," максимально заданое =",items[i].maxVal);
			}
			if(items[i].val<=items[i].minVal){
				result+=constructAlarmMessage(" "+items[i].descr," поточное значение =",items[i].val," минимальное заданое =",items[i].maxVal);
			}
		}

		return result;
	}

protected:

	uint8_t id;
	String name;
	String type;
	String size;
	String descr;
	uint8_t itemCount;

	//allows periodic send of value into channel
	boolean periodicSend=true;
	//post values only when val was updated (marking PinDigital only)
	//boolean postValueOnChanged=false;
	//allows to post item vals to MQ
	//boolean postValueToMqHttp=false;
	//allows to process values from bind mqtt
	boolean processValueFromMqtt=false;
	//channel could be used as management for current item
	boolean autoCreateChannel=true;

	boolean alarmMode=false;

	SensorValue* items;

	virtual boolean processMqVal(uint8_t index,String value){

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
				+"\"type\":\""+type+"\","
				+"\"size\":\""+size+"\","
				+"\"descr\":\""+descr+"\","
				+"\"itemCount\":\""+String(itemCount)+"\"";
	}

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
				+"\"queue\":\""+m.queue+"\"}";
	}

	String getSensorValueSimpleJson(SensorValue m){
		return "{\"name\":\""+m.name+"\","
				+"\"val\":\""+String(m.val)+"\","
				+"\"fieldId\":\""+String(m.fieldId)+"\"}";
	}

	void initializeChildren(){
		if(itemCount>0)
			items=new SensorValue[itemCount];
	}

};

#endif /* LIBRARIES_DEVICELIB_AbstractItem_H_ */
