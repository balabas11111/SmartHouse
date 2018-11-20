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

WiFiClient wclient;
ESP8266WebServer server ( 80 );
//WebSocketsServer webSocket = WebSocketsServer(8081);

I2Chelper i2cHelper(D1,D2,false);
DisplayHelper displayHelper(true);

TimeTrigger sensorsTrigger(0,(espSettingsBox.refreshInterval*1000),true,measureSensors);
TimeTrigger thingSpeakTrigger(0,(espSettingsBox.postDataToTSInterval*1000),espSettingsBox.isThingSpeakEnabled,processThingSpeakPost);

TimeTrigger postPonedCommandTrigger(0,5000,false,nullptr);

PinDigital buttonLeft(VAR_NAME(buttonLeft),D7,onLeftButtonChanged);
PinDigital buttonRight(VAR_NAME(buttonRight),D6,onRightButtonChanged,1000);

PinDigital lampLeft(VAR_NAME(lampLeft),D8,onLeftLampChanged,OUTPUT,CHANGE,HIGH,LOW);
PinDigital lampRight(VAR_NAME(lampRight),D0,onRightLampChanged,OUTPUT,CHANGE,HIGH,LOW);

PinDigital acMeter(VAR_NAME(acMeter),D5,onAcMeterChanged,INPUT,CHANGE,HIGH,HIGH);

//Pir_Sensor pirDetector(VAR_NAME(pirDetector),A0,onPirDetectorChanged);

//NtpTimeClientService timeClient(&espSettingsBox,processTimeClientEvent,60000);

BME280_Sensor bmeMeasurer(20,VAR_NAME(bmeMeasurer));
BH1750_Sensor luxMeasurer(21,VAR_NAME(luxMeasurer));

//DHT22_Sensor dhtMeasurer(VAR_NAME(dhtSensor), D0, 22);
DS18D20_Sensor ds18d20Measurer(VAR_NAME(ds18d20Measurer), D3);

WiFiHelper wifiHelper("WiFiHelper",&espSettingsBox, &displayHelper, /*nullptr,*/&server,postInitWebServer,false);
ThingSpeakHelper thingSpeakHelper(&espSettingsBox,&wifiHelper);
MqttHelper mqttHelper(&espSettingsBox,wclient,processMqttEvent);


Loopable* loopArray[]={&wifiHelper,&mqttHelper,&sensorsTrigger,&buttonLeft,&buttonRight,&acMeter,&thingSpeakTrigger,&postPonedCommandTrigger};
Initializable* initializeArray[]={&espSettingsBox,&wifiHelper,&i2cHelper,&bmeMeasurer,&luxMeasurer,&ds18d20Measurer};

AbstractItem* abstractItems[]={&lampLeft,&lampRight,&bmeMeasurer,&luxMeasurer,&acMeter,&ds18d20Measurer};

DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray),espSettingsBox.alamSendInterval);

void setup() {
  deviceHelper.startDevice(espSettingsBox.DeviceId);

  espSettingsBox.printSettingsFile();
  deviceHelper.printDeviceDiagnostic();
  //espSettingsBox.printSpiffsInfo();

  deviceHelper.init(initializeArray, ARRAY_SIZE(initializeArray));
  deviceHelper.printDeviceDiagnostic();
  deviceHelper.displayDetails();

  deviceHelper.printDeviceDiagnostic();
  loadSensors();

  deviceHelper.printDeviceDiagnostic();

  measureSensors();
  deviceHelper.printDeviceDiagnostic();

  mqttHelper.begin(abstractItems, ARRAY_SIZE(abstractItems));
  deviceHelper.printDeviceDiagnostic();
  Serial.println("=========================Device Started=========================");
}

void loop() {
	deviceHelper.loop();
}

void postInitWebServer(){
	server.on("/submitForm_commands", HTTP_POST, [](){
			server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), executeCommand());
		});
	server.on(espSettingsBox.getSimpleJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), espSettingsBox.getSimpleJson());
	});
	server.on("/submitForm_settings", HTTP_POST, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), setEspSettingsBoxValues());
	});
	server.on("/getJson_settings", HTTP_GET, [](){
		wifiHelper.checkAuthentication();
		String page=server.arg("page");
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), espSettingsBox.getJson(page));
	});
	server.on("/submitForm_sensors", HTTP_POST, [](){
		wifiHelper.checkAuthentication();
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), setSensorJson());
	});
	server.on("/getJson_sensors", HTTP_GET, [](){
		wifiHelper.checkAuthentication();
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), getAllSensorsJson());
	});

	server.on(buttonLeft.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), buttonLeft.getJson());
	});
	server.on(buttonRight.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), buttonRight.getJson());
	});

	server.on(lampLeft.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lampLeft.getJson());
	});
	server.on(lampRight.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lampRight.getJson());
	});
	server.on(lampLeft.getSetValueUrl(), HTTP_ANY, [](){
		handleLampChange(&lampLeft);
	});
	server.on(lampRight.getSetValueUrl(), HTTP_ANY, [](){
		handleLampChange(&lampRight);
	});
	server.on(acMeter.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), acMeter.getJson());
	});
/*
	server.on(pirDetector.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), pirDetector.getJson());
	});
*/
	server.on(bmeMeasurer.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), bmeMeasurer.getJson());
	});
	server.on(luxMeasurer.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), luxMeasurer.getJson());
	});
	/*server.on(dhtMeasurer.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), dhtMeasurer.getJson());
	});*/
	server.on(ds18d20Measurer.getJsonPublishUrl(), HTTP_GET, [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), ds18d20Measurer.getJson());
	});

	server.on("/getI2C", [](){
		server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), i2cHelper.scan());
	});
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
	Serial.print("AcMeter=");
	Serial.println(acMeter.isOn());
}
//---------------------------------------------------------------------
//handle lamp events
void handleLampChange(PinDigital* lamp){
	wifiHelper.checkAuthentication();
	int8_t on=server.arg("val").toInt();

	setLampValue(lamp, on);

	server.send(200, FPSTR(CONTENT_TYPE_TEXT_HTML), lamp->getSimpleJson());
}

void setLampValue(PinDigital* lamp,int8_t on){
	if(on==-1){
		lamp->change();
	}
	lamp->setVal(on);

	lamp->printValues();

	//senDAbstractItemToMqtt(lamp);
}

void onLeftLampChanged(){
	Serial.println("* Left lamp changed");
}

void onRightLampChanged(){
	Serial.println("* Right lamp changed");
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
	Serial.println("begin settings parsing");

	Serial.print("args count =");
	Serial.println(server.args());

	uint8_t argsProcessed=0;

	String sensorName=server.arg("currentSensor_name");
	String status="NotFound";

	AbstractItem* aitem;

	for(uint8_t i=0;i<ARRAY_SIZE(abstractItems);i++){
		if(abstractItems[i]->getName()==sensorName){
			aitem=abstractItems[i];
			break;
		}
	}

	if(aitem!=nullptr){
		for(int i=0;i<server.args();i++){
			AbstractItemRequest req=AbstractItem::createitemRequest(server.argName(i),server.arg(i));

			if(req.valid){
				argsProcessed++;
				if(aitem->setFieldFromRequest(req)){
					status="Ok";
				}
			}
		}

		espSettingsBox.saveAbstractItemToFile(aitem);
	}

	return "{\"status\":\""+status+"\",\"sensor\":\""+sensorName+"\"}";
}


String getAllSensorsJson(){
	delay(1);
	String result=deviceHelper.getJson(abstractItems, ARRAY_SIZE(abstractItems));

	Serial.print("AllSensors json=");
	Serial.println(result);

	return result;
}

String postPonedCommand="";

String executeCommand(){
	wifiHelper.checkAuthentication();
	Serial.println("Executing command");

	String command=server.arg("confirm_command");

	StatusMessage sm={"Error","Команда не распознана"};

	String status="Error";
	String message="Команда не распознана";

	if(command=="restart"){
		Serial.println("Device will be restarted");
		postPonedCommandTrigger.setHandler(executePostPonedCommand);
		postPonedCommandTrigger.start();
		postPonedCommand=command;

		status="Ok";
		message="Устройство будет перезапущено. Дождитесь перезагрузки страницы";
	}
	if(command=="recreateThingSpeak"){
		if(!espSettingsBox.isThingSpeakEnabled){
			status="Failed";
			message="Публикация ThingSpeak не разрешена";
		}else
		if(espSettingsBox.thSkUsrKey=="" || espSettingsBox.thSkUsrKey=="EmptyKey"){
			status="Failed";
			message="Не задан пользователь ThingSpeak";
		}else{
			status="Ok";
			message=thingSpeakHelper.recreateThingSpeak(abstractItems,ARRAY_SIZE(abstractItems));
		}
	}
	if(command=="deleteSettings"){
		status="Ok";
		message="Удалено файлов :"+String(espSettingsBox.deleteSettingsFiles());
	}

	return "{\"status\":\""+status+"\",\"message\":\""+message+"\"}";
}

void executePostPonedCommand(){
	if(postPonedCommand=="restart"){
		Serial.print("Executing restart");
		ESP.restart();
	}
}

//----------espSettings save-------------------------------------------
String setEspSettingsBoxValues(){
	wifiHelper.checkAuthentication();
	boolean result=false;
	String page=server.arg("page");

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

void senDAbstractItemToMqtt(AbstractItem* item){
	Serial.println("----------Sending message---------");
	for(uint8_t i=0;i<item->getItemCount();i++){
		String queue=item->getItem(i).queue;

		if(queue.length()!=0){
			String val=item->getValStr(i);

			Serial.print("queue=");
			Serial.println(queue);

			queue.replace(espSettingsBox.thSkRManageKey, espSettingsBox.thSkWManageKey);
			queue.replace("subscribe", "publish");

			//channels/623698/subscribe/fields/field2/N9EQ8RTYQ7ZXYR8T
			//channels/<channelID>/publish/fields/field<fieldnumber>/<apikey>


			Serial.print("target queue=");
			Serial.print(queue);

			Serial.print(" val=");
			Serial.println(val);

			if(espSettingsBox.isMqttEnabled){
				boolean res=mqttHelper.publish(queue, val);
				Serial.print("res=");
				Serial.println(res);
			}else{
				Serial.println("Mqtt is not enabled");
			}
			/*
			if(espSettingsBox.sendItemsToBaseQUeue){
				mqttHelper.publish(item->getJson());
			}
			*/
		}
	}
	Serial.println("---------------------------");
}

void sendAbstractItemToHttp(AbstractItem* item){
	if(espSettingsBox.isHttpPostEnabled){
		wifiHelper.executeFormPostRequest(espSettingsBox.httpPostIp.toString(), item->getJson());
	}
}

void measureSensors(){
	deviceHelper.update(abstractItems, ARRAY_SIZE(abstractItems));
	deviceHelper.processAlarm(abstractItems, ARRAY_SIZE(abstractItems));
}

void processMqttEvent(String topic,String message){
	Serial.println("----------Message received---------");
	Serial.print("topic=");
	Serial.print(topic);
	Serial.print("; message='");
	Serial.print(message);
	Serial.println("';");

	boolean result=false;

	for(uint8_t i=0;i<ARRAY_SIZE(abstractItems);i++){
		result=abstractItems[i]->processMqValue(topic, message);
		if(result){
			break;
		}
	}

	Serial.print("result=");
	Serial.println(result);

	Serial.println("---------------------------");

	deviceHelper.printDeviceDiagnostic();
}
