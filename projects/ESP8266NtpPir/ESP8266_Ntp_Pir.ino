#include "Arduino.h"
#include <Hash.h>

#include "EspSettingsBox.h"
#include <ESPSett_Alarm.h>
#include <ESPSett_Display.h>
#include <ESPSett_Ntp.h>
#include <ESPSett_Telegram.h>

#include "FS.h"
#include <Wire.h>

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WiFi.h>

#include "interfaces/JSONprovider.h"
#include "interfaces/JSONprocessor.h"
#include "interfaces/Loopable.h"
#include "interfaces/SendAble.h"

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

#include "DS18D20_Sensor.h"
#include "BME280_Sensor.h"
#include "StatusMessage/StatusMessage.h"
#include "Display_Custom/DisplayHelper_TM1637_Clock_PIR.h"

#include "TM1637.h"
ESPSett_Ntp espSett_Ntp;

ESPExtraSettingsBox* extraBoxes[]={&espSett_Ntp};
EspSettingsBox espSettingsBox(extraBoxes,ARRAY_SIZE(extraBoxes));

BeeperB beeper(D5,HIGH,LOW,true,false);

I2Chelper i2cHelper(D1,D2,false);

PinDigital buttonMenu(FPSTR(SENSOR_buttonLeft),D8,onButtonMenuChanged);
PinDigital signalLed(FPSTR(SENSOR_signalLed),D0,LOW,LOW);

Pir_Sensor pirDetector(VAR_NAME(pirDetector),D4,onPirDetectorChanged,700,30000);

DS18D20_Sensor ds18d20Measurer(FPSTR(SENSOR_ds18d20Measurer), D3);
BME280_Sensor bmeMeasurer(FPSTR(SENSOR_bmeMeasurer));

ESP8266WebServer server ( 80 );
ESP8266HTTPUpdateServer httpUpdater(true);

TM1637 timeDisplay(D6,D7);

TimeTrigger sensorsTrigger(0,(espSettingsBox.refreshInterval*1000),true,updateSensors);
TimeTrigger thingSpeakTrigger(0,(espSettingsBox.postDataToTSInterval*1000),false,processThingSpeakPost);

NtpTimeClientService timeService(&espSettingsBox);
DisplayHelper_TM1637_Clock_PIR displayHelper(&timeDisplay,&espSettingsBox,&bmeMeasurer,&ds18d20Measurer,&timeService);//,);

WiFiHelper wifiHelper(&espSettingsBox, nullptr, &server,nullptr,postInitWebServer,true);
ThingSpeakHelper thingSpeakHelper(&espSettingsBox,&wifiHelper);

TimeIntervalService timeIntervalService(&espSettingsBox,&timeService,onTimeIntervalEvent,nullptr,0);

Loopable* loopArray[]={&wifiHelper,&buttonMenu,&thingSpeakTrigger,&timeService,&displayHelper,
						&pirDetector,&timeIntervalService};

AbstractItem* sensors[]={&bmeMeasurer,&ds18d20Measurer,&pirDetector,&signalLed};
JSONprovider* jsonProviders[]={&bmeMeasurer,&ds18d20Measurer,&pirDetector,&signalLed,&timeService,&timeIntervalService,&espSettingsBox};
JSONprocessor* jsonProcessors[]={&timeIntervalService,&thingSpeakHelper,&espSettingsBox};
SendAble* senders[]={&thingSpeakHelper};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray),
						  jsonProcessors,ARRAY_SIZE(jsonProcessors),
						  jsonProviders,ARRAY_SIZE(jsonProviders),
						  sensors,ARRAY_SIZE(sensors),
						  senders,ARRAY_SIZE(senders),
						  &espSettingsBox,
						  120000);

void initComponents(){
	thingSpeakHelper.setItems(sensors, ARRAY_SIZE(sensors));
	displayHelper.init();
	beeper.init();
	i2cHelper.init();
	httpUpdater.setup(&server,espSettingsBox.settingsUser.c_str(),espSettingsBox.settingsPass.c_str());
	displayHelper.displayConn();
	wifiHelper.init();

	timeService.init();
	bmeMeasurer.init();
	ds18d20Measurer.init();

	loadSensors();

	sensorsTrigger.init();
	timeIntervalService.init();
}

void setup() {
  Serial.begin(115200);

  deviceHelper.printDeviceDiagnosticNoSpiff();

  espSettingsBox.init();

  deviceHelper.printDeviceDiagnostic();
  espSettingsBox.printSpiffsInfo();

  displayHelper.init();

  deviceHelper.startDevice(espSettingsBox.DeviceId);

  initComponents();
  updateSensors();

  displayHelper.postProcessConnection(wifiHelper.isAP(),wifiHelper.getIpStr());

  playPostInitSounds();

  onPirDetectorChanged();

  deviceHelper.printDeviceDiagnostic();
  Serial.println(FPSTR(MESSAGE_DEVICE_STARTED));
}

void loop() {
	deviceHelper.loop();

	beeper.shortBeep(wifiHelper.getReconnected());
	beeper.playGenerator(timeIntervalService.getBeeperActive());

	processTimeIntervals();
}

//---------------------------------------------------------------------
void playPostInitSounds(){
	if(i2cHelper.getDevCount()==0){
		  beeper.longBeep();
	  }else if(ds18d20Measurer.getItemCount()==0){
		  beeper.shortBeep(3);
	  }else{
		  beeper.shortBeep();
	  }
}
//-------------Web server functions-------------------------------------
void postInitWebServer(){
	server.on(FPSTR(URL_GET_JSON), HTTP_GET, [](){getProvidersJson();});
	server.on(FPSTR(URL_PROCESS_JSON), HTTP_POST, [](){processJson();});
}

void getProvidersJson(){
	if(!wifiHelper.checkAuthentication()){
		return;
	}

	if(wifiHelper.isValidGetJsonRequest()){

		deviceHelper.printDeviceDiagnostic();

		server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8),
							deviceHelper.getProvidersJson(
									wifiHelper.getNameParam(),
									wifiHelper.getPageParam())
								);
	}else{
		Serial.println(FPSTR("!!!---JSON provider not found!---!!!"));
		//wifiHelper.printRequestDetails();
		String msg=FPSTR(MESSAGE_PROV_NOT_FOUND);
							msg+=wifiHelper.getNameParam();
		server.send(404, FPSTR(CONTENT_TYPE_JSON_UTF8), StatusMessage(STATUS_PARAM_NOT_FOUND_INT,msg).getJson());
	}
}

void processJson(){
	if(wifiHelper.checkAuthentication()){

		if(wifiHelper.isValidProcessJsonRequest()){

			deviceHelper.printDeviceDiagnostic();

			StatusMessage sm=deviceHelper.processJson(
					server.arg(FPSTR(MESSAGE_SERVER_ARG_REMOTE_TARGET)),
					server.arg(FPSTR(MESSAGE_SERVER_ARG_REMOTE_PAGE)),
					server.arg(FPSTR(MESSAGE_SERVER_ARG_VAL_JSON))
				  );

			return server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), sm.getJson());
		}else{
			Serial.println(FPSTR("!!!---JSON processor not found! Parameters missing---!!!"));
			//wifiHelper.printRequestDetails();
			String msg=FPSTR(MESSAGE_PROC_NOT_FOUND);
					msg+=wifiHelper.getNameParam();
			server.send(404, FPSTR(CONTENT_TYPE_JSON_UTF8), StatusMessage(STATUS_PARAM_NOT_FOUND_INT,msg).getJson());
		}
	}
}

//---------------------------------------------------------------------
//button handling
void onButtonMenuChanged(){
	displayHelper.changePageIfTrigger(!buttonMenu.isOn());
	timeIntervalService.stopAlarmsIfActive();
}

void onPirDetectorChanged(){
	signalLed.turnOnOff(pirDetector.isOn());
}

void printPir(){
	pirDetector.displayDetails();
}
//-------------TimeIntervalService------------------------
void onTimeIntervalEvent(TimeIntervalDetail timeInterval){
	//timeIntervalService.printTimeInterval(timeInterval);
	Serial.print(FPSTR("BEEPER ="));
	Serial.println(timeIntervalService.getBeeperActive());
}

void processTimeIntervals(){

}

//------------------Sensors func---------------------------------------------------
void loadSensors(){
	espSettingsBox.loadAbstractItemsFromFile(sensors, ARRAY_SIZE(sensors));
}

void updateSensors(){
	deviceHelper.update(sensors, ARRAY_SIZE(sensors));

	for(uint8_t i=0;i<ARRAY_SIZE(sensors);i++){
		Serial.println();
		Serial.println(sensors[i]->getJson());
	}

	deviceHelper.printDeviceDiagnostic();
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
		wifiHelper.executeFormPostRequest(espSettingsBox.httpPostIp.toString(), item->getJson());
	}
}

WiFiEventHandler stationModeConnectedHandler;
WiFiEventHandler onStationModeDHCPTimeoutHandler;
WiFiEventHandler onStationModeDisconnectedHandler;
WiFiEventHandler onStationModeGotIPHandler;

void setupWifiEvents(){
	Serial.println(FPSTR("Init WiFi events"));

	if(wifiHelper.isAP()){
		WiFi.onSoftAPModeStationConnected([](const WiFiEventSoftAPModeStationConnected& evt){wifiHelper.onSoftAPModeStationConnected(evt);});
		WiFi.onSoftAPModeStationDisconnected([](const WiFiEventSoftAPModeStationDisconnected& evt){wifiHelper.onSoftAPModeStationDisconnected(evt);});
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
		Serial.print(wifiHelper.macToString(evt.bssid));
		Serial.print(FPSTR(" channel="));
		Serial.println(evt.channel);
	}

	void onStationModeDisconnected(const WiFiEventStationModeDisconnected& evt){
		Serial.print(FPSTR("STA DISconnected: "));
		Serial.print(FPSTR(" ssid="));
		Serial.print(evt.ssid);
		Serial.print(FPSTR(" bssid="));
		Serial.print(wifiHelper.macToString(evt.bssid));
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
