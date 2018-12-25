/*
 * TimeIntervalService.h
 *
 *  Created on: 24 дек. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_TIME_TIMEINTERVALSERVICE_H_
#define LIBRARIES_HELPERS_TIME_TIMEINTERVALSERVICE_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Time/NtpTimeClientService.h"
#include "EspSettingsBox.h"
#include "Loopable.h"
#include "Initializable.h"
#include "FunctionalInterrupt.h"

#define ALARM_SIZE 10

const char* const IntervalState_Names[] PROGMEM={
		"NEW", "WAIT", "ACTIVE", "FINISHED", "TO_DELETE", "MISSED", "FAILED"
};
const char* const IntervalCompNow_Names[] PROGMEM={
		"NOW_IN_FUTURE_OF_INTERVAL", "NOW_IN_INTERVAL", "NOW_IN_PAST_OF_INTERVAL"
};
const char* const IntervalType_Names[] PROGMEM={
		"ONCE", "PERIODIC", "DAILY", "MULTIDAILY", "WEEKLY", "MONTHLY", "QUATERLY"
};

const char TimeIntervalService_ID[] PROGMEM ="123";
const char TimeIntervalService_NAME[] PROGMEM ="TimeIntervalService";
const char TimeIntervalService_DESCR[] PROGMEM ="Service to schedule periodic executions";

typedef enum {
  NOW_IN_FUTURE_OF_INTERVAL, NOW_IN_INTERVAL, NOW_IN_PAST_OF_INTERVAL
} IntervalCompNow;

typedef enum {
  ONCE, PERIODIC, DAILY, MULTIDAILY, WEEKLY, MONTHLY, QUATERLY
} IntervalType;

typedef enum {
  NEW, WAIT, ACTIVE, FINISHED, TO_DELETE, MISSED, FAILED
} IntervalState;

typedef struct timeIntervalDetail{
  uint8_t id;
  String name;
  IntervalType type;
  ulong startTime;
  ulong endTime;
  IntervalState state;
  String days;
  String param;
}TimeIntervalDetail;

class TimeIntervalService: public Initializable, public Loopable {
public:
	virtual ~TimeIntervalService(){};

	TimeIntervalService(EspSettingsBox* espSettingsBox,NtpTimeClientService* ntpTimeClientService,std::function<void(TimeIntervalDetail)> externalFunction){
		this->espSettingsBox=espSettingsBox;
		this->timeService=ntpTimeClientService;
		this->waitForTimeReceive=false;
		this->externalFunction=externalFunction;
	}

	virtual boolean initialize(boolean _init){
		Serial.println(FPSTR("---Init TimeIntervalService---"));
		if(!isTimeServiceHasTime()){
			if(_init){
				Serial.println(FPSTR("Wait for TimeService"));
				waitForTimeReceive=true;
			}
			return false;
		}else{
			return finishInit();
		}

		return initialized;
	}

	virtual boolean loop(){
		if(waitForTimeReceive && isTimeServiceHasTime()){
			finishInit();
		}

		processIntervals();

		return true;
	}

	ulong now(){
		return timeService->now();
	}

	boolean finishInit(){
		Serial.println(FPSTR("---Finish TimeIntervalService initialization"));
		loadFromFile();
		processIntervals();

		waitForTimeReceive=false;
		return true;
	}

	TimeIntervalDetail getInterval(uint8_t ind){
		if(!isInitComplete() || index>=itemCount){
			return NULL;
		}

		return items[ind];
	}

	virtual String getJson(){
		String result="{"+getItemJson()+",\"items\":[";

			for(uint8_t i=0;i<itemCount;i++){
				result+=getItemJson(i);
				if(i!=itemCount-1){
					result+=",";
				}
			}

			result+="]}";

		return result;
	}

	virtual void add(){

	}

protected:
	boolean loadFromFile(){

		if(!espSettingsBox->isSettingsFileExists(FPSTR(TimeIntervalService_NAME))){
			Serial.println(FPSTR("---TimeInterval file NOT exists---"));
			return false;
		}

		String fileName=espSettingsBox->getSettingsFileFileName(FPSTR(TimeIntervalService_NAME));

		Serial.print(FPSTR("Load TimeIntervals from file "));
		Serial.println(fileName);

     	StaticJsonBuffer<1024> jsonBuffer;
		delay(1);

		File file = SPIFFS.open(fileName, "r");
		size_t size = file.size();

		std::unique_ptr<char[]> buf (new char[size]);
		file.readBytes(buf.get(), size);
		JsonObject& root = jsonBuffer.parseObject(buf.get());

		if (!root.success()) {
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
		  }else{
			Serial.println();
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_READ_FROM_FILE_COMPLETE));
			String vals="";
			root.printTo(vals);
			Serial.println(vals);

			itemCount=root["itemCount"];

			Serial.print(FPSTR("itemCount="));
			Serial.println(itemCount);

			for(uint8_t i=0;i<itemCount;i++){
				items[i].id=i;
				items[i].name=root["items"][i]["name"].as<char*>();
				items[i].type=root["items"][i]["typeInt"];
				items[i].state=root["items"][i]["type"];
				items[i].startTime=root["items"][i]["minVal"];
				items[i].endTime=root["items"][i]["maxVal"];
				items[i].days=root["items"][i]["days"];
				items[i].param=root["items"][i]["param"];

				Serial.println(getItemJson(i));
			}

			Serial.println(FPSTR("TimeIntervalService load complete"));
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		}

		  file.close();
		  return true;
	}

	void saveToFile(){
		Serial.println();
		espSettingsBox->saveSettingToFile(FPSTR(TimeIntervalService_NAME),getJson());
	}

	void processIntervals(){
		boolean doSave=false;

		uint8_t dispatched=0;
		uint8_t rescheduled=0;
		uint8_t updated=0;

		for(uint8_t i=0;i<itemCount;i++){
			ulong now=now();

			boolean reschedule=false;
			boolean dispatch=false;
			boolean update=false;

			switch(items[i].state){
				case NEW: reschedule=true; break;
				case WAIT:
					{	IntervalCompNow compRes=compareIntervalAndNow(now,items[i]);
						switch(compRes){
							case NOW_IN_PAST_OF_INTERVAL:
									setState(i,FINISHED);
									reschedule=true;
									break;
							case NOW_IN_INTERVAL:
									setState(i,ACTIVE);
									dispatch=true;
									break;
						}
						break;
					}
				case ACTIVE:
					{	IntervalCompNow compRes=compareIntervalAndNow(now,items[i]);
						switch(compRes){
							case NOW_IN_PAST_OF_INTERVAL:
									setState(i,FINISHED);
									reschedule=true;
									dispatch=true;
									break;
						}
						break;
					}
				case FINISHED:
					{	IntervalCompNow compRes=compareIntervalAndNow(now,items[i]);
						switch(compRes){
							case NOW_IN_PAST_OF_INTERVAL:
									reschedule=true;
									break;
							case NOW_IN_INTERVAL:
									setState(i,ACTIVE);
									dispatch=true;
									break;
							case NOW_IN_FUTURE_OF_INTERVAL:
									setState(i,WAIT);
									update=true;
									break;
						}
						break;
					}

			}

			doSave=dispatch || reschedule || update;

			if(update){
				updated++;
			}

			if(dispatch){
				dispatched++;
				if(externalFunction!=nullptr){
					Serial.print(FPSTR("dispatch "));
					Serial.println(getItemJson(i));
					externalFunction(items[i]);
				}
			}

			if(reschedule){
				scheduleNextExecution(i);
				rescheduled++;
				Serial.print(FPSTR("reschedule "));
				Serial.println(getItemJson(i));
			}

		}

		if(doSave){
			saveToFile();

			Serial.print(FPSTR(" updated="));
			Serial.print(updated);
			Serial.print(FPSTR(" rescheduled="));
			Serial.print(rescheduled);
			Serial.print(FPSTR(" dispatched="));
			Serial.println(dispatched);
		}
	}

	boolean scheduleNextExecution(uint8_t ind){

		return true;
	}

private:
	boolean waitForTimeReceive=false;
	EspSettingsBox* espSettingsBox;
	NtpTimeClientService* timeService;

	std::function<void(TimeIntervalDetail)> externalFunction;

	uint8_t itemCount=0;
	TimeIntervalDetail items[ALARM_SIZE];

	String getItemJson(){
		return "\"id\":\"123\",\"name\":\"TimeIntervalService\",\"type\":\"ScheduleService\",\"size\":\"1\",\
				\"descr\":\"TimeIntervalService_DESCR\",\"itemCount\":\""+String(itemCount)+"\"";

	}

	String getItemJson(uint8_t index){
		TimeIntervalDetail item=items[index];

		return "{\"id\":\""+String(item.id)+"\","
					+"\"name\":\""+item.name+"\","
					+"\"type\":\""+getTypeName(item.type)+"\","
					+"\"typeInt\":\""+String(item.type)+"\","
					+"\"state\":\""+getStateName(item.state)+"\","
					+"\"stateInt\":\""+String(item.state)+"\","
					+"\"minVal\":\""+String(item.startTime)+"\","
					+"\"maxVal\":\""+String(item.endTime)+"\","
					+"\"days\":\""+item.days+"\","
					+"\"param\":\""+item.param+"\"}";
	}

	String getTypeName(IntervalType id){
		return String(FPSTR(IntervalType_Names[id]));
	}
	String getStateName(IntervalState id){
		return String(FPSTR(IntervalState_Names[id]));
	}

	void setState(uint8_t ind,IntervalState newState){
		items[ind].state=newState;
	}

	IntervalCompNow compareIntervalAndNow(ulong now,TimeIntervalDetail interval){
		if(interval.endTime<now){
			return NOW_IN_FUTURE_OF_INTERVAL;
		}

		if(interval.endTime>=now && now>=interval.startTime){
			return NOW_IN_INTERVAL;
		}

		return NOW_IN_PAST_OF_INTERVAL;
	}

	boolean isTimeServiceHasTime(){
		return timeService->isTimeReceived();
	}

	boolean isInitComplete(){
		return initialized && !waitForTimeReceive;
	}
};

#endif /* LIBRARIES_HELPERS_TIME_TIMEINTERVALSERVICE_H_ */
