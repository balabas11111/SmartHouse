#include "Arduino.h"
#include <Hash.h>
#include <ESP8266WiFi.h>
#include "EspSettingsBox.h"
#include "Loopable.h"
#include "FS.h"
#include "I2Chelper.h"
#include "WiFiHelper.h"
#include <Wire.h>

#include "NtpTimeClientService.h"
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "TimeTrigger.h"

#include <DeviceHelper.h>
#include <DisplayHelperAbstract.h>
#include <ThingSpeakHelper.h>
#include <PinDigital.h>
#include <Pir_Sensor.h>
#include <BeeperB.h>

#include "DS18D20_Sensor.h"
#include "BME280_Sensor.h"
#include "StatusMessage.h"

#include "TM1637.h"

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

/* NTP Clock Device

 Connection

  I2C                D2, D1

  TM1637 - Display   D6 D7
  DS18D20            D3
  PirSensor          A0
  Button             D5
  Beeper             D8
 */
#define MODE_TIME_REFRESH_INTERVAL 500
#define MODE_OTHER_REFRESH_INTERVAL 30000
#define DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED 1000
#define SYMBOL_EMPTY 0x7f
#define SYMBOL_DEGREE 60
#define SYMBOL_HUMIDITY 47
#define SYMBOL_PRESSURE 46
#define SYMBOL_MINUS 99

#define MODE_TIME 0
#define MODE_BME_TEMP 1
#define MODE_BME_HUMIDITY 2
#define MODE_BME_PRESSURE 3
#define MODE_DS18D20_TEMP 4

#define HUMAN_PRESENTED LOW
#define HUMAN_NOT_PRESENTED HIGH

EspSettingsBox espSettingsBox(FPSTR("NTP clock with PIR sensor"));

BeeperB beeper(D5,HIGH,LOW,true,false);

I2Chelper i2cHelper(D1,D2,false);

PinDigital buttonMenu(FPSTR(SENSOR_buttonLeft),D8,onButtonMenuChanged);
PinDigital AC_Rele1(FPSTR(SENSOR_AC_Rele1),D0,LOW,HIGH);
Pir_Sensor pirDetector(VAR_NAME(pirDetector),A0,onPirDetectorChanged);

DS18D20_Sensor ds18d20Measurer(FPSTR(SENSOR_ds18d20Measurer), D3);
BME280_Sensor bmeMeasurer(FPSTR(SENSOR_bmeMeasurer));

ESP8266WebServer server ( 80 );
ESP8266HTTPUpdateServer httpUpdater(true);

TM1637 timeDisplay(D6,D7);
DisplayHelperAbstract displayHelper(&espSettingsBox);

TimeTrigger sensorsTrigger(0,(espSettingsBox.refreshInterval*1000),true,measureSensors);
TimeTrigger thingSpeakTrigger(0,(espSettingsBox.postDataToTSInterval*1000),espSettingsBox.isThingSpeakEnabled,processThingSpeakPost);
TimeTrigger displayRefreshTrigger(0,MODE_TIME_REFRESH_INTERVAL,false,refreshDisplay);

NtpTimeClientService timeClient(&espSettingsBox,nullptr,60000);

WiFiHelper wifiHelper(&espSettingsBox, &displayHelper, &server,postInitWebServer,false);
ThingSpeakHelper thingSpeakHelper(&espSettingsBox,&wifiHelper);

Loopable* loopArray[]={&wifiHelper,&buttonMenu,&thingSpeakTrigger,&timeClient,&displayRefreshTrigger,&pirDetector};

AbstractItem* abstractItems[]={&bmeMeasurer,&ds18d20Measurer,&pirDetector,&AC_Rele1};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray),120000);

/*
 0 - time
 1 - temp1
 2 - humidity
 3 - pressure
 4 - temp DS18
 */
uint8_t displayMode=0;
uint8_t maxMode=0;
uint8_t currentDs18=0;

void setup() {
  Serial.begin(115200);

  timeDisplay.set(2);
  timeDisplay.init();
  displayLoad();
   //load
  deviceHelper.startDevice(espSettingsBox.DeviceId);

  espSettingsBox.init();

  beeper.init();
  beeper.shortBeep();

  i2cHelper.init();

  httpUpdater.setup(&server);
  wifiHelper.init();

  timeClient.init();
  bmeMeasurer.init();
  ds18d20Measurer.init();

  loadSensors();
  measureSensors();

  initDisplayMode();
  deviceHelper.printDeviceDiagnostic();

  if(ds18d20Measurer.getItemCount()==0 || i2cHelper.getDevCount()==0){
	  beeper.longBeep();
  }else{
	  beeper.shortBeep();
	  beeper.shortBeep();
  }
  Serial.println(FPSTR(MESSAGE_DEVICE_STARTED));
}

void loop() {
	deviceHelper.loop();
}

boolean dotVal=false;

void displayLoad(){
	timeDisplay.clearDisplay();
	delay(50);
	timeDisplay.point(false);
	int8_t load[4]={1,0,10,13};
	timeDisplay.display(load);
}

void initDisplayMode(){
  if(bmeMeasurer.getStatus()){
	  maxMode+=MODE_BME_PRESSURE;
  }

  maxMode+=ds18d20Measurer.getItemCount();

  displayMode=0;
  currentDs18=0;

  Serial.print(FPSTR("maxDisplayMode="));
  Serial.println(maxMode);

  displayRefreshTrigger.start();
}

void displayTime(){
	dotVal=!dotVal;
	timeDisplay.point(dotVal);

	int8_t* time=timeClient.getCurrentTime();
	timeDisplay.display(time);
}

void displayTemperatureBme(boolean fromButton){
	displayDisplayModeInfo(SYMBOL_DEGREE,20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
	displayIntStringValue(bmeMeasurer.getIntStringValByIndex(BME_280_TEMPERATURE_INDEX),SYMBOL_DEGREE);
}

void displayHumidityBme(boolean fromButton){
	displayDisplayModeInfo(SYMBOL_HUMIDITY,20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
	displayIntStringValue(bmeMeasurer.getIntStringValByIndex(BME_280_HUMIDITY_INDEX),SYMBOL_HUMIDITY);
}

void displayPressureBme(boolean fromButton){
	displayDisplayModeInfo(SYMBOL_PRESSURE,20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
	Serial.print("pressure PA=");
	Serial.print(bmeMeasurer.getPressurePascal());
	Serial.print("; pressure mm=");
	Serial.println(bmeMeasurer.getIntStringValFromFloat(bmeMeasurer.getPressureHgmm()));
	displayIntStringValue(bmeMeasurer.getIntStringValFromFloat(bmeMeasurer.getPressureHgmm()),SYMBOL_PRESSURE);
}

void displayTemperatureDS18D20(boolean fromButton){
	displayDisplayModeInfo(SYMBOL_DEGREE,20,DELAY_AFTER_NOT_TIME_PAGE_DISPLAYED,fromButton);
	String dsTempVal=ds18d20Measurer.getIntStringValByIndex(currentDs18);
	displayIntStringValue(dsTempVal,SYMBOL_DEGREE);
}

void displayDisplayModeInfo(int modeDescriptor,uint8_t delay1,uint16_t delay2,boolean fromButton){
	clearDisplay(delay1);

	if(fromButton){
		int8_t statusText[4]={SYMBOL_MINUS,displayMode,SYMBOL_EMPTY,modeDescriptor};
		timeDisplay.display(statusText);

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
			Serial.print(valText[startIndex+i]);
		}
		for(uint8_t i=0;i<startIndex;i++){
			valText[i]=SYMBOL_EMPTY;
		}
		Serial.println();
	}

	timeDisplay.display(valText);
}


void clearDisplay(uint8_t delayTime){
	timeDisplay.clearDisplay();
	delay(delayTime);
	timeDisplay.point(false);
}

void refreshDisplayPage(boolean fromButton){
	if(timeClient.isTimeReceived()){

		switch(displayMode){
			case MODE_TIME: displayTime(); break;
			case MODE_BME_TEMP: displayTemperatureBme(fromButton); break;
			case MODE_BME_HUMIDITY: displayHumidityBme(fromButton); break;
			case MODE_BME_PRESSURE: displayPressureBme(fromButton); break;

			default:
				if(ds18d20Measurer.getItemCount()>0){
					displayTemperatureDS18D20(fromButton);
				}else{
					changePage();
				}
		}
	}else{
		displayLoad();
	}
}

void refreshDisplay(){
	refreshDisplayPage(false);
}

void selectCurrentDS18D20(){
	if(displayMode==1+maxMode-ds18d20Measurer.getItemCount()){
		currentDs18=0;
	}
	else{
		if(displayMode>=1+maxMode-ds18d20Measurer.getItemCount() && displayMode<maxMode){
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

	if(displayMode==MODE_TIME){
		displayRefreshTrigger.start(MODE_TIME_REFRESH_INTERVAL);
	}else{
		displayRefreshTrigger.start(MODE_OTHER_REFRESH_INTERVAL);
	}

	refreshDisplayPage(true);
}

//---------------------------------------------------------------------
//button handling
void onButtonMenuChanged(){
	if(!buttonMenu.isOn()){
		Serial.println(FPSTR("Menu button released"));
		changePage();
	}
}

void processTimeClientEvent(int8_t* time){
	Serial.println(timeClient.getCurrentTimeAsString('.'));
	//timeDisplay.display(time);
}

void postInitWebServer(){
	server.on(FPSTR(URL_SUBMIT_FORM_COMMANDS), HTTP_POST, [](){
		server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), executeCommand());
	});
	server.on(FPSTR(URL_SUBMIT_FORM_SETTINGS), HTTP_POST, [](){
		server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), setEspSettingsBoxValues());
	});
	server.on(FPSTR(URL_SUBMIT_FORM_SENSORS), HTTP_POST, [](){
		wifiHelper.checkAuthentication();
		server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), setSensorJson());
	});

	server.on(FPSTR(ESPSETTINGSBOX_GET_SIMPLE_JSON_PUBLISH_URL), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), espSettingsBox.getSimpleJson());
	});
	server.on(FPSTR(URL_GET_JSON_SETTINGS), HTTP_GET, [](){
		wifiHelper.checkAuthentication();
		String page=server.arg(FPSTR(MESSAGE_SERVER_ARG_PAGE));
		server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), espSettingsBox.getJson(page));
	});
	server.on(FPSTR(URL_GET_JSON_SENSORS), HTTP_GET, [](){
		wifiHelper.checkAuthentication();
		server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), getAllSensorsJson());
	});

	server.on(FPSTR(URL_GET_SENSORS_CURRNT_VALUES), HTTP_GET, [](){
		if(server.hasArg(FPSTR(MESSAGE_SERVER_ARG_SENSOR))){
			String arg=server.arg(FPSTR(MESSAGE_SERVER_ARG_SENSOR));

			if(arg==FPSTR(MESSAGE_SERVER_ARG_VAL_ALL)){
				wifiHelper.checkAuthentication();
				server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), getAllSensorsJson());
			}

			for(uint8_t i=0;i<ARRAY_SIZE(abstractItems);i++){
				if(arg==abstractItems[i]->getName()){
					server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), abstractItems[i]->getJson());
					break;
				}
			}
		}

		server.send(404, FPSTR(CONTENT_TYPE_JSON_UTF8), FPSTR(MESSAGE_STATUS_JSON_WIDGET_NOT_FOUND));
	});
}
//base functions
void measureSensors(){
	deviceHelper.update(abstractItems, ARRAY_SIZE(abstractItems));

	for(uint8_t i=0;i<ARRAY_SIZE(abstractItems);i++){
		Serial.println();
		Serial.println(abstractItems[i]->getJson());

	}

	//String result=deviceHelper.processAlarm(abstractItems, ARRAY_SIZE(abstractItems));

	deviceHelper.printDeviceDiagnostic();
}

//---------------------------------------------------------------------
//handle pirDetector events
void onPirDetectorChanged(){
	Serial.println("* Pir changed");
}

//---------------------------------------------------------------------
void loadSensors(){
	espSettingsBox.loadAbstractItemsFromFile(abstractItems, ARRAY_SIZE(abstractItems));
}

void saveSensors(){
	espSettingsBox.saveAbstractItemsToFile(abstractItems, ARRAY_SIZE(abstractItems));
}
//-----------------------------------------------------
String setSensorJson(){
	Serial.println(FPSTR(MESSAGE_ABSTRACT_ITEM_SET_SENSOR_VAL_SETTING_BEGIN));

	uint8_t argsProcessed=0;

	String sensorName=server.arg(FPSTR(MESSAGE_SERVER_ARG_CURRENT_SENSOR_NAME));
	String status=FPSTR(MESSAGE_ABSTRACT_ITEM_NOT_FOUND);

	Serial.print(FPSTR(MESSAGE_ABSTRACT_ITEM_SET_SENSOR_VAL_NAME_EQ));
	Serial.println(sensorName);

	AbstractItem* aitem=NULL;

	for(uint8_t i=0;i<ARRAY_SIZE(abstractItems);i++){
		if(abstractItems[i]->getName()==sensorName){
			aitem=abstractItems[i];
			break;
		}
	}

	if(aitem!=nullptr && aitem!=NULL){
		for(int i=0;i<server.args();i++){
			AbstractItemRequest req=AbstractItem::createitemRequest(server.argName(i),server.arg(i));

			if(req.valid){
				argsProcessed++;
				if(aitem->setFieldFromRequest(req)){
					status=FPSTR(MESSAGE_ABSTRACT_ITEM_OK);
				}
			}
		}

		Serial.print(FPSTR(MESSAGE_ABSTRACT_ITEM_SET_SENSOR_VAL_STATUS_EQ));
		Serial.println(status);

		espSettingsBox.saveAbstractItemToFile(aitem);
	}

	Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

	return "{\"status\":\""+status+"\",\"sensor\":\""+sensorName+"\"}";
}

String getAllSensorsJson(){
	return deviceHelper.getJson(abstractItems, ARRAY_SIZE(abstractItems));
}

String executeCommand(){
	wifiHelper.checkAuthentication();
	Serial.println(FPSTR(MESSAGE_COMMANDS_EXECUTE_COMMAND));

	String command=server.arg(FPSTR(MESSAGE_SERVER_ARG_CONFIRM_COMMAND));

	StatusMessage sm={FPSTR(MESSAGE_COMMANDS_ERROR),FPSTR(MESSAGE_COMMANDS_COMMAND_NOT_RECOGNIZED)};

	if(command==FPSTR(MESSAGE_SERVER_ARG_VAL_restart)){
		Serial.println(FPSTR(MESSAGE_COMMANDS_DEVICE_WILL_BE_RESTARTED));
		deviceHelper.createPostponedCommand(command);

		sm.setVals(FPSTR(MESSAGE_COMMANDS_OK),FPSTR(MESSAGE_COMMANDS_DEVICE_WILL_BE_RESTARTED_MSG));
	}
	if(command==FPSTR(MESSAGE_SERVER_ARG_VAL_recreateThingSpeak)){
		sm=thingSpeakHelper.recreateThingSpeaChannelskWithCheck(abstractItems,ARRAY_SIZE(abstractItems));
	}
	if(command==FPSTR(MESSAGE_SERVER_ARG_VAL_deleteSettings)){
		String msg=FPSTR(MESSAGE_COMMANDS_FILES_DELETED);
			   msg+=String(espSettingsBox.deleteSettingsFiles());
		sm.setVals(FPSTR(MESSAGE_COMMANDS_OK),msg);
	}

	return "{\"status\":\""+sm.status+"\",\"message\":\""+sm.message+"\"}";
}

void executePostPonedCommand(){
	deviceHelper.executePostponedCommand();
}

//----------espSettings save-------------------------------------------
String setEspSettingsBoxValues(){
	wifiHelper.checkAuthentication();
	boolean result=false;
	String page=server.arg(FPSTR(MESSAGE_SERVER_ARG_PAGE));

	for(uint8_t i=0;i<server.args();i++){
		String argName=server.argName(i);
		String argVal=server.arg(i);

		result=espSettingsBox.setSettingsValue(argName,argVal) || result;
	}

	if(result){
		espSettingsBox.saveSettingsJson();
	}

	return espSettingsBox.getJson(page);
}
//-------------------------Thing speak functions---------------------
void processThingSpeakPost(){
	thingSpeakHelper.sendItemsToThingSpeak(abstractItems, ARRAY_SIZE(abstractItems));
}
//------------------------------MQTT functions-----------------------------------------------------

void sendAbstractItemToHttp(AbstractItem* item){
	if(espSettingsBox.isHttpPostEnabled){
		wifiHelper.executeFormPostRequest(espSettingsBox.httpPostIp.toString(), item->getJson());
	}
}


