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

#include "Loopable.h"
#include "I2Chelper.h"
#include "WiFiHelper.h"

#include "NtpTimeClientService.h"
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
  SignalLed PIR      D4

  TM1637 - Display   D6 D7

  Beeper             D5
  Button             D8


 */
#define HUMAN_PRESENTED LOW
#define HUMAN_NOT_PRESENTED HIGH

ESPSett_Ntp espSett_Ntp;
ESPSett_Telegram espSett_Telegram;

ESPExtraSettingsBox* extraBoxes[]={&espSett_Ntp,&espSett_Telegram};

EspSettingsBox espSettingsBox(FPSTR("NTP clock with PIR sensor"),extraBoxes,ARRAY_SIZE(extraBoxes));
//EspSettingsBox espSettingsBox(FPSTR("NTP clock with PIR sensor"));

BeeperB beeper(D5,HIGH,LOW,true,false);

I2Chelper i2cHelper(D1,D2,false);

//PinDigital buttonMenu(FPSTR(SENSOR_buttonLeft),D8,onButtonMenuChanged);
PinDigital buttonMenu(FPSTR(SENSOR_buttonLeft),D8,onButtonChanged);
//PinDigital AC_Rele1(FPSTR(SENSOR_AC_Rele1),D0,LOW,HIGH);
PinDigital signalLed(FPSTR(SENSOR_signalLed),D0,LOW,HIGH);
Pir_Sensor pirDetector(VAR_NAME(pirDetector),A0,onPirDetectorChanged,700,30000);

DS18D20_Sensor ds18d20Measurer(FPSTR(SENSOR_ds18d20Measurer), D3);
BME280_Sensor bmeMeasurer(FPSTR(SENSOR_bmeMeasurer));

ESP8266WebServer server ( 80 );
ESP8266HTTPUpdateServer httpUpdater(true);

TM1637 timeDisplay(D6,D7);

TimeTrigger sensorsTrigger(0,(espSettingsBox.refreshInterval*1000),false,updateSensors);
TimeTrigger thingSpeakTrigger(0,(espSettingsBox.postDataToTSInterval*1000),false,processThingSpeakPost);

NtpTimeClientService timeClient(&espSettingsBox,nullptr,60000);
DisplayHelper_TM1637_Clock_PIR displayHelper(&timeDisplay,&espSettingsBox,&bmeMeasurer,&ds18d20Measurer,&timeClient);


WiFiHelper wifiHelper(&espSettingsBox, nullptr, &server,postInitWebServer,false);
ThingSpeakHelper thingSpeakHelper(&espSettingsBox,&wifiHelper);

Loopable* loopArray[]={&wifiHelper,&buttonMenu,&thingSpeakTrigger,&timeClient,
						&displayHelper,&pirDetector};

AbstractItem* abstractItems[]={&bmeMeasurer,&ds18d20Measurer,&pirDetector,&signalLed};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray),120000);

void initComponents(){
	displayHelper.init();
	beeper.init();
	i2cHelper.init();
	httpUpdater.setup(&server);
	displayHelper.displayConn();
	wifiHelper.init();

	timeClient.init();
	bmeMeasurer.init();
	ds18d20Measurer.init();

	loadSensors();

	sensorsTrigger.init();
}

void setup() {
  Serial.begin(115200);

  deviceHelper.printDeviceDiagnosticNoSpiff();

  espSettingsBox.init();

  deviceHelper.printDeviceDiagnostic();
  espSettingsBox.printSpiffsInfo();

  displayHelper.init();

  //espSettingsBox.testExtraBoxFunctionality();

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
	//buttonMenu.loop();
}

//test functions--------------------------------------
void measureSensors(){
	Serial.println("measuresensors");
}

void processThingSpeakPost(){
	Serial.println("process ThingSpeak post");
}

int8_t symbolKey=0;

void onButtonChanged(){
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
		if(symbolKey>=26){
			symbolKey=0;
		}
	}
}

//---------------------------------------------------------------------
//handle pirDetector events
void onPirDetectorChanged(){
	signalLed.turnOnOff(pirDetector.isOn());
}

//---------------------------------------------------------------------
void playPostInitSounds(){
	if(i2cHelper.getDevCount()==0){
		  beeper.longBeep();
	  }else if(ds18d20Measurer.getItemCount()==0){
		  beeper.shortBeep();
		  beeper.shortBeep();
		  beeper.shortBeep();
	  }else{
		  beeper.shortBeep();
	  }
}

//---------------------------------------------------------------------
//button handling
void onButtonMenuChanged(){
	if(!buttonMenu.isOn()){
		Serial.println(FPSTR("Menu button released"));
		displayHelper.changePage();
	}
}

//-------------Web server functions-------------------------------------
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
/*
			if(arg==FPSTR(NTP_TIME_CLIENT_SERVICE_NAME)){
				wifiHelper.checkAuthentication();
				server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), timeClient.getJson());
			}
*/
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
void updateSensors(){
	deviceHelper.update(abstractItems, ARRAY_SIZE(abstractItems));

	for(uint8_t i=0;i<ARRAY_SIZE(abstractItems);i++){
		Serial.println();
		Serial.println(abstractItems[i]->getJson());
	}

	deviceHelper.printDeviceDiagnostic();
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
void executeThingSpeakPost(){
	thingSpeakHelper.sendItemsToThingSpeak(abstractItems, ARRAY_SIZE(abstractItems));
}
//------------------------------MQTT functions-----------------------------------------------------

void sendAbstractItemToHttp(AbstractItem* item){
	if(espSettingsBox.isHttpPostEnabled){
		wifiHelper.executeFormPostRequest(espSettingsBox.httpPostIp.toString(), item->getJson());
	}
}


