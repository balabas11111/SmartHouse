/*
 * AbstractSensor_AlarmService.cpp
 *
 *  Created on: Feb 12, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <services/alarm/AbstractSensor_AlarmService.h>

AbstractSensor_AlarmService::AbstractSensor_AlarmService(long minAlarmInterval) {
	this->alarmMode=false;
	this->minAlarmInterval=minAlarmInterval*1000;

	this->lastAlarmTime=0;
}

AbstractSensor_AlarmService::~AbstractSensor_AlarmService() {
	// TODO Auto-generated destructor stub
}

String AbstractSensor_AlarmService::processAlarms(AbstractSensor** sensors,uint8_t sensorsSize) {
	    unsigned long now=millis();

		if(minAlarmInterval==0){
			return "";
		}

		if(alarmMode && (lastAlarmTime+minAlarmInterval>now)){
			return "";
		}

		String alarmMessage="";
		boolean alarmStarted=alarmMode;

		for(uint8_t i=0;i<sensorsSize;i++){
			boolean alarm=sensors[i]->checkForAlarm();

			if(alarm){
				if(!alarmStarted){
					alarmStarted=true;
				}
				alarmMode=true;
				alarmMessage+=sensors[i]->generateAlarmText();
			}
		}

		if(alarmMessage!=""){
			if(alarmStarted){
				Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_STARTED));
			}else{
				Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_IDENTIFIED));
			}
			Serial.println(alarmMessage);
			lastAlarmTime=now;
		}else{
			Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_NOT_IDENTIFIED));

			if(alarmMode){
				Serial.println(FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_FINISHED));
				alarmMode=false;
				alarmMessage=FPSTR(MESSAGE_DEVICE_HELPER_ALARM_MODE_FINISHED_RESULT);
				lastAlarmTime=now;
			}
		}
		if(alarmMessage!=""){
			Serial.println(alarmMessage);
		}

		return alarmMessage;
}
