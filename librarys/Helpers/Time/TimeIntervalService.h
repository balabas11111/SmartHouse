/*
 * TimeIntervalService.h
 *
 *  Created on: 24 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_TIME_TIMEINTERVALSERVICE_H_
#define LIBRARIES_HELPERS_TIME_TIMEINTERVALSERVICE_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "Time/NtpTimeClientService.h"
#include "EspSettingsBox.h"

#include "interfaces/Initializable.h"

#include "interfaces/Loopable.h"
#include "interfaces/EntityService.h"

#include "FunctionalInterrupt.h"
#include "DS3231.h"
#include "TimeTrigger.h"
#include "projectConsts.h"

#include "EspSettingsUtil.h"

#define SEC_IN_DAY 24*60*60
#define SEC_IN_WEEK 7*24*60*60

const char TimeIntervalService_EMPTY_DAYS[] PROGMEM ="0,0,0,0,0,0,0";

const char TimeIntervalService_ID[] PROGMEM ="123";
const char TimeIntervalService_NAME[] PROGMEM ="timeIntervalService";
const char TimeIntervalService_FileName_NAME[] PROGMEM ="timeIntServ";
const char TimeIntervalService_DESCR[] PROGMEM ="Service to schedule periodic executions";

const char* const IntervalCompNow_Names[] PROGMEM={
		"NOW_IN_FUTURE_OF_INTERVAL", "NOW_IN_INTERVAL", "NOW_IN_PAST_OF_INTERVAL"
};

typedef enum {
  NOW_IN_FUTURE_OF_INTERVAL, NOW_IN_INTERVAL, NOW_IN_PAST_OF_INTERVAL
} IntervalCompNow;

typedef enum {
  ONCE, PERIODIC, DAILY, MULTIDAILY, WEEKLY, MONTHLY, QUATERLY
} IntervalType;

typedef enum {
  NEW, WAIT, ACTIVE, FINISHED, TO_DELETE, RESCHEDULE
} IntervalState;

typedef struct timeIntervalDetail{
  uint8_t id;
  String name;
  uint8_t type;
  uint32_t startTime;
  uint32_t endTime;
  uint32_t time;
  uint8_t state;
  String days;
  uint8_t kind;
}TimeIntervalDetail;

const PROGMEM char MESSAGE_ERROR_TIME_INTERVAL_SERVICE_UPDATE[]        = "Error update time Interval";
const PROGMEM char MESSAGE_TIME_INTERVAL_STATUS_OK[]                   = "{\"status\":\"Ok\",\"message\":\"Ok\"}";
const PROGMEM char MESSAGE_TIME_INTERVAL_STATUS_ERROR[]                = "{\"status\":\"Error\",\"item\":\"Error update time Interval\"}";
const PROGMEM char MESSAGE_TIME_INTERVAL_STATUS_ERROR_MISSING_PARAMS[] = "{\"status\":\"Error\",\"item\":\"Required params missing\"}";

class TimeIntervalService: public Initializable, public Loopable, public EntityService {
public:
	virtual ~TimeIntervalService(){};

	TimeIntervalService(EspSettingsBox* espSettingsBox,NtpTimeClientService* ntpTimeClientService,
			std::function<void(TimeIntervalDetail)> externalFunction,
			std::function<void(void)> onTimeReceivedFunction,
			uint8_t fixedItemlength){
		this->espSettingsBox=espSettingsBox;
		this->timeService=ntpTimeClientService;
		this->waitForTimeReceive=false;
		this->externalFunction=externalFunction;
		this->onTimeReceivedFunction=onTimeReceivedFunction;
		this->fixedItemlength=fixedItemlength;
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

	uint8_t getEntityId() override{
		return Entity_timeIntervals;
	}

	virtual boolean loop(){
		if(waitForTimeReceive && isTimeServiceHasTime()){
			finishInit();
		}

		processIntervals();

		return true;
	}

	ulong now(){
		return timeService->getNow();
	}

	boolean finishInit(){
		Serial.println(FPSTR("---Finish TimeIntervalService initialization"));
		boolean loaded=loadFromFile();

		if(onTimeReceivedFunction!=nullptr){
			onTimeReceivedFunction();
		}

		processIntervals();

		waitForTimeReceive=false;

		if(!loaded){

			addDefaultTestInterval();
			saveToFile();
			Serial.println(FPSTR("Default interval complete"));
		}

		return true;
	}

	void addDefaultTestInterval(){
		Serial.println(FPSTR("Default interval will be added"));
		uint32_t testTime=timeService->getNow()+120;
		add("Test interval",ONCE,testTime,testTime+120,0,"",0);
	}

	TimeIntervalDetail getInterval(uint8_t ind){
		if(!isInitComplete() || ind>=itemCount){
			ind=itemCount;
		}

		return items[ind];
	}

	boolean isNotIntervalIdValid(uint8_t id){
		return ((fixedItemlength!=0 && itemCount>fixedItemlength) || id>itemCount);
	}

	virtual int getAbstractItems(JsonArray& itemsJson,uint8_t pageId) override{

		Serial.print(FPSTR("TimeIntervalService::getAbstractItems pageId="));
		Serial.print(pageId);
		Serial.print(FPSTR(" pageName="));
		Serial.println(PAGE_NAME[pageId]);

		long start=millis();

		Serial.println();
		JsonObject& item=itemsJson.createNestedObject();

		switch(pageId){
			case Page_listVal:
			case Page_list:{
				item["id"]=3;
				item["name"]=FPSTR(TimeIntervalService_NAME);
				break;
			}
			default:{
				return HTTP_CODE_NOT_IMPLEMENTED;
			}
		}

		if(itemCount==0){
			return HTTP_CODE_OK;
		}

		Serial.print(FPSTR("Process timeIntervals count="));
		JsonArray& itemChilds=item.createNestedArray(FPSTR(DEFAULT_CHILDREN_TAG));

		Serial.println(itemCount);

		for(uint8_t i=0;i<itemCount;i++){
			JsonObject& child=itemChilds.createNestedObject();

			Serial.print(FPSTR("child = "));
			Serial.println(i);

			child["id"]=items[i].id;
			child["name"]=items[i].name;
			child["type"]=items[i].type;
			child["startTime"]=items[i].startTime;
			child["endTime"]=items[i].endTime;
			child["time"]=items[i].time;
			child["state"]=items[i].state;
			child["days"]=items[i].days;
			child["kind"]=items[i].kind;
		}

		long total=millis()-start;

		Serial.print(FPSTR("timeIntervalService::getAbstractItems get="));
		Serial.println(total);

		return HTTP_CODE_OK;
	}

	virtual int postAbstractItems(JsonArray& items,uint8_t pageId) override{
		return HTTP_CODE_NOT_IMPLEMENTED;
	}

	void add(String name,IntervalType type,uint32_t start,uint32_t end,uint16_t time,String days,uint8_t kind){
		if(fixedItemlength!=0 && itemCount==fixedItemlength){
			return;
		}
		if(days==NULL || days.length()==0){
			days=FPSTR(TimeIntervalService_EMPTY_DAYS);
		}
		updateInterval(-1,name,type,start,end,time,WAIT,days,kind);
	}

	boolean updateInterval(int8_t id,String name,uint8_t type,uint32_t start,uint32_t end,uint16_t time,uint8_t state,String days,uint8_t kind){
		boolean isNew=false;
		if(id<0){
			Serial.println(FPSTR("new Interval add"));
			id=itemCount;
			isNew=true;
		}

		if(isNotIntervalIdValid(id)){
			return false;
		}

		boolean noerror=validateInterval(id,name,type,start,end,time,state,days,kind);

		ulong now=timeService->getNow();

		if(noerror){
			items[id].id=id;
			items[id].name=name;
			items[id].type=type;
			items[id].startTime=start;
			items[id].endTime=end;
			items[id].time=type==PERIODIC?time:0;
			items[id].state=state;
			items[id].days=type==MULTIDAILY?days:"0,0,0,0,0,0,0";
			items[id].kind=kind;

			if(compareIntervalAndNow(now,items[id])==NOW_IN_FUTURE_OF_INTERVAL){
				Serial.println(FPSTR("state updated to WAIT"));
				items[id].state=WAIT;
			}
			//
			if(isNew){
				Serial.print(FPSTR("New Interval "));
				Serial.println(getItemJson(id));
				itemCount++;
			}
		}

		return noerror;

	}

	boolean deleteInterval(uint8_t id,boolean doSaveToFile){
		if(id>=itemCount){
			return false;
		}

		if(id!=itemCount-1){
			for(uint8_t i=id;i<itemCount;i++){
				items[i]=items[i+1];
				items[i].id=i;
			}
		}

		itemCount--;

		if(doSaveToFile){
			saveToFile();
		}

		return true;
	}

	boolean validateInterval(int8_t id,String name,uint8_t type,uint32_t start,uint32_t end,uint16_t time,uint8_t state,String days,uint8_t kind){
		boolean result=true;

		if(!(id>=0 && id<=itemCount)){
			result=false;
			Serial.print(FPSTR("id out of range"));
			Serial.print(FPSTR("id="));
			Serial.println(id);
		}
		if(!(name!=NULL && name.length()>0)){
			result=false;
			Serial.print(FPSTR("name is wrong"));
			Serial.print(FPSTR(", name="));
			Serial.println(name);
		}
		if(!(type>=0 && type<sizeof(IntervalType_Names))){
			result=false;
			Serial.print(FPSTR("type out of range"));
			Serial.print(FPSTR(", type="));
			Serial.println(type);
		}
		if(!(start<end)){
			result=false;
			Serial.print(FPSTR("start<end"));
			Serial.print(FPSTR(", start="));
			Serial.print(start);
			Serial.print(FPSTR(", end="));
			Serial.println(end);
		}
		if(!(state>=0 && state<sizeof(IntervalState_Names))){
			result=false;
			Serial.print(FPSTR("state out of range"));
			Serial.print(FPSTR(", state="));
			Serial.println(state);
		}
		if(!(days!=NULL && days.length()==13)){
			result=false;
			Serial.print(FPSTR("days is wrong"));
			Serial.print(FPSTR(", days="));
			Serial.println(days);
		}
		if(!(kind>=0 && state<sizeof(IntervalKind_Names))){
			result=false;
			Serial.print(FPSTR("kind out of range"));
			Serial.print(FPSTR(", kind="));
			Serial.println(kind);
		}

		if(!result){
			Serial.println(FPSTR("FAILED updateInterval("));
		}

		return result;
	}

	StatusMessage processJson(String page,String json){
		Serial.print(FPSTR("timeIntervalService  page="));
		Serial.print(page);
		Serial.println(FPSTR(" val="));
		Serial.println(json);

		if(page==FPSTR("delete")){
			return deleteIntervalById(json.toInt());
		}

		if(page==FPSTR("intervals")){
			return updateIntervalFromJson(json);
		}

		if(page==FPSTR("reschedule")){
			uint8_t r=stopAndRescheduleInterval(json.toInt());
			switch (r){
				case -1:
					return StatusMessage(STATUS_CONF_ERROR_INT,FPSTR("Неверный индекс"));
				case 0:
					return StatusMessage(STATUS_NOT_FOUND_INT,FPSTR("Интервал неактивен"));
				default:
					return StatusMessage(STATUS_OK_ACCEPTED_INT);
			}
		}
		return StatusMessage(STATUS_NOT_IMPLEMENTED_INT,page);
	}

	StatusMessage deleteIntervalById(uint8_t id){
		if(deleteInterval(id,true)){
			return StatusMessage(STATUS_OK_DELETED_INT);
		}
		return StatusMessage(STATUS_FAILED_INT);
	}

	StatusMessage updateIntervalFromJson(String json){

		boolean ok=false;
		boolean isNew=false;
		StatusMessage sm=StatusMessage(STATUS_UNKNOWN_INT);
		String name="";

		if(json.length()>0){
			StaticJsonBuffer<1024> jsonBuffer;
			delay(1);

			JsonObject& root = jsonBuffer.parseObject(json);

			if(root.success()){

				int8_t id=root["id"];
				name=root["name"].as<char*>();
				uint8_t type=root["type"];
				uint32_t startTime=root["startTime"];
				uint32_t endTime=root["endTime"];
				uint32_t time=root["time"];
				uint8_t state=root["state"];
				String days=root["days"].as<char*>();
				uint8_t kind=root["kind"];

				ok=updateInterval(id,name,type,startTime,endTime,time,state,days,kind);

				isNew=(id<0);

				if(!ok){
					sm.setStatusCode(STATUS_UPDATE_ERROR_INT);
				}
			}else{
				sm.setStatusCode(STATUS_PARSE_ERROR_INT);
			}
		}else{
			sm.setStatusCode(STATUS_INVALID_LENGTH_INT);
		}

		if(ok && saveToFile()){
			sm.setStatusCode(STATUS_OK_INT);
			String msg=(isNew?"Создан новый ":"Обновлен ")+name;

			sm.setMessage(msg);
		}

		return sm;
	}

	boolean hasActiveAlarms(int8_t kind){
		for(uint8_t i=0;i<itemCount;i++){
			if(items[i].state==ACTIVE && items[i].kind==kind){
				return true;
			}
		}
		return false;
	}

	uint8_t getActiveAlarmsCount(){
		uint8_t c=0;
		for(uint8_t i=0;i<itemCount;i++){
			if(items[i].state==ACTIVE){
				c++;
			}
		}
		return c;
	}

	int8_t stopAndRescheduleInterval(uint8_t i){
		if(i>=itemCount){
			return -1;
		}
		if(items[i].state==ACTIVE){
			items[i].state=RESCHEDULE;
			return 1;
		}
		return 0;
	}

	uint8_t stopAndRescheduleAll(int8_t kind){
		Serial.println(FPSTR("All active alarms will be rescheduled"));
		uint8_t c=0;
		for(uint8_t i=0;i<itemCount;i++){
			if(kind<0 || items[i].kind==kind){
				if(stopAndRescheduleInterval(i)==1){
					c++;
				}
			}
		}
		return c;
	}

	void printTimeInterval(TimeIntervalDetail time){
		Serial.println(getItemJson(time));
	}

	boolean loadFromFile(){

		if(!espSettingsBox->isSettingsFileExists(FPSTR(TimeIntervalService_FileName_NAME))){
			Serial.println(FPSTR("---TimeInterval file NOT exists---"));
			return false;
		}

		String fileName=EspSettingsUtil::getExtraSettingsBoxFilePath(FPSTR(TimeIntervalService_FileName_NAME));

		Serial.print(FPSTR("Load TimeIntervals from file "));
		Serial.println(fileName);

		DynamicJsonBuffer jsonBuffer;
		delay(1);

		File file = SPIFFS.open(fileName, "r");
		size_t size = file.size();

		std::unique_ptr<char[]> buf (new char[size]);
		file.readBytes(buf.get(), size);
		JsonObject& root = jsonBuffer.parseObject(buf.get());

		if (!root.success()) {
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_ERROR_PARSE_JSON));
			return false;
		  }else{
			Serial.println();
			Serial.println(FPSTR(MESSAGE_ESPSETTINGSBOX_READ_FROM_FILE_COMPLETE));
			String vals="";
			root.printTo(vals);
			Serial.println(vals);

			itemCount=root["itemCount"];

			Serial.print(FPSTR("IntervalCount="));
			Serial.println(itemCount);

			for(uint8_t i=0;i<itemCount;i++){
				if(fixedItemlength==0 || i<fixedItemlength){
					items[i].id=i;
					items[i].name=root["intervals"][i]["name"].as<char*>();
					items[i].type=root["intervals"][i]["type"];
					items[i].state=root["intervals"][i]["state"];
					items[i].startTime=root["intervals"][i]["startTime"];
					items[i].endTime=root["intervals"][i]["endTime"];
					items[i].time=root["intervals"][i]["time"];
					items[i].days=root["intervals"][i]["days"].as<char*>();
					items[i].kind=root["intervals"][i]["kind"];
				}
				Serial.println(getItemJson(i));
			}

			if(fixedItemlength!=0){
				itemCount=fixedItemlength;
			}

			Serial.println(FPSTR("TimeIntervalService load complete"));
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		}

		  file.close();
		  return true;
	}

	boolean saveToFile(){
		Serial.println();
		String fileName=EspSettingsUtil::getExtraSettingsBoxFilePath(FPSTR(TimeIntervalService_FileName_NAME));
		return false;//EspSettingsUtil::saveStringToFile(fileName,getJson());
	}

	String getStateName(uint8_t id){
		return String(FPSTR(IntervalState_Names[id]));
	}
protected:
	void processIntervals(){
		if(itemCount==0){
			return;
		}
		boolean doSave=false;

		uint8_t dispatched=0;
		uint8_t rescheduled=0;
		uint8_t updated=0;
		uint8_t deleted=0;

		for(int8_t i=(itemCount-1);i>-1;i--){
			ulong now=timeService->getNow();

			boolean reschedule=false;
			boolean dispatch=false;
			boolean update=false;
			boolean delet=false;

			IntervalCompNow compRes=compareIntervalAndNow(now,items[i]);

			uint8_t state=items[i].state;

			if(compRes==NOW_IN_PAST_OF_INTERVAL && items[i].type==ONCE){
				if(state==ACTIVE){
					setState(i,FINISHED);
					dispatch=true;
				}
				delet=true;
			}else{
				switch(state){
					case RESCHEDULE:
						{
							Serial.print(FPSTR("state RESCHEDULE ind="));
							Serial.println(i);

							dispatch=true;
							reschedule=true;

							break;
						}
					case TO_DELETE:
						{
							Serial.print(FPSTR("state TO_DELETE ind="));
							Serial.println(i);
							delet=true;
							break;
						}
					case NEW:
						{	switch(compRes){
								case NOW_IN_PAST_OF_INTERVAL:
										Serial.print(FPSTR("NEW is in past ind="));
										Serial.println(i);
										setState(i,FINISHED);
										reschedule=true;
										break;
								case NOW_IN_INTERVAL:
										Serial.print(FPSTR("NEW is in interval ind="));
										Serial.println(i);
										setState(i,ACTIVE);
										dispatch=true;
										break;
								case NOW_IN_FUTURE_OF_INTERVAL:
										Serial.print(FPSTR("NEW is in future ind="));
										Serial.println(i);
										setState(i,WAIT);
										update=true;
										break;
							}
							break;
						}
					case WAIT:
						{	switch(compRes){
								case NOW_IN_PAST_OF_INTERVAL:
										Serial.print(FPSTR("WAIT is in past ind="));
										Serial.println(i);
										setState(i,FINISHED);
										reschedule=true;
										break;
								case NOW_IN_INTERVAL:
										Serial.print(FPSTR("WAIT is in interval ind="));
										Serial.println(i);
										setState(i,ACTIVE);
										dispatch=true;
										break;
							}
							break;
						}
					case ACTIVE:
						{	switch(compRes){
								case NOW_IN_PAST_OF_INTERVAL:
										Serial.print(FPSTR("ACTIVE is in past ind="));
										Serial.println(i);
										setState(i,FINISHED);
										dispatch=true;
										reschedule=true;
										break;
								case NOW_IN_FUTURE_OF_INTERVAL:
										Serial.print(FPSTR("ACTIVE is in future ind="));
										Serial.println(i);
										setState(i,WAIT);
										update=true;
										break;
							}
							break;
						}
					case FINISHED:
						{	switch(compRes){
								case NOW_IN_PAST_OF_INTERVAL:
										Serial.print(FPSTR("FINISHED is in past ind="));
										Serial.println(i);
										reschedule=true;
										break;
								case NOW_IN_INTERVAL:
										Serial.print(FPSTR("FINISHED is in interval ind="));
										Serial.println(i);
										setState(i,ACTIVE);
										dispatch=true;
										break;
								case NOW_IN_FUTURE_OF_INTERVAL:
										Serial.print(FPSTR("FINISHED is in future ind="));
										Serial.println(i);
										setState(i,WAIT);
										update=true;
										break;
							}
							break;
						}

					}//switch ended
			}

			doSave= reschedule || update || delet || doSave;

			if(update){
				updated++;
			}

			if(dispatch){
				dispatched++;
				dispatchExternalFunction(i);
			}

			if(delet){
				deleteInterval(i, false);
				deleted++;
			}

			if(reschedule){
				doSave=scheduleNextExecution(i) || doSave;
				rescheduled++;
			}

		}

		if(doSave){
			saveToFile();

			Serial.print(FPSTR(" deleted="));
			Serial.print(deleted);
			Serial.print(FPSTR(" updated="));
			Serial.print(updated);
			Serial.print(FPSTR(" rescheduled="));
			Serial.print(rescheduled);
			Serial.print(FPSTR(" dispatched="));
			Serial.println(dispatched);
		}
	}

	boolean scheduleNextExecution(uint8_t ind){

		boolean result=false;

		Serial.print("Before =");
		printTimeInterval(items[ind]);

		if(items[ind].type==ONCE){
			items[ind].state=TO_DELETE;
			result= true;
		}
		if(items[ind].type==PERIODIC){
			items[ind].startTime+=items[ind].time;
			items[ind].endTime+=items[ind].time;
			items[ind].state=WAIT;
			result= true;
		}
		if(items[ind].type==DAILY){
			items[ind].startTime+=SEC_IN_DAY;
			items[ind].endTime+=SEC_IN_DAY;
			items[ind].state=WAIT;
			result= true;
		}
		if(items[ind].type==MULTIDAILY){
			int8_t days[7]={0,0,0,0,0,0,0};

			String d=items[ind].days;
			uint8_t lastPos=0;

			for(uint8_t i=0;i<d.length();i++){
				if(d.substring(i,i+1)==","){
					uint8_t day=d.substring(lastPos,i-1).toInt();
					days[day]=1;
				}
			}

			for(uint8_t i=0;i<7;i++){
				Serial.print(days[i]);
			}

			Serial.println();

			DateTime dt1=DateTime(items[ind].startTime);
			uint8_t todayDay=dt1.dayOfTheWeek();

			Serial.print("todayDay=");
			Serial.println(todayDay);

			int nextDay=-1;

			for(uint8_t i=todayDay+1;i<7;i++){
				if(days[i]==1){
					nextDay=i;
					break;
				}
			}

			if(nextDay==-1){
				for(uint8_t i=0;i<=todayDay;i++){
					if(days[i]==1){
						nextDay=i;
						break;
					}
				}
			}

			Serial.print("nextDay=");
			Serial.println(nextDay);

			uint8_t dayDif=0;

			if(nextDay>todayDay){
				dayDif=nextDay-todayDay;
			}else{
				dayDif=6-todayDay+nextDay+1;
			}

			Serial.print("dayDif=");
			Serial.println(dayDif);

			items[ind].startTime+=dayDif*SEC_IN_DAY;
			items[ind].endTime+=dayDif*SEC_IN_DAY;
			items[ind].state=WAIT;
			result= true;
		}
		if(items[ind].type==WEEKLY){
			items[ind].startTime+=7*SEC_IN_DAY;
			items[ind].endTime+=7*SEC_IN_DAY;
			items[ind].state=WAIT;
			result= true;
		}
		if(items[ind].type==MONTHLY){

			DateTime dt1= DateTime(items[ind].startTime);


			uint newMonth=dt1.month();
			uint16_t newYear=dt1.year();

			if(newMonth==12){
				newMonth=1;
				newYear+=1;
			}else{
				newMonth+=1;
			}

			DateTime dt2=DateTime(newYear,newMonth,dt1.day(),dt1.hour(),dt1.minute(),dt1.second());

			uint32_t increment=dt2.unixtime()-items[ind].startTime;

			items[ind].startTime+=increment;
			items[ind].endTime+=increment;
			items[ind].state=WAIT;
			result= true;
		}

		Serial.print("After  =");
		printTimeInterval(items[ind]);

		return result;
	}

	void dispatchExternalFunction(uint8_t ind){
		Serial.print(FPSTR("dispatch "));
		Serial.println(getItemJson(ind));

		boolean started=items[ind].state==ACTIVE;

		if(externalFunction!=nullptr){
			externalFunction(items[ind]);
		}
	}

	void printDateTime(uint32_t interval){
		DateTime dt1=DateTime(interval);
		Serial.print(dt1.getFormattedIsoDateTime());
	}
private:

	uint8_t fixedItemlength=0;
	boolean waitForTimeReceive=false;
	EspSettingsBox* espSettingsBox;
	NtpTimeClientService* timeService;

	std::function<void(TimeIntervalDetail)> externalFunction;
	std::function<void(void)> onTimeReceivedFunction;

	uint8_t itemCount=0;
	TimeIntervalDetail items[ALARM_SIZE];

	String getItemJson(){
		String result=FPSTR(TimeIntervalService_ServVals);
			result+=FPSTR(TimeIntervalService_ServLocalVals);
			result+=",\"itemCount\":\""+String(itemCount)+"\",\"now\":\""+String(now())+"\"";

			return result;
	}

	String getItemJson(uint8_t index){
		return getItemJson(items[index]);
	}

	String getItemJson(TimeIntervalDetail item){
		return "{\"id\":\""+String(item.id)+"\","
					+"\"name\":\""+item.name+"\","
					//+"\"type\":\""+getTypeName(item.type)+"\","
					+"\"type\":\""+String(item.type)+"\","
					//+"\"state\":\""+getStateName(item.state)+"\","
					//+"\"rescheduleIndex\":\""+String(item.state==RESCHEDULE_INTERVAL_INDEX)+"\","
					+"\"state\":\""+(item.state)+"\","
					+"\"startTime\":\""+String(item.startTime)+"\","
					+"\"endTime\":\""+String(item.endTime)+"\","
					+"\"time\":\""+String(item.time)+"\","
					+"\"days\":\""+item.days+"\","
					+"\"kind\":\""+String(item.kind)+"\","
					+"\"startDateTime\":\""+timeService->getDateTimeAsTimeStamp(item.startTime)+"\","
					+"\"endDateTime\":\""+timeService->getDateTimeAsTimeStamp(item.endTime)
					+"\"}";/*,"
						+"\"startDateTime\":"+timeService->getDateTimeAsJson(item.startTime)+","
						+"\"endDateTime\":"+timeService->getDateTimeAsJson(item.endTime)
					+"}";*/
	}


	String getTypeName(uint8_t id){
		return String(FPSTR(IntervalType_Names[id]));
	}

	void setState(uint8_t ind,IntervalState newState){
		items[ind].state=newState;
	}

	IntervalCompNow compareIntervalAndNow(ulong now,TimeIntervalDetail interval){
		if(interval.endTime>now && interval.startTime>now){
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
