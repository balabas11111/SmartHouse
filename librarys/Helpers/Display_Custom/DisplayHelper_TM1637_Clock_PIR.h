/*
 * DisplayHelper_TM1637_Clock_PIR.h
 *
 *  Created on: 22 дек. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_DISPLAY_CUSTOM_DISPLAYHELPER_TM1637_CLOCK_PIR_H_
#define LIBRARIES_HELPERS_DISPLAY_CUSTOM_DISPLAYHELPER_TM1637_CLOCK_PIR_H_

#include "Arduino.h"
#include "EspSettingsBox.h"
#include <ESP8266WiFi.h>
#include "BME280_Sensor.h"
#include "DS18D20_Sensor.h"
#include "NtpTimeClientService.h"
#include "TM1637.h"
#include "TimeTrigger.h"
#include "Loopable.h"

#define MODE_TIME_REFRESH_INTERVAL 500
#define MODE_DATE_REFRESH_INTERVAL 1000
#define MODE_OTHER_REFRESH_INTERVAL 30000

#define MODE_DISPLAY_AP_SAP_DELAY 1000
#define MODE_DISPLAY_WIFI_DELAY 1000

#define DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED 1000
#define SYMBOL_EMPTY 0x7f
#define SYMBOL_A 10
#define SYMBOL_DEGREE 60
#define SYMBOL_HUMIDITY 47
#define SYMBOL_PRESSURE 46
#define SYMBOL_MINUS 99

#define MODE_TIME 0
#define MODE_DATE 1
#define MODE_BME_TEMP 2
#define MODE_BME_HUMIDITY 3
#define MODE_BME_PRESSURE 4
#define MODE_DS18D20_TEMP 5

class DisplayHelper_TM1637_Clock_PIR: public Loopable {
public:
	DisplayHelper_TM1637_Clock_PIR(TM1637* timeDisplay,EspSettingsBox* espSettingsBox,
			BME280_Sensor* bmeMeasurer,DS18D20_Sensor* ds18d20Measurer,NtpTimeClientService* timeClient){

		this->displayRefreshTrigger=new TimeTrigger(0,MODE_TIME_REFRESH_INTERVAL,false,[this](){refreshDisplay();});
		this->timeDisplay=timeDisplay;
		this->espSettingsBox=espSettingsBox;
		this->bmeMeasurer=bmeMeasurer;
		this->ds18d20Measurer=ds18d20Measurer;
		this->timeClient=timeClient;
	}
	virtual ~DisplayHelper_TM1637_Clock_PIR(){};

	virtual boolean loop(){
		if(!timeClient->isTimeReceived()){
			return false;
		}
		return displayRefreshTrigger->loop();
	}

	void displayLoad(){
		timeDisplay->clearDisplay();
		delay(50);
		timeDisplay->point(false);
		int8_t load[4]={1,0,10,13};
		timeDisplay->display(load);
	}

	void displayWiFiParams(boolean isAP,String ip){

		Serial.println(FPSTR("---Display WiFiParams---"));

		timeDisplay->clearDisplay();
		delay(MODE_DISPLAY_AP_SAP_DELAY);
		timeDisplay->point(false);

		if(isAP){
			int8_t mode[4]={SYMBOL_EMPTY,SYMBOL_EMPTY,10,13};
			timeDisplay->display(mode);
		}else{
			int8_t mode[4]={SYMBOL_EMPTY,0,10,10};
			timeDisplay->display(mode);
		}

		delay(MODE_DISPLAY_WIFI_DELAY);

		int len=ip.length();
		int beginIndex=0;

		for(uint8_t i=0;i<len;i++){

			String symbol=ip.substring(i,i+1);

			if(symbol=="." || i==len-1){
				int8_t ipPart[4]={SYMBOL_EMPTY,SYMBOL_EMPTY,SYMBOL_EMPTY,SYMBOL_EMPTY};

				String part=ip.substring(beginIndex,i);

				if(i==len-1){
					part=ip.substring(beginIndex);
				}

				beginIndex=i+1;

				for(uint8_t j=0;j<part.length();j++){
					ipPart[j]=part.substring(j,j+1).toInt();
				}

				timeDisplay->display(ipPart);
				delay(2000);
			}
		}
	}

	void init(){
		 timeDisplay->set(2);
		 timeDisplay->init();
		 displayLoad();
	}

	void initDisplayMode(){
	  if(bmeMeasurer->getStatus()){
		  maxMode+=MODE_BME_PRESSURE;
	  }

	  maxMode+=ds18d20Measurer->getItemCount();

	  displayMode=0;
	  currentDs18=0;

	  Serial.print(FPSTR("maxDisplayMode="));
	  Serial.println(maxMode);

	  displayRefreshTrigger->start();
	}

	void postProcessConnection(boolean isAp,String ip){
		initDisplayMode();
	    displayWiFiParams(isAp,ip);
		displayLoad();
	}

	void displayTime(){
		dotVal=!dotVal;
		timeDisplay->point(dotVal);

		int8_t* time=timeClient->getCurrentTimePrepared();
		timeDisplay->display(time);
	}

	void displayDate(){
		clearDisplay(1);
		dotVal=true;
		timeDisplay->point(dotVal);

		int8_t* date=timeClient->getCurrentMonthDate();
		/*
		for(uint8_t i=0;i<4;i++){
			Serial.print(date[i]);
		}
		Serial.println();
		*/
		timeDisplay->display(date);
	}

	void displayTemperatureBme(boolean fromButton){
		displayDisplayModeInfo(SYMBOL_DEGREE,20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
		displayIntStringValue(bmeMeasurer->getIntStringValByIndex(BME_280_TEMPERATURE_INDEX),SYMBOL_DEGREE);
	}

	void displayHumidityBme(boolean fromButton){
		displayDisplayModeInfo(SYMBOL_HUMIDITY,20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
		displayIntStringValue(bmeMeasurer->getIntStringValByIndex(BME_280_HUMIDITY_INDEX),SYMBOL_HUMIDITY);
	}

	void displayPressureBme(boolean fromButton){
		displayDisplayModeInfo(SYMBOL_PRESSURE,20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
		Serial.print("pressure PA=");
		Serial.print(bmeMeasurer->getPressurePascal());
		Serial.print("; pressure mm=");
		Serial.println(bmeMeasurer->getIntStringValFromFloat(bmeMeasurer->getPressureHgmm()));
		displayIntStringValue(bmeMeasurer->getIntStringValFromFloat(bmeMeasurer->getPressureHgmm()),SYMBOL_PRESSURE);
	}

	void displayTemperatureDS18D20(boolean fromButton){
		displayDisplayModeInfo(SYMBOL_DEGREE,20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
		String dsTempVal=ds18d20Measurer->getIntStringValByIndex(currentDs18);
		displayIntStringValue(dsTempVal,SYMBOL_DEGREE);
	}

	void displayDisplayModeInfo(uint8_t modeDescriptor,uint8_t delay1,uint16_t delay2,boolean fromButton){
		clearDisplay(delay1);

		if(fromButton){
			int8_t statusText[4]={SYMBOL_MINUS,displayMode,SYMBOL_EMPTY,modeDescriptor};
			timeDisplay->display(statusText);

			delay(delay2);
		}
	}

	void displayIntStringValue(String str,int modeDescriptor){
		int len=str.length();
		int8_t valText[4]={SYMBOL_EMPTY,SYMBOL_EMPTY,SYMBOL_EMPTY,SYMBOL_EMPTY};
		uint8_t displLastIndex=3;

		if(len>=4){
			len=4;
		}else{
			valText[3]=modeDescriptor;
		}
		if(len>=1){
			uint8_t startIndex=displLastIndex-len;
			for(uint8_t i=0;i<len;i++){
				valText[startIndex+i]=(str.substring(i, i+1)).toInt();
				//Serial.print(valText[startIndex+i]);
			}
			for(uint8_t i=0;i<startIndex;i++){
				valText[i]=SYMBOL_EMPTY;
			}
			//Serial.println();
		}

		timeDisplay->display(valText);
	}


	void clearDisplay(uint8_t delayTime){
		timeDisplay->clearDisplay();
		delay(delayTime);
		timeDisplay->point(false);
	}

	void refreshDisplayPage(boolean fromButton){
		if(timeClient->isTimeReceived()){
/*
			Serial.print(FPSTR("refreshDisplayPage fromButton="));
			Serial.print(fromButton);
			Serial.print(FPSTR(" displayMode="));
			Serial.print(displayMode);
*/
			switch(displayMode){
				case MODE_TIME: displayTime(); break;
				case MODE_DATE: displayDate(); break;
				case MODE_BME_TEMP: displayTemperatureBme(fromButton); break;
				case MODE_BME_HUMIDITY: displayHumidityBme(fromButton); break;
				case MODE_BME_PRESSURE: displayPressureBme(fromButton); break;

				default:
					if(ds18d20Measurer->getItemCount()>0){
						displayTemperatureDS18D20(fromButton);
					}else{
						changePage();
					}
			}
		}else{
			//Serial.println(FPSTR("timeClient not initialized"));
			displayLoad();
		}
	}

	void refreshDisplay(){
		//Serial.println("Refresh display");
		refreshDisplayPage(false);
	}

	void selectCurrentDS18D20(){
		if(displayMode==1+maxMode-ds18d20Measurer->getItemCount()){
			currentDs18=0;
		}
		else{
			if(displayMode>=1+maxMode-ds18d20Measurer->getItemCount() && displayMode<maxMode){
				currentDs18++;
			}
		}
	}

	void changePage(){
		if(displayMode>=maxMode){
			displayMode=MODE_TIME;
		}else{
			displayMode++;
		}

		selectCurrentDS18D20();

		Serial.print(FPSTR("maxMode="));
		Serial.println(maxMode);
		Serial.print(FPSTR("displayMode="));
		Serial.println(displayMode);
		Serial.print(FPSTR("currentDs18="));
		Serial.println(currentDs18);

		uint refreshInterval=MODE_OTHER_REFRESH_INTERVAL;

		switch(displayMode){
			case MODE_TIME: refreshInterval=MODE_TIME_REFRESH_INTERVAL;break;
			case MODE_DATE: refreshInterval=MODE_DATE_REFRESH_INTERVAL;break;

			default: refreshInterval=MODE_OTHER_REFRESH_INTERVAL;
		}

		displayRefreshTrigger->start(refreshInterval);

		refreshDisplayPage(true);
	}
private:
	TM1637* timeDisplay;
	EspSettingsBox* espSettingsBox;
	BME280_Sensor* bmeMeasurer;
	DS18D20_Sensor* ds18d20Measurer;
	NtpTimeClientService* timeClient;
	TimeTrigger* displayRefreshTrigger;

	/*
	 0 - time
	 1 - date
	 2 - temp1
	 3 - humidity
	 4 - pressure
	 5 - temp DS18
	 */
	uint8_t displayMode=0;
	uint8_t maxMode=0;
	uint8_t currentDs18=0;
	boolean dotVal=false;
};



#endif /* LIBRARIES_HELPERS_DISPLAY_CUSTOM_DISPLAYHELPER_TM1637_CLOCK_PIR_H_ */
