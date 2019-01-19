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
#include "Time/NtpTimeClientService.h"
#include "TM1637.h"
#include "TimeTrigger.h"
#include "interfaces/Loopable.h"

#define MODE_TIME_REFRESH_INTERVAL 500
#define MODE_DATE_REFRESH_INTERVAL 1000
#define MODE_OTHER_REFRESH_INTERVAL 30000
#define MODE_SWITCH_TO_TIME_INTERVAL 10000

#define DISPLAY_AP_SAP_DELAY 1000
#define DISPLAY_WIFI_DELAY 1000
#define DISPLAY_POSTIP_DELAY 1000

#define DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED 1000

#define MODE_TIME 0
#define MODE_DATE 1
#define MODE_BME_TEMP 2
#define MODE_BME_HUMIDITY 3
#define MODE_BME_PRESSURE 4
#define MODE_DS18D20_TEMP 5

class DisplayHelper_TM1637_Clock_PIR: public Loopable {
public:
	DisplayHelper_TM1637_Clock_PIR(TM1637* timeDisplay,EspSettingsBox* espSettingsBox,
				BME280_Sensor* bmeMeasurer,DS18D20_Sensor* ds18d20Measurer){

		this->displayRefreshTrigger=new TimeTrigger(0,MODE_TIME_REFRESH_INTERVAL,false,[this](){refreshDisplay();});
		this->timeSwitchTrigger=new TimeTrigger(0,MODE_SWITCH_TO_TIME_INTERVAL,false,[this](){autoSwitchToTimeDisplay();});
		this->timeDisplay=timeDisplay;
		this->espSettingsBox=espSettingsBox;
		this->bmeMeasurer=bmeMeasurer;
		this->ds18d20Measurer=ds18d20Measurer;
	}

	DisplayHelper_TM1637_Clock_PIR(TM1637* timeDisplay,EspSettingsBox* espSettingsBox,
			BME280_Sensor* bmeMeasurer,DS18D20_Sensor* ds18d20Measurer,NtpTimeClientService* timeClient){

		this->displayRefreshTrigger=new TimeTrigger(0,MODE_TIME_REFRESH_INTERVAL,false,[this](){refreshDisplay();});
		this->timeSwitchTrigger=new TimeTrigger(0,MODE_SWITCH_TO_TIME_INTERVAL,false,[this](){autoSwitchToTimeDisplay();});
		this->timeDisplay=timeDisplay;
		this->espSettingsBox=espSettingsBox;
		this->bmeMeasurer=bmeMeasurer;
		this->ds18d20Measurer=ds18d20Measurer;
		this->timeClient=timeClient;
	}
	virtual ~DisplayHelper_TM1637_Clock_PIR(){};

	void init(){
		 timeDisplay->set(2);
		 timeDisplay->init();
		 displayLoad();
	}

	virtual boolean loop(){
		if(timeClient==nullptr || !timeClient->isTimeReceived()){
			return false;
		}

		timeSwitchTrigger->loop();
		return displayRefreshTrigger->loop();
	}

	void displayTime(){
		clearDisplay(1);
		int8_t load[4]={SYMBOL_t,1,SYMBOL_n,SYMBOL_E};
		display(load);
	}

	void displayLoad(){
		clearDisplay(1);
		int8_t load[4]={SYMBOL_L,SYMBOL_o,SYMBOL_A,SYMBOL_d};
		display(load);
	}

	void displayErr(){
		clearDisplay(1);
		int8_t load[4]={SYMBOL_E,SYMBOL_r,SYMBOL_r,SYMBOL_SPACE};
		display(load);
	}

	void displayConn(){
		clearDisplay(1);
		int8_t load[4]={SYMBOL_C,SYMBOL_o,SYMBOL_n,SYMBOL_n};
		display(load);
	}

	void displayDone(){
		clearDisplay(1);
		int8_t load[4]={SYMBOL_d,SYMBOL_o,SYMBOL_n,SYMBOL_E};
		display(load);
	}

	void displayWiFiParams(boolean isAP,String ip){

		Serial.println(FPSTR("---Display WiFiParams---"));

		timeDisplay->clearDisplay();
		delay(DISPLAY_AP_SAP_DELAY);
		timeDisplay->point(false);

		if(isAP){
			int8_t mode[4]={SYMBOL_SPACE,SYMBOL_SPACE,SYMBOL_A,SYMBOL_P};
			display(mode);
		}else{
			int8_t mode[4]={SYMBOL_SPACE,SYMBOL_S,SYMBOL_t,SYMBOL_A};
			display(mode);
		}

		delay(DISPLAY_WIFI_DELAY);

		int8_t mode[4]={SYMBOL_A,SYMBOL_d,SYMBOL_d,SYMBOL_r};
		display(mode);
		delay(DISPLAY_WIFI_DELAY);

		int len=ip.length();
		int beginIndex=0;

		for(uint8_t i=0;i<len;i++){

			String symbol=ip.substring(i,i+1);

			if(symbol=="." || i==len-1){
				int8_t ipPart[4]={SYMBOL_SPACE,SYMBOL_SPACE,SYMBOL_SPACE,SYMBOL_SPACE};

				String part=ip.substring(beginIndex,i);

				if(i==len-1){
					part=ip.substring(beginIndex);
				}

				beginIndex=i+1;
				uint8_t startIndex=4-part.length();

				for(uint8_t j=0;j<part.length();j++){
					ipPart[j+startIndex]=part.substring(j,j+1).toInt();
				}

				display(ipPart);
				delay(DISPLAY_POSTIP_DELAY);
			}
		}
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
		displayDone();
	}

	void displayTimePage(){
		if(timeClient==nullptr){
			displayErr();
			return;
		}
		dotVal=!dotVal;
		timeDisplay->point(dotVal);

		int8_t* time=timeClient->getCurrentTimePrepared();
		display(time);
	}

	void displayDatePage(boolean fromButton){
		if(timeClient==nullptr){
			displayErr();
			return;
		}
		clearDisplay(0);

		if(fromButton){
			int8_t statusText[4]={SYMBOL_d,SYMBOL_A,SYMBOL_t,SYMBOL_E};
			display(statusText);

			delay(DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED);
		}
		dotVal=true;
		timeDisplay->point(dotVal);

		int8_t* date=timeClient->getCurrentMonthDate();
		display(date);

		if(fromButton){
			timeSwitchTrigger->start();
		}
	}

	void displayTemperatureBmePage(boolean fromButton){
		clearDisplay(0);

		if(fromButton){
			int8_t statusText[4]={SYMBOL_t,SYMBOL_E,SYMBOL_SPACE,SYMBOL_DEGREE};
			display(statusText);

			delay(DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED);
		}
		displayDisplayModeInfo(20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
		displayIntStringValue(bmeMeasurer->getIntStringValByIndex(BME_280_TEMPERATURE_INDEX),SYMBOL_DEGREE);

		if(fromButton){
			timeSwitchTrigger->start();
		}
	}

	void displayHumidityBmePage(boolean fromButton){
		clearDisplay(0);

		if(fromButton){
			int8_t statusText[4]={SYMBOL_h,SYMBOL_u,SYMBOL_SPACE,SYMBOL_PERCENT};
			display(statusText);

			delay(DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED);
		}

		displayDisplayModeInfo(20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
		displayIntStringValue(bmeMeasurer->getIntStringValByIndex(BME_280_HUMIDITY_INDEX),SYMBOL_h);

		if(fromButton){
			timeSwitchTrigger->start();
		}
	}

	void displayPressureBmePage(boolean fromButton){
		clearDisplay(0);

		if(fromButton){
			int8_t statusText[4]={SYMBOL_P,SYMBOL_r,SYMBOL_E,SYMBOL_S};
			display(statusText);

			delay(DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED);
		}
		Serial.print("pressure PA=");
		Serial.print(bmeMeasurer->getPressurePascal());
		Serial.print("; pressure mm=");
		Serial.println(bmeMeasurer->getIntStringValFromFloat(bmeMeasurer->getPressureHgmm()));
		displayIntStringValue(bmeMeasurer->getIntStringValFromFloat(bmeMeasurer->getPressureHgmm()),SYMBOL_P);

		if(fromButton){
			timeSwitchTrigger->start();
		}
	}

	void displayTemperatureDS18D20Page(boolean fromButton){
		displayDisplayModeInfo(20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
		String dsTempVal=ds18d20Measurer->getIntStringValByIndex(currentDs18);
		displayIntStringValue(dsTempVal,SYMBOL_DEGREE);

		if(fromButton){
			timeSwitchTrigger->start();
		}
	}

	void displayDisplayModeInfo(uint8_t delay1,uint16_t delay2,boolean fromButton){
		clearDisplay(delay1);

		if(fromButton){
			int8_t statusText[4]={SYMBOL_n,SYMBOL_r,SYMBOL_SPACE,displayMode};
			display(statusText);

			delay(delay2);
		}
	}

	void displayIntStringValue(String str,int modeDescriptor){
		int len=str.length();
		int8_t valText[4]={SYMBOL_SPACE,SYMBOL_SPACE,SYMBOL_SPACE,SYMBOL_SPACE};
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
				valText[i]=SYMBOL_SPACE;
			}
			//Serial.println();
		}

		display(valText);
	}

	void display(int8_t DispData[]){
		timeDisplay->display(DispData);
	}

	void clearDisplay(uint8_t delayTime){
		timeDisplay->clearDisplay();
		delay(delayTime);
		timeDisplay->point(false);
	}

	void refreshDisplayPage(boolean fromButton){
		if(timeClient==nullptr){
			displayErr();
			return;
		}
		if(timeClient->isTimeReceived()){
/*
			Serial.print(FPSTR("refreshDisplayPage fromButton="));
			Serial.print(fromButton);
			Serial.print(FPSTR(" displayMode="));
			Serial.print(displayMode);
*/
			switch(displayMode){
				case MODE_TIME: displayTimePage(); break;
				case MODE_DATE: displayDatePage(fromButton); break;
				case MODE_BME_TEMP: displayTemperatureBmePage(fromButton); break;
				case MODE_BME_HUMIDITY: displayHumidityBmePage(fromButton); break;
				case MODE_BME_PRESSURE: displayPressureBmePage(fromButton); break;

				default:
					if(ds18d20Measurer->getItemCount()>0){
						displayTemperatureDS18D20Page(fromButton);
					}else{
						changePage();
					}
			}
		}else{
			//Serial.println(FPSTR("timeClient not initialized"));
			displayLoad();
		}
	}

	void autoSwitchToTimeDisplay(){
		displayMode=100;
		changePage();
		timeSwitchTrigger->stop();
		displayTime();
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

	void changePageIfTrigger(boolean trigger){
		if(trigger){
			Serial.println(FPSTR("Page change triggered"));
			changePage();
		}
	}

	void changePage(){
		if(displayMode>=maxMode){
			displayMode=MODE_TIME;
		}else{
			displayMode++;
		}

		selectCurrentDS18D20();
/*
		Serial.print(FPSTR("maxMode="));
		Serial.println(maxMode);
		Serial.print(FPSTR("displayMode="));
		Serial.println(displayMode);
		Serial.print(FPSTR("currentDs18="));
		Serial.println(currentDs18);
*/
		uint refreshInterval=MODE_OTHER_REFRESH_INTERVAL;

		switch(displayMode){
			case MODE_TIME: refreshInterval=MODE_TIME_REFRESH_INTERVAL;break;
			case MODE_DATE: refreshInterval=MODE_DATE_REFRESH_INTERVAL;break;

			default: refreshInterval=MODE_OTHER_REFRESH_INTERVAL;
		}

		Serial.print(FPSTR("refreshInterval="));
		Serial.print(refreshInterval);
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
	TimeTrigger* timeSwitchTrigger;

	/*
	 0 - time
	 1 - date
	 2 - temp1
	 3 - humidity
	 4 - pressure
	 5 - temp DS18
	 */
	int8_t displayMode=0;
	uint8_t maxMode=0;
	uint8_t currentDs18=0;
	boolean dotVal=false;
};



#endif /* LIBRARIES_HELPERS_DISPLAY_CUSTOM_DISPLAYHELPER_TM1637_CLOCK_PIR_H_ */
