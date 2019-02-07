/*
 * NtpTimeClientService.h
 *
 *  Created on: 28 Ð¾ÐºÑ‚. 2018 Ð³.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_NTPTIMECLIENTSERVICE_H_
#define LIBRARIES_HELPERS_NTPTIMECLIENTSERVICE_H_

#include "interfaces/Initializable.h"
#include "interfaces/JSONprovider.h"
#include "interfaces/Loopable.h"

#include "EspSettingsBox.h"
#include "ESPExtraSettingsBox.h"

#include "ESP_Consts.h"
#include "TimeTrigger.h"
#include "WiFiUdp.h"
#include "NTPClient.h"
#include "FunctionalInterrupt.h"

#include "projectConsts.h"

#include "DS3231.h"

#define YEAR0 1970
#define LEAP_YEAR(Y)     ( ((YEAR0+Y)>0) && !((YEAR0+Y)%4) && ( ((YEAR0+Y)%100) || !((YEAR0+Y)%400) ) )

const char NTP_TIME_CLIENT_SERVICE_NAME[] PROGMEM ="NtpTimeClientService";
const char NTP_TIME_CLIENT_SERVICE_TYPE[] PROGMEM ="NtpTime";
//-----------------println variables--------------------------
const char NTP_TIME_CLIENT_INIT_BEGIN[] PROGMEM ="-----Init Time Service-----";
const char NTP_TIME_CLIENT_FORCE_UPDATE_STATUS[] PROGMEM = "timeClient->forceUpdate() status=";
const char NTP_TIME_CLIENT_NO_NTP_SYNC_ENABLED[] PROGMEM = "No NTP sync enabled";
const char NTP_TIME_CLIENT_NO_LOCAL_TIME_DEVCE[] PROGMEM = "NO LocalTime device";
const char NTP_TIME_CLIENT_UPDATE_FAILED[] PROGMEM = "TimeUpdate FAILED";
const char NTP_TIME_CLIENT_NTP_SYNC_COMPLETE[] PROGMEM = "---NTP Sync complete---";
const char NTP_TIME_CLIENT_NTP_DETAILS[] PROGMEM = "---NTP details---";
const char NTP_TIME_CLIENT_RTC_DETAILS[] PROGMEM = "---RTC details---";
const char NTP_TIME_CLIENT_UNIX_TIME_EQ[] PROGMEM = " unixTime=";
const char NTP_TIME_CLIENT_DATE_TIME_EQ[] PROGMEM = " DateTime=";

class NtpTimeClientService: public Initializable, public Loopable,public JSONprovider {
public:

	NtpTimeClientService(EspSettingsBox* _espSettingsBox){
		construct(_espSettingsBox,nullptr);
	}

	NtpTimeClientService(EspSettingsBox* _espSettingsBox,std::function<void(void)> _externalFunction){//, ulong externalFunctionInterval){
		construct(_espSettingsBox,_externalFunction);
	}

	~NtpTimeClientService(){};

	virtual boolean initialize(boolean _init){
		if(_init){

			Serial.println(FPSTR(NTP_TIME_CLIENT_INIT_BEGIN));

			int boxIndex=espSettingsBox->getExtraBoxIndex(FPSTR(NTP_SETTINGS_BOX_NAME));

			String poolServerName=espSettingsBox->getExtraValueByBoxIndex(boxIndex, NTP_poolServerName);
			int timeOffset=espSettingsBox->getExtraValueIntByBoxIndex(boxIndex, NTP_timeOffset);
			int updateInterval=1000*espSettingsBox->getExtraValueIntByBoxIndex(boxIndex, NTP_updateInterval);
			ntpEnabled=espSettingsBox->getExtraValueBooleanByBoxIndex(boxIndex, NTP_enabled);

			if(updateInterval<MIN_UPDATE_INTERVAL){
				updateInterval=MIN_UPDATE_INTERVAL;
			}
#ifndef HAS_LOCAl_CLOCK
		Serial.println(FPSTR("NO Local clock device. Only NTP is used"));
#endif
			if(ntpEnabled){
				timeClient->setPoolServerName(poolServerName.c_str());
				timeClient->setTimeOffset(timeOffset);
				timeClient->setUpdateInterval(updateInterval);

				timeClient->printDetails();

				timeClient->begin();
				bool result=timeClient->forceUpdate();

				Serial.print(FPSTR(NTP_TIME_CLIENT_FORCE_UPDATE_STATUS));
				Serial.println(result);

				initialized=true;

				if(result){
					onNtpTimeReceived();
				}

				displayDetails();
			}else{
				Serial.println(FPSTR(NTP_TIME_CLIENT_NO_NTP_SYNC_ENABLED));
#ifdef HAS_LOCAl_CLOCK
				finishInitialization();
#endif
			}

			initialized=_init;
		}

		return initialized;
	}

	virtual boolean loop(){
		if(initialized){
			return update();
		}

		return false;
	}

	ulong getNow(){
#ifdef HAS_LOCAl_CLOCK
		return RTClib::now().unixtime();
#else
		return timeClient->getEpochTime();
#endif
	}

	boolean update(){
		if(!initialized || !ntpEnabled){
			return false;
		}

		bool result=timeClient->update();

		if(!result){
			Serial.println(FPSTR(NTP_TIME_CLIENT_UPDATE_FAILED));
			timeClient->printDetails();
			return false;
		}
		if(timeClient->udpStarted()){
			onNtpTimeReceived();
			return true;
		}
		return false;
	}

	String getDayOfWeekNameShort(uint8_t dow){
		return DAYS_OF_WEEK_SHORT[dow];
	}

	String getDayOfWeekName(uint8_t dow){
		return DAYS_OF_WEEK[dow];
	}

	int8_t* getCurrentTimePrepared(){
		parseTimeToCache();

		return currTimePrepared;
	}

	int8_t* getCurrentMonthDate(){
		parseDateToCache();

		return monthDate;
	}

	String getJson(){
#ifdef HAS_LOCAl_CLOCK
		DateTime dt=RTClib::now();
#else
		DateTime dt=DateTime(timeClient->getEpochTime());
#endif
		String result="{\"id\":\"-1\",\"name\":\"";
				result+=String(getName());
				result+="\",\"type\":\"";
				result+=String(getName());
				result+="\",";
#ifdef HAS_LOCAl_CLOCK
				result+="\"HAS_LOCAl_CLOCK\":\"1\",";
#else
				result+="\"HAS_LOCAl_CLOCK\":\"0\",";
#endif
				result+="\"dateTime\":";
				result+=getDateTimeAsJson(dt);
				result+="}";
		return result;
	}

	String getDateTimeAsJson(uint32_t unixTime){
		DateTime dt=DateTime(unixTime);
		return getDateTimeAsJson(dt);
	}

	String getDateTimeAsJson(DateTime dt){
		uint8_t dow=dt.dayOfTheWeek();

		return "{\"unixTime\":\""+String(dt.unixtime())+"\","
				+"\"year\":\""+String(dt.year())+"\","
				+"\"month\":\""+String(dt.month())+"\","
				+"\"day\":\""+String(dt.day())+"\","
				+"\"hour\":\""+String(dt.hour())+"\","
				+"\"minute\":\""+String(dt.minute())+"\","
				+"\"second\":\""+String(dt.second())+"\","
				+"\"dayOfTheWeek\":\""+String(dow)+"\","
				+"\"dayOfTheWeekNameShort\":\""+String(getDayOfWeekNameShort(dow))+"\","
				+"\"dayOfTheWeekName\":\""+String(getDayOfWeekName(dow))+"\""
			+"}";
	}

	String getDateTimeAsTimeStamp(uint32_t unixTime){
		DateTime dt=DateTime(unixTime);
		return dt.getFormattedIsoDateTime();
	}

	virtual String getName(){
		return FPSTR(NTP_TIME_CLIENT_SERVICE_NAME);
	}

	virtual void displayDetails(){
		if(!initialized){
			return;
		}
		Serial.print(FPSTR(NTP_TIME_CLIENT_NTP_DETAILS));
		Serial.print(FPSTR(NTP_TIME_CLIENT_UNIX_TIME_EQ));
		Serial.print(timeClient->getEpochTime());
		Serial.print(FPSTR(NTP_TIME_CLIENT_DATE_TIME_EQ));
		Serial.println(timeClient->getFormattedTime());
#ifdef HAS_LOCAl_CLOCK
		Serial.print(FPSTR(NTP_TIME_CLIENT_RTC_DETAILS));

		DateTime dt=RTClib::now();

		Serial.print(FPSTR(NTP_TIME_CLIENT_UNIX_TIME_EQ));
		Serial.print(dt.unixtime());
		Serial.print(FPSTR(NTP_TIME_CLIENT_DATE_TIME_EQ));
		Serial.println(dt.getFormattedIsoDateTime());
#endif
	}

	boolean isTimeReceived(){
		return timeReceived;
	}
protected:
	void onNtpTimeReceived(){
		Serial.println(FPSTR(NTP_TIME_CLIENT_NTP_SYNC_COMPLETE));
		timeClient->end();

		DateTime dt=DateTime(timeClient->getEpochTime());

	#ifdef HAS_LOCAl_CLOCK
		rtc.setYear(dt.year()-2000);
		rtc.setMonth(dt.month());
		rtc.setDate(dt.day());

		rtc.setHour(timeClient->getHours());
		rtc.setMinute(timeClient->getMinutes());
		rtc.setSecond(timeClient->getSeconds());

		Serial.println(FPSTR(NTP_TIME_CLIENT_RTC_DETAILS));
		Serial.print(FPSTR(NTP_TIME_CLIENT_DATE_TIME_EQ));
		Serial.println(RTClib::now().getFormattedIsoDateTime());
	#else
		Serial.println(FPSTR(NTP_TIME_CLIENT_NO_LOCAL_TIME_DEVCE));
	#endif
		Serial.println(getJson());
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		finishInitialization();
	}

	void finishInitialization(){
		timeReceived=true;

		if(externalFunction!=nullptr){
			externalFunction();
		}
	}
private:
#ifdef HAS_LOCAl_CLOCK
	DS3231  rtc;
#endif

	WiFiUDP ntpUDP;

	EspSettingsBox* espSettingsBox;
	NTPClient* timeClient;
	std::function<void(void)> externalFunction;
	boolean timeReceived=false;
	boolean ntpEnabled=true;

	int8_t currTimePrepared[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
	int8_t monthDate[4]={0x00,0x00,0x00,0x00};

	void construct(EspSettingsBox* _espSettingsBox,std::function<void(void)> _externalFunction){//, ulong externalFunctionInterval){
		Serial.println(FPSTR("Create time client"));

		this->timeClient=new NTPClient(ntpUDP,"pl.pool.ntp.org",7200,1000);
		this->espSettingsBox=_espSettingsBox;
		this->externalFunction=_externalFunction;

#ifdef HAS_LOCAl_CLOCK
		this->rtc.setClockMode(false);
#endif
	}

	void parseTimeToCache(){
#ifdef HAS_LOCAl_CLOCK
		bool h12;
		bool PM;

		uint8_t h=rtc.getHour(h12, PM);
		uint8_t m=rtc.getMinute();
		uint8_t s=rtc.getSecond();
#else
		timeClient->update();
		uint8_t h=timeClient->getHours();
		uint8_t m=timeClient->getMinutes();
		uint8_t s=timeClient->getSeconds();
#endif

		currTimePrepared[0] = h / 10;
		currTimePrepared[1] = h % 10;
		currTimePrepared[2] = m / 10;
		currTimePrepared[3] = m % 10;
		currTimePrepared[4] = s / 10;
		currTimePrepared[5] = s % 10;
	}

	void parseDateToCache(){
#ifdef HAS_LOCAl_CLOCK
		bool centure;
		uint8_t y=rtc.getYear();
		uint8_t m=rtc.getMonth(centure);
		uint8_t d=rtc.getDate();
#else
		timeClient->update();
		DateTime dt=DateTime(timeClient->getEpochTime());

		uint8_t y=dt.year();
		uint8_t m=dt.month();
		uint8_t d=dt.day();
#endif
		monthDate[0] = d/10;
		monthDate[1] = d%10;
		monthDate[2] = m/10;
		monthDate[3] = m%10;
	}
	/*
	 void printDateTimeDetails(DateTime dt){
		Serial.print("dt.unixtime()=");
		Serial.print(dt.unixtime());
		Serial.print(" dt.DateTime=");
		Serial.println(dt.getFormattedIsoDateTime());

		Serial.println(FPSTR("---Setup Local time-----"));
		Serial.print(FPSTR("NewTime "));
		Serial.print(dt.year());Serial.print(FPSTR("-"));
		Serial.print(dt.month());Serial.print(FPSTR("-"));
		Serial.print(dt.day());Serial.print(FPSTR(" "));
		Serial.print(timeClient->getHours());Serial.print(FPSTR(":"));
		Serial.print(timeClient->getMinutes());Serial.print(FPSTR("-"));
		Serial.println(timeClient->getSeconds());

		Serial.println("dt.year()");
		Serial.println(dt.year()-2000);
		Serial.println("dt.month()");
		Serial.println(dt.month());
		Serial.println("dt.day()");
		Serial.println(dt.day());
	}

	boolean parseTimeToCache(){
		boolean timeChanged=false;

		uint8_t h=timeClient->getHours();
		uint8_t m=timeClient->getMinutes();
		uint8_t s=timeClient->getSeconds();

		uint8_t tmpTime[TIME_LENGTH] = {0x00,0x00,0x00};
		int8_t tmpTimePrep[PREPARED_TIME_LENGTH] = {0x00,0x00,0x00,0x00,0x00,0x00};

		tmpTimePrep[INDEX_HOUR1] = h / 10;
		tmpTimePrep[INDEX_HOUR2] = h % 10;
		tmpTimePrep[INDEX_MINUTE1] = m / 10;
		tmpTimePrep[INDEX_MINUTE2] = m % 10;
		tmpTimePrep[INDEX_SECUNDA1] = s / 10;
		tmpTimePrep[INDEX_SECUNDA2] = s % 10;

		tmpTime[INDEX_HOUR] = h;
		tmpTime[INDEX_MINUTE] = m;
		tmpTime[INDEX_SECUNDA] = s;

		for(uint8_t i=0;i<PREPARED_TIME_LENGTH;i++){
			if(tmpTimePrep[i]!=currTimePrepared[i]){
				timeChanged=true;
			}

			currTimePrepared[i]=tmpTimePrep[i];
		}
		for(uint8_t i=0;i<TIME_LENGTH;i++){
			if(tmpTime[i]!=currTime[i]){
				timeChanged=true;
			}

			currTime[i]=tmpTime[i];
		}
		return timeChanged;
	}

	boolean parseDateToCache(){
		boolean dateUpdated=false;

		ulong time=getnow();

		uint32_t tmpDate[DATE_LENGTH] = {0x00,0x00,0x00,0x00,0x00};
		uint8_t tmpDatePrepared[PREPARED_DATE_LENGTH] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

		uint year = 0;
		uint month=0;
		uint days = 0;
		uint leapYear=0;
		uint dayOfWeek=0;

		time=time/60/60/24;

		dayOfWeek = ((time + 4) % 7);

		uint monthLength=0;
		while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
			year++;

		}
		leapYear = LEAP_YEAR(year);

		days -= LEAP_YEAR(year) ? 366 : 365;
		time  -= days;

		days=0;
		month=0;
		monthLength=0;

		for (month=0; month<12; month++) {
			if (month==1) {
				if (LEAP_YEAR(year)){monthLength=29;} else {monthLength=28;}
			} else {
			  monthLength = monthDays[month];
			}

			if (time >= monthLength){ time -= monthLength;}else{break;}
		}

		year+=YEAR0;
		month+=1;
		time+=1;

		tmpDate[INDEX_YEAR] = year;
		tmpDate[INDEX_MONTH] = month;
		tmpDate[INDEX_DAY] = time;
		tmpDate[INDEX_DAY_OF_WEEK] = dayOfWeek;
		tmpDate[INDEX_LEAP_YEAR] = leapYear;

		tmpDatePrepared[INDEX_YEAR1] = year / 1000;
		tmpDatePrepared[INDEX_YEAR2] = (year % 1000)/100;
		tmpDatePrepared[INDEX_YEAR3] = (year % 1000)%100/10;
		tmpDatePrepared[INDEX_YEAR4] = (year % 1000)%100%10;

		tmpDatePrepared[INDEX_MONTH1] = month / 10;
		tmpDatePrepared[INDEX_MONTH2] = month % 10;

		tmpDatePrepared[INDEX_DAY1] = time / 10;
		tmpDatePrepared[INDEX_DAY2] = time % 10;

		tmpDatePrepared[INDEX_DAY_OF_WEEK_PREP] = dayOfWeek;
		tmpDatePrepared[INDEX_LEAP_YEAR_PREP] = leapYear;

		for(uint8_t i=0;i<PREPARED_DATE_LENGTH;i++){
			if(tmpDatePrepared[i]!=currDatePrepared[i]){
				dateUpdated=true;
			}

			currDatePrepared[i]=tmpDatePrepared[i];
		}

		for(uint8_t i=0;i<DATE_LENGTH;i++){
			if(tmpDate[i]!=currDate[i]){
				dateUpdated=true;
			}

			currDate[i]=tmpDate[i];
		}
		return dateUpdated;
	}
*/
	 /*
		Serial.print("boxIndex=");
		Serial.println(boxIndex);
		Serial.print("poolServerName=");
		Serial.println(poolServerName);
		Serial.print("=poolServerName.c_str() =");
		Serial.println(poolServerName.c_str());
		Serial.print("=");
		Serial.print("timeOffset =");
		Serial.println(timeOffset);
		Serial.print("updateInterval=");
		Serial.println(updateInterval);
		Serial.print("triggerInterval=");
		Serial.println(triggerInterval);

		String getCurrentTimeAsString(const char divider){
			String result=String(currTimePrepared[INDEX_HOUR1])
					+String(currTimePrepared[INDEX_HOUR2])
					+String(divider)
					+String(currTimePrepared[INDEX_MINUTE1])
					+String(currTimePrepared[INDEX_MINUTE2])
					+String(divider)
					+String(currTimePrepared[INDEX_SECUNDA1])
					+String(currTimePrepared[INDEX_SECUNDA2]);

			return result;
		}

		String getCurrentTimeNoSecondsAsString(const char divider){
			String result=String(currTimePrepared[INDEX_HOUR1])
					+String(currTimePrepared[INDEX_HOUR2])
					+String(divider)
					+String(currTimePrepared[INDEX_MINUTE1])
					+String(currTimePrepared[INDEX_MINUTE2]);

			return result;
		}

		String getCurrentDateAsString(const char divider){
			return String(getYear())+String(divider)
				   +String(getMonth())+String(divider)
				   +String(getDayOfMonth())+String(divider)
				   +String(getLeapYear())+String(divider)
				   +String(getDayOfWeekNameShort())+String(divider);
		}

		String getTimeStamp(){
		update();
		String result="";

		for(uint8_t i=0;i<=INDEX_DAY_OF_WEEK;i++){
			result+=String(currDate[i]);
			result+=FPSTR(CALENDAR_DATE_DIVIDER);
		}

		result.setCharAt(result.length(), ' ');

		for(uint8_t i=0;i<TIME_LENGTH;i++){
			result+=String(currTime[i]);
			result+=FPSTR(CALENDAR_DATE_DIVIDER);
		}

		result.remove(result.length());

		return result;
	}

	ulong DateTimeToEpochTime(uint16_t year,uint8_t month,uint8_t day,uint8_t hour,uint8_t minute,uint8_t seconds){

		uint8_t monthLength=monthDays[month];
		uint16_t yearLength=(LEAP_YEAR(year))?366:355;

		if (month==1) { // february
			if (LEAP_YEAR(year)){monthLength=29;} else {monthLength=28;}
		}

		ulong result=seconds+minute*60+hour*60*60+day*24*60*60+monthLength*24*60*60+yearLength*24*60*60;

		return result;
	}
	*/
};

#endif /* LIBRARIES_HELPERS_NTPTIMECLIENTSERVICE_H_ */
