#include <Arduino.h>
#include <BH1750_Sensor.h>
#include <BME280_Sensor.h>
#include <Hash.h>
#include <ESP8266WiFi.h>
#include "EspSettingsBox.h"
#include "MqttHelper.h"
#include "Loopable.h"
#include "FS.h"
#include "I2Chelper.h"
#include "DisplayHelper.h"
#include "WiFiHelper.h"
#include <Wire.h>

#include <ESP8266WebServer.h>
#include "TimeTrigger.h"

#include <DeviceHelper.h>
#include <ThingSpeakHelper.h>
#include <PinDigital.h>
#include <Pir_Sensor.h>

#include "BH1750_Sensor.h"
#include "BME280_Sensor.h"
#include "DHT22_Sensor.h"
#include "DS18D20_Sensor.h"
#include "StatusMessage.h"

//#include "MessageSenderTelegram.h"
#include "WiFiClientSecure.h"
#include <TelegramBot.h>

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])
#define VAR_NAME(var) #var

#define DISPLAY_LOOPS 1

#define FIRMVARE_VERSION "b@l@bas-soft ONOFF v0.0.5"

//Robot Dyn//vem  // 12E
/*
#define A0_PIN 17
#define D0_PIN 16 //GPIO016 ////beeper
#define D1_PIN 5  //GPIO05  //DallasTemp           PIN_WIRE_SCL
#define D2_PIN 4  //GPIO04  //OLED //SDA //blue    PIN_WIRE_SDA
#define D3_PIN 0  //GPIO00  //OLED //SCL //green
#define D4_PIN 2  //GPIO02  //RedLed               INTERNAL_LED_PIN
#define D5_PIN 14 //GPIO14  //DHT
#define D6_PIN 12 //GPIO12  //DallasTemp red led2
#define D7_PIN 13 //GPIO13  //GreenLed
#define D8_PIN 15 //GPIO15  //button
#define SD3_PIN 10
#define SD2_PIN 9
*/
#define RX_PIN 3
#define TX_PIN 1


#define HUMAN_PRESENTED LOW
#define HUMAN_NOT_PRESENTED HIGH

EspSettingsBox espSettingsBox("",true,true);

//WiFiClient wclient;
ESP8266WebServer server ( 80 );
//WebSocketsServer webSocket = WebSocketsServer(8081);

I2Chelper i2cHelper(D1,D2,false);
DisplayHelper displayHelper(true);

TimeTrigger sensorsTrigger(0,(espSettingsBox.refreshInterval*1000),true,measureSensors);
TimeTrigger thingSpeakTrigger(0,(espSettingsBox.postDataToTSInterval*1000),espSettingsBox.isThingSpeakEnabled,processThingSpeakPost);

PinDigital buttonLeft(FPSTR(SENSOR_buttonLeft),D7,onLeftButtonChanged);
PinDigital buttonRight(FPSTR(SENSOR_buttonRight),D6,onRightButtonChanged,1000);

PinDigital lampLeft(FPSTR(SENSOR_lampLeft),D8,onLeftLampChanged,OUTPUT,CHANGE,HIGH,LOW);
PinDigital lampRight(FPSTR(SENSOR_lampRight),D0,onRightLampChanged,OUTPUT,CHANGE,HIGH,LOW);

PinDigital acMeter(FPSTR(SENSOR_acMeter),D5,onAcMeterChanged,INPUT,CHANGE,HIGH,HIGH);

//Pir_Sensor pirDetector(VAR_NAME(pirDetector),A0,onPirDetectorChanged);
//NtpTimeClientService timeClient(&espSettingsBox,processTimeClientEvent,60000);

BME280_Sensor bmeMeasurer(FPSTR(SENSOR_bmeMeasurer),20);
BH1750_Sensor luxMeasurer(FPSTR(SENSOR_luxMeasurer),21);

//DHT22_Sensor dhtMeasurer(VAR_NAME(dhtSensor), D0, 22);
DS18D20_Sensor ds18d20Measurer(FPSTR(SENSOR_ds18d20Measurer), D3);

WiFiHelper wifiHelper(&espSettingsBox, &displayHelper, &server,postInitWebServer,false);
ThingSpeakHelper thingSpeakHelper(&espSettingsBox,&wifiHelper);
//MqttHelper mqttHelper(&espSettingsBox,wclient,processMqttEvent);

const char BotToken[] = "737840576:AAH_9-PM8knquJ3x1GN-sOTX4NGPNdU50iE";

//WiFiClientSecure net_ssl;
//TelegramBot bot (BotToken, net_ssl);
//UniversalTelegramBot bot(espSettingsBox.telegramApiKey,client);
//MessageSenderTelegram messageSenderTelegram(&bot);

Loopable* loopArray[]={&wifiHelper,&buttonLeft,&buttonRight,&acMeter,&sensorsTrigger,&thingSpeakTrigger};

AbstractItem* abstractItems[]={&lampLeft,&lampRight,&bmeMeasurer,&luxMeasurer,&acMeter,&ds18d20Measurer};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray),120000);



void setup() {
  deviceHelper.startDevice(espSettingsBox.DeviceId);

  espSettingsBox.init();
  wifiHelper.init();
  i2cHelper.init();
  bmeMeasurer.init();
  luxMeasurer.init();
  ds18d20Measurer.init();

  //deviceHelper.displayDetails();

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
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), executeCommand());
	});
	server.on(FPSTR(URL_SUBMIT_FORM_SETTINGS), HTTP_POST, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), setEspSettingsBoxValues());
	});
	server.on(FPSTR(ESPSETTINGSBOX_GET_SIMPLE_JSON_PUBLISH_URL), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), espSettingsBox.getSimpleJson());
	});
	server.on(FPSTR(URL_GET_JSON_SETTINGS), HTTP_GET, [](){
		wifiHelper.checkAuthentication();
		String page=server.arg(FPSTR(MESSAGE_SERVER_ARG_PAGE));
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), espSettingsBox.getJson(page));
	});
	server.on(FPSTR(URL_GET_JSON_SENSORS), HTTP_GET, [](){
		wifiHelper.checkAuthentication();
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), getAllSensorsJson());
	});
	server.on(FPSTR(URL_SUBMIT_FORM_SENSORS), HTTP_POST, [](){
		wifiHelper.checkAuthentication();
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), setSensorJson());
	});

	server.on(FPSTR(URL_GET_SENSORS_CURRNT_VALUES), HTTP_GET, [](){
		if(server.hasArg(FPSTR(MESSAGE_SERVER_ARG_SENSOR))){
			String arg=server.arg(FPSTR(MESSAGE_SERVER_ARG_SENSOR));

			if(arg==FPSTR(MESSAGE_SERVER_ARG_VAL_ALL)){
				wifiHelper.checkAuthentication();
				server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), getAllSensorsJson());
			}

			for(uint8_t i=0;i<ARRAY_SIZE(abstractItems);i++){
				if(arg==abstractItems[i]->getName()){
					server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), abstractItems[i]->getJson());
					break;
				}
			}
		}

		server.send(404, FPSTR(CONTENT_TYPE_JSON), FPSTR(MESSAGE_STATUS_JSON_WIDGET_NOT_FOUND));
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

				server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lamp->getSimpleJson());
			}
		}

		server.send(404, FPSTR(CONTENT_TYPE_JSON), FPSTR(MESSAGE_STATUS_JSON_WIDGET_NOT_FOUND));
	});
	/*
    server.on(FPSTR(URL_JSON_SENSORS), HTTP_GET, [](){

	});
	server.on(FPSTR(URL_SET_LAMP_LEFT), HTTP_ANY, [](){
		handleLampChange(&lampLeft);
	});
	server.on(FPSTR(URL_SET_LAMP_RIGHT), HTTP_ANY, [](){
		handleLampChange(&lampRight);
	});

	server.on(FPSTR(URL_GET_BUTTON_LEFT), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), buttonLeft.getJson());
	});
	server.on(FPSTR(URL_GET_BUTTON_RIGHT), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), buttonRight.getJson());
	});

	server.on(FPSTR(URL_GET_LAMP_LEFT), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lampLeft.getJson());
	});
	server.on(FPSTR(URL_GET_LAMP_RIGHT), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lampRight.getJson());
	});
	server.on(FPSTR(URL_GET_ACMETER), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), acMeter.getJson());
	});
	server.on(FPSTR(URL_GET_BME_MEASURER), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), bmeMeasurer.getJson());
	});
	server.on(FPSTR(URL_GET_LUX_MEASURER), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), luxMeasurer.getJson());
	});
	server.on(FPSTR(URL_GET_DS18D20_MEASURER), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), ds18d20Measurer.getJson());
	});
	*/

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
	//lamp->printValues();
}

void onLeftLampChanged(){
	//Serial.println("* Left lamp changed");
}

void onRightLampChanged(){
	//Serial.println("* Right lamp changed");
}

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
//-----------------------------Telegram functions-----------------------------------
void send_message(String message) {
	Serial.println(message);
}

/*
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

