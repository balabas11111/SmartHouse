#include <Arduino.h>
#include <BH1750_Sensor.h>
#include <BME280_Sensor.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include "EspSettingsBox.h"
//#include "MqttHelper.h"
#include "Loopable.h"
#include "FS.h"
#include "I2Chelper.h"
#include "WiFiHelper.h"
#include <Wire.h>

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include "TimeTrigger.h"

#include <DeviceHelper.h>
#include <DisplayHelperAbstract.h>
#include <ThingSpeakHelper.h>
#include <PinDigital.h>
#include <Pir_Sensor.h>

#include "BH1750_Sensor.h"
#include "BME280_Sensor.h"
#include "DHT22_Sensor.h"
#include "DS18D20_Sensor.h"
#include "StatusMessage.h"

#include "PinDigitalVirtual.h"
#include "PCF8574Helper.h"

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

#define DISPLAY_LOOPS 1

#define FIRMVARE_VERSION "b@l@bas-soft ONOFF v0.0.5"

#define HUMAN_PRESENTED LOW
#define HUMAN_NOT_PRESENTED HIGH

EspSettingsBox espSettingsBox(true,true);

ESP8266WebServer server ( 80 );
ESP8266HTTPUpdateServer httpUpdater(true);

I2Chelper i2cHelper(D1,D2,false);
DisplayHelperAbstract displayHelper(&espSettingsBox);

//TimeTrigger sensorsTrigger(0,(espSettingsBox.refreshInterval*1000),true,measureSensors);
//TimeTrigger thingSpeakTrigger(0,(espSettingsBox.postDataToTSInterval*1000),espSettingsBox.isThingSpeakEnabled,processThingSpeakPost);

PinDigital buttonLeft(FPSTR(SENSOR_buttonLeft),D7,onLeftButtonChanged);
PinDigital buttonRight(FPSTR(SENSOR_buttonRight),D6,onRightButtonChanged,1000);

//PinDigital lampLeft(FPSTR(SENSOR_lampLeft),D8,onLeftLampChanged,OUTPUT,CHANGE,HIGH,LOW);
//PinDigital lampRight(FPSTR(SENSOR_lampRight),D0,onRightLampChanged,OUTPUT,CHANGE,HIGH,LOW);
PinDigital acMeter(FPSTR(SENSOR_acMeter),D5,onAcMeterChanged,INPUT,CHANGE,HIGH,HIGH);

PinDigitalVirtual lampLeft(FPSTR(SENSOR_lampLeft));
PinDigitalVirtual lampRight(FPSTR(SENSOR_lampRight));
PinDigitalVirtual lamp2(FPSTR(SENSOR_lamp2));
PinDigitalVirtual lamp3(FPSTR(SENSOR_lamp3));
//PinDigitalVirtual acMeter(FPSTR(SENSOR_acMeter),onAcMeterChanged,HIGH,HIGH);

PinDigitalVirtual* virtualItems[]={&lampLeft,&lampRight,&lamp2,&lamp3};

//Pir_Sensor pirDetector(VAR_NAME(pirDetector),A0,onPirDetectorChanged);
//NtpTimeClientService timeClient(&espSettingsBox,processTimeClientEvent,60000);

BME280_Sensor bmeMeasurer(FPSTR(SENSOR_bmeMeasurer));
BH1750_Sensor luxMeasurer(FPSTR(SENSOR_luxMeasurer));

//DHT22_Sensor dhtMeasurer(VAR_NAME(dhtSensor), D0, 22);
DS18D20_Sensor ds18d20Measurer(FPSTR(SENSOR_ds18d20Measurer), D3);

WiFiHelper wifiHelper(&espSettingsBox, &displayHelper, &server,postInitWebServer,false);
ThingSpeakHelper thingSpeakHelper(&espSettingsBox,&wifiHelper);

PCF8574Helper extender(virtualItems,ARRAY_SIZE(virtualItems));

Loopable* loopArray[]={&extender,&wifiHelper,&buttonLeft,&buttonRight,&acMeter/*,&sensorsTrigger,&thingSpeakTrigger*/};

AbstractItem* abstractItems[]={&lampLeft,&lampRight,&lamp2,&lamp3,&bmeMeasurer,&luxMeasurer,&acMeter,&ds18d20Measurer};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray),120000);


void setup() {
  Serial.begin(115200);
  deviceHelper.startDevice(espSettingsBox.DeviceId);

  espSettingsBox.init();

  i2cHelper.init();

  extender.init();
  httpUpdater.setup(&server);
  wifiHelper.init();
  bmeMeasurer.init();
  luxMeasurer.init();
  ds18d20Measurer.init();

  loadSensors();
  measureSensors();

  //mqttHelper.begin(abstractItems, ARRAY_SIZE(abstractItems));
  deviceHelper.printDeviceDiagnostic();
  Serial.println(FPSTR(MESSAGE_DEVICE_STARTED));
}

void loop() {
	deviceHelper.loop();
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

	server.on(FPSTR(URL_SET_DIGITAL_PIN_CURRENT_VALUE), HTTP_ANY, [](){

		wifiHelper.checkAuthentication();

		if(server.hasArg(FPSTR(MESSAGE_SERVER_ARG_SENSOR)) && server.hasArg(FPSTR(MESSAGE_SERVER_ARG_VAL))){
			String arg=server.arg(FPSTR(MESSAGE_SERVER_ARG_SENSOR));
			PinDigital* lamp=NULL;

			if(arg==lampLeft.getName()){
				lamp=&lampLeft;
			}else
			if(arg==lampRight.getName()){
				lamp=&lampRight;
			}

			if(lamp!=NULL){
				int8_t on=server.arg(FPSTR(MESSAGE_SERVER_ARG_VAL)).toInt();
				setLampValue(lamp, on);

				server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8), lamp->getSimpleJson());
			}
		}

		server.send(404, FPSTR(CONTENT_TYPE_JSON_UTF8), FPSTR(MESSAGE_STATUS_JSON_WIDGET_NOT_FOUND));
	});
}
//base functions
void measureSensors(){
	deviceHelper.update(abstractItems, ARRAY_SIZE(abstractItems));
	String result=deviceHelper.processAlarm(abstractItems, ARRAY_SIZE(abstractItems));

	deviceHelper.printDeviceDiagnostic();
}

//---------------------------------------------------------------------
//button handling
void onLeftButtonChanged(){
	setLampValue(&lampLeft, buttonLeft.isOn());
}

void onRightButtonChanged(){
	setLampValue(&lampRight, !(lampRight.isOn()));
}

void onAcMeterChanged(){
	acMeter.printValues();
}
//---------------------------------------------------------------------
//handle lamp events
void setLampValue(PinDigital* lamp,uint8_t on){
	lamp->setVal(on);
}

void onLeftLampChanged(){}

void onRightLampChanged(){}

//---------------------------------------------------------------------
//handle pirDetector events
void onPirDetectorChanged(){
	//Serial.println("* Pir changed");
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
/*
//-----------------------------Telegram functions-----------------------------------
void send_message(String message) {
	Serial.println(message);
}

void processMqttEvent(String topic,String message){
	mqttHelper.processMqttEvent(topic, message, abstractItems, ARRAY_SIZE(abstractItems));
	deviceHelper.printDeviceDiagnostic();
}

void handleLampChange(PinDigital* lamp){
	wifiHelper.checkAuthentication();
	int8_t on=server.arg("val").toInt();

	setLampValue(lamp, on);

	server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lamp->getSimpleJson());
}
*/

