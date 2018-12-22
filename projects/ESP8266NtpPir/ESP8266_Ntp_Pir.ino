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
#include "Display_Custom/DisplayHelper_TM1637_Clock_PIR.h"

#include "TM1637.h"

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

/* NTP Clock Device

 Connection

  PirSensor          A0

  AC_RELE            D0
  I2C                D1, D2
  DS18D20            D3

  TM1637 - Display   D6 D7

  Beeper             D5
  Button             D8


 */
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

TimeTrigger sensorsTrigger(0,(espSettingsBox.refreshInterval*1000),true,measureSensors);
TimeTrigger thingSpeakTrigger(0,(espSettingsBox.postDataToTSInterval*1000),espSettingsBox.isThingSpeakEnabled,processThingSpeakPost);

NtpTimeClientService timeClient(&espSettingsBox,nullptr,60000);
DisplayHelper_TM1637_Clock_PIR displayHelper(&timeDisplay,&espSettingsBox,&bmeMeasurer,&ds18d20Measurer,&timeClient);

WiFiHelper wifiHelper(&espSettingsBox, nullptr, &server,postInitWebServer,false);
ThingSpeakHelper thingSpeakHelper(&espSettingsBox,&wifiHelper);

Loopable* loopArray[]={&wifiHelper,&buttonMenu,&thingSpeakTrigger,&timeClient,&displayHelper/*,&pirDetector*/};

AbstractItem* abstractItems[]={&bmeMeasurer,&ds18d20Measurer,&pirDetector,&AC_Rele1};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray),120000);

void setup() {
  Serial.begin(115200);

  displayHelper.init();

  deviceHelper.startDevice(espSettingsBox.DeviceId);

  espSettingsBox.init();

  beeper.init();

  i2cHelper.init();

  httpUpdater.setup(&server);
  wifiHelper.init();

  timeClient.init();
  bmeMeasurer.init();
  ds18d20Measurer.init();

  loadSensors();
  measureSensors();

  displayHelper.postProcessConnection(wifiHelper.isAP(),wifiHelper.getIpStr());

  deviceHelper.printDeviceDiagnostic();

  if(i2cHelper.getDevCount()==0){
	  beeper.longBeep();
  }else if(ds18d20Measurer.getItemCount()==0){
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

void refreshDisplay(){
	//Serial.println(FPSTR("main refresh display"));
	displayHelper.refreshDisplay();
}


//---------------------------------------------------------------------
//button handling
void onButtonMenuChanged(){
	if(!buttonMenu.isOn()){
		Serial.println(FPSTR("Menu button released"));
		displayHelper.changePage();
	}
}

void processTimeClientEvent(int8_t* time){
	Serial.println(FPSTR("processTimeClientEvent"));
	Serial.println(timeClient.getCurrentTimeAsString('.'));
	Serial.println(timeClient.getCurrentDateAsString('.'));
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

			if(arg==FPSTR(NTP_TIME_CLIENT_SERVICE_NAME)){
				wifiHelper.checkAuthentication();
				server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), timeClient.getJson());
			}

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


