#include "Arduino.h"
#include <Hash.h>

#include "EspSettingsBox.h"
#include <extraBoxes/ESPSett_Alarm.h>
#include <extraBoxes/ESPSett_Display.h>
#include <extraBoxes/ESPSett_Ntp.h>
#include <extraBoxes/ESPSett_Telegram.h>

#include "FS.h"
#include <Wire.h>

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WiFi.h>

#include "interfaces/JSONprovider.h"
#include "interfaces/JSONprocessor.h"
#include "interfaces/Loopable.h"
#include "interfaces/SendAble.h"
#include "interfaces/DeviceLibable.h"
#include "I2Chelper.h"
#include "WiFiHelper.h"

#include "Time/TimeIntervalService.h"
#include "Time/NtpTimeClientService.h"
#include "TimeTrigger.h"

#include <DeviceHelper.h>
#include <Display_Pageable/DisplayHelperAbstract.h>
#include <ThingSpeakHelper.h>
#include <PinDigital.h>
#include <Pir_Sensor.h>
#include <BeeperB.h>
#include "BeeperSerial.h"

#include "DS18D20_Sensor.h"
#include "BME280_Sensor.h"
#include "StatusMessage/StatusMessage.h"
#include "Display_Custom/DisplayHelper_TM1637_Clock_PIR.h"

#include "TM1637.h"
ESPSett_Ntp espSett_Ntp;

ESPExtraSettingsBox* extraBoxes[]={&espSett_Ntp};
EspSettingsBox espSettingsBox(extraBoxes);

BeeperB beeper(D5,HIGH,LOW,true,false);

uint16_t sounds[]={250,250,250,500};
uint16_t delays[]={250,250,250,1000};
BeeperSerial beeperSerial(&beeper,sounds,delays,ARRAY_SIZE(sounds));

PinDigital buttonMenu(FPSTR(SENSOR_buttonLeft),D8,onButtonMenuChanged);
PinDigital signalLed(FPSTR(SENSOR_signalLed),D0,LOW,LOW);

Pir_Sensor pirDetector(VAR_NAME(pirDetector),D4,onPirDetectorChanged,700,30000);

DS18D20_Sensor ds18d20Measurer(FPSTR(SENSOR_ds18d20Measurer), D3);
BME280_Sensor bmeMeasurer(FPSTR(SENSOR_bmeMeasurer));

//ESP8266WebServer server ( 80 );
//ESP8266HTTPUpdateServer httpUpdater(true);

TM1637 timeDisplay(D6,D7);

TimeTrigger sensorsTrigger(0,(espSettingsBox.refreshInterval*1000),true,updateSensors);
TimeTrigger thingSpeakTrigger(0,(espSettingsBox.postDataToTSInterval*1000),false,processThingSpeakPost);

NtpTimeClientService timeService(&espSettingsBox);
DisplayHelper_TM1637_Clock_PIR displayHelper(&timeDisplay,&espSettingsBox,&bmeMeasurer,&ds18d20Measurer,&timeService);//,);

WiFiHelper wifi(&espSettingsBox, postInitWebServer);
ThingSpeakHelper thingSpeakHelper(&espSettingsBox,&wifi);

TimeIntervalService timeIntervalService(&espSettingsBox,&timeService,onTimeIntervalEvent,nullptr,0);

Loopable* loopArray[]={&wifi,&buttonMenu,&thingSpeakTrigger,&timeService,&displayHelper,
						&pirDetector,&timeIntervalService,&beeperSerial,&sensorsTrigger};

AbstractItem* sensors[]={&bmeMeasurer,&ds18d20Measurer,&pirDetector,&signalLed};
JSONprovider* jsonProviders[]={&bmeMeasurer,&ds18d20Measurer,&pirDetector,&signalLed,&timeService,&timeIntervalService,&espSettingsBox};
JSONprocessor* jsonProcessors[]={&timeIntervalService,&thingSpeakHelper,&espSettingsBox};
SendAble* senders[]={&thingSpeakHelper};

//DeviceLibable* allItems[]={&bmeMeasurer,&ds18d20Measurer,&pirDetector,&signalLed,&timeIntervalService,&espSettingsBox,&thingSpeakHelper};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray),
						  jsonProcessors,ARRAY_SIZE(jsonProcessors),
						  jsonProviders,ARRAY_SIZE(jsonProviders),
						  sensors,ARRAY_SIZE(sensors),
						  senders,ARRAY_SIZE(senders),
						  &espSettingsBox,
						  120000);

void initComponents(){

	deviceHelper.printHeap();
	beeper.init();

	deviceHelper.printHeap();
	displayHelper.displayConn();

	deviceHelper.printHeap();
	wifi.init();
	deviceHelper.printHeap();
	timeService.init();
	deviceHelper.printHeap();
	bmeMeasurer.init();
	deviceHelper.printHeap();
	ds18d20Measurer.init();
	deviceHelper.printHeap();

	espSettingsBox.loadSensorsFromFile(sensors);
	deviceHelper.printHeap();

	updateSensors();
	deviceHelper.printHeap();

	thingSpeakHelper.setItems(sensors, ARRAY_SIZE(sensors));
	deviceHelper.printHeap();
	sensorsTrigger.init();
	deviceHelper.printHeap();
	timeIntervalService.init();
	deviceHelper.printHeap();
}

void setup() {
  Serial.begin(115200);

  I2Chelper::initStatic(D1,D2);
  displayHelper.init();

  espSettingsBox.initSpiff();
  espSettingsBox.deleteFilesByPreffix("/");

  deviceHelper.startDevice(espSettingsBox.DeviceId,buttonMenu.getPin());

  initComponents();

  displayHelper.postProcessConnection(wifi.isAP(),wifi.getIpStr());

  playPostInitSounds();

  onPirDetectorChanged();

  deviceHelper.printDeviceDiagnostic();
  Serial.println(FPSTR(MESSAGE_DEVICE_STARTED));
}

void loop() {
	deviceHelper.loop();

	//beeper.shortBeep(wifi.getReconnected());

	processTimeIntervals();
}

//---------------------------------------------------------------------
void playPostInitSounds(){
	uint8_t count=3*3*ds18d20Measurer.hasNoItems();
	Serial.print(FPSTR("Post init sounds="));
	Serial.println(count);
	beeper.shortBeep(count);
}
//-------------Web server functions-------------------------------------
void postInitWebServer(){
	wifi.server()->on(FPSTR(URL_GET_JSON), HTTP_GET, [](){getProvidersJson();});
	wifi.server()->on(FPSTR(URL_PROCESS_JSON), HTTP_POST, [](){processJson();});
}

void getProvidersJson(){
	if(!wifi.checkAuthentication()){
		return;
	}

	if(wifi.isValidGetJsonRequest()){

		deviceHelper.printDeviceDiagnostic();

		wifi.server()->send(200, FPSTR(CONTENT_TYPE_JSON_UTF8),
							deviceHelper.getProvidersJson(
									wifi.getNameParam(),
									wifi.getPageParam())
								);
	}else{
		Serial.println(FPSTR("!!!---JSON provider not found!---!!!"));
		//wifi.printRequestDetails();
		String msg=FPSTR(MESSAGE_PROV_NOT_FOUND);
		msg+=wifi.getNameParam();
		wifi.server()->send(404, FPSTR(CONTENT_TYPE_JSON_UTF8), StatusMessage(STATUS_PARAM_NOT_FOUND_INT,msg).getJson());
	}
}

void processJson(){
	if(wifi.checkAuthentication()){

		if(wifi.isValidProcessJsonRequest()){

			deviceHelper.printDeviceDiagnostic();

			wifi.server()->arg(FPSTR(MESSAGE_SERVER_ARG_REMOTE_TARGET));

			StatusMessage sm=deviceHelper.processIncomeJson(
					wifi.getRemoteTargetParam(),
					wifi.getRemotePageParam(),
					wifi.getValJsonParam()
				  );

			wifi.server()->send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), sm.getJson());
		}else{
			Serial.println(FPSTR("!!!---JSON processor not found! Parameters missing---!!!"));
			//wifi.printRequestDetails();
			String msg=FPSTR(MESSAGE_PROC_NOT_FOUND);
			msg+=wifi.getNameParam();

			wifi.server()->send(404, FPSTR(CONTENT_TYPE_JSON_UTF8), StatusMessage(STATUS_PARAM_NOT_FOUND_INT,msg).getJson());
		}
	}
}

//---------------------------------------------------------------------
//button handling
void onButtonMenuChanged(){
	if(timeIntervalService.hasActiveAlarms(INTERVAL_KIND_ALARM)){
		timeIntervalService.stopAndRescheduleAll(INTERVAL_KIND_ALARM);
		return;
	}
	displayHelper.changePageIfTrigger(!buttonMenu.isOn());
}

void onPirDetectorChanged(){
	signalLed.turnOnOff(pirDetector.isOn());
}

void printPir(){
	pirDetector.displayDetails();
}
//-------------TimeIntervalService------------------------
void onTimeIntervalEvent(TimeIntervalDetail timeInterval){
	Serial.print(FPSTR("BEEPER ="));
	Serial.println(timeIntervalService.getStateName(timeInterval.state));

	if(timeInterval.kind==INTERVAL_KIND_ALARM){
		if(timeInterval.state==ACTIVE){
			beeperSerial.start();
		}else{
			beeperSerial.stop();
		}
	}
}

void processTimeIntervals(){
	if(!timeIntervalService.hasActiveAlarms(INTERVAL_KIND_ALARM)){
		beeperSerial.stop();
	}
}

//------------------Sensors func---------------------------------------------------

void updateSensors(){
	deviceHelper.update();
}

//-------------------------Thing speak functions---------------------
void executeThingSpeakPost(){
	thingSpeakHelper.send();
}
//test functions--------------------------------------
void processThingSpeakPost(){
	Serial.println("process ThingSpeak post");
}

int8_t symbolKey=0;

void changeSymbol(){
	if(!buttonMenu.isOn()){
		Serial.println(FPSTR("Menu button UP"));

		int8_t s1=symbolKey / 100;
		int8_t s2=(symbolKey % 100) / 10;
		int8_t s3=(symbolKey % 100) % 10;

		if(s1<0){s1*=-1;}
		if(s2<0){s2*=-1;}
		if(s3<0){s3*=-1;}

		Serial.print(s1);
		Serial.print(s2);
		Serial.print(s3);

		Serial.print("='");
		Serial.print(symbolKey);
		Serial.println("'=");

		int8_t statusText[4]={s1,s2,s3,symbolKey};
		timeDisplay.display(statusText);

		symbolKey+=1;
		if(symbolKey<0){
			symbolKey=0;
		}
	}
}
/*
//------------------------------MQTT functions-----------------------------------------------------

void sendAbstractItemToHttp(AbstractItem* item){
	if(espSettingsBox.isHttpPostEnabled){
		wifi.executeFormPostRequest(espSettingsBox.httpPostIp.toString(), item->getJson());
	}
}

WiFiEventHandler stationModeConnectedHandler;
WiFiEventHandler onStationModeDHCPTimeoutHandler;
WiFiEventHandler onStationModeDisconnectedHandler;
WiFiEventHandler onStationModeGotIPHandler;

void setupWifiEvents(){
	Serial.println(FPSTR("Init WiFi events"));

	if(wifi.isAP()){
		WiFi.onSoftAPModeStationConnected([](const WiFiEventSoftAPModeStationConnected& evt){wifi.onSoftAPModeStationConnected(evt);});
		WiFi.onSoftAPModeStationDisconnected([](const WiFiEventSoftAPModeStationDisconnected& evt){wifi.onSoftAPModeStationDisconnected(evt);});
	}else{
		Serial.println(FPSTR("--------setupWifiEvents-------"));
		stationModeConnectedHandler=WiFi.onStationModeConnected(onStationModeConnected);
		onStationModeDisconnectedHandler=WiFi.onStationModeDisconnected(onStationModeDisconnected);
		onStationModeDHCPTimeoutHandler=WiFi.onStationModeDHCPTimeout(onStationModeDHCPTimeout);
		onStationModeGotIPHandler=WiFi.onStationModeGotIP(onStationModeGotIP);
	}
}
//--------------------------------------------------------------------------------------
 boolean reconnected=false;
void onStationModeConnected(const WiFiEventStationModeConnected& evt){
		Serial.print(FPSTR("STA connected: "));
		Serial.print(FPSTR(" ssid="));
		Serial.print(evt.ssid);
		Serial.print(FPSTR(" bssid="));
		Serial.print(wifi.macToString(evt.bssid));
		Serial.print(FPSTR(" channel="));
		Serial.println(evt.channel);
	}

	void onStationModeDisconnected(const WiFiEventStationModeDisconnected& evt){
		Serial.print(FPSTR("STA DISconnected: "));
		Serial.print(FPSTR(" ssid="));
		Serial.print(evt.ssid);
		Serial.print(FPSTR(" bssid="));
		Serial.print(wifi.macToString(evt.bssid));
		Serial.print(FPSTR(" reason="));
		Serial.println(evt.reason);
	}

	void onStationModeDHCPTimeout(){
		Serial.print(FPSTR("STA DHCP timed out: "));
	}

	void onStationModeGotIP(const WiFiEventStationModeGotIP& evt){
		Serial.print(FPSTR("onStationModeGotIP IP="));
		Serial.println(evt.ip);
		reconnected=true;
	}
*/
