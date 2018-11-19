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
#include <PinDigital.h>
#include <Pir_Sensor.h>
#include "ConfigStorage.h"
#include "BH1750_Sensor.h"
#include "BME280_Sensor.h"
#include "DHT22_Sensor.h"
#include "DS18D20_Sensor.h"

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
	int8_t on=buttonLeft.isOn();

	setLampValue(&lampLeft, on);
	//buttonLeft.printValues();
}

TimeTrigger rightButtonClickTrigger(0,1000,true,clickTriggerFinished);

void clickTriggerFinished(){
	rightButtonClickTrigger.stop();
	Serial.println("CLick timed out");
}

void onRightButtonChanged(){
	setLampValue(&lampRight, !(lampRight.isOn()));
	//buttonRight.printValues();
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

	String status="Error";
	String message="Команда не распознана";

	if(command=="restart"){
		Serial.println("Device will be restarted");
		postPonedCommandTrigger.setHandler(executePostPonedCommand);
		postPonedCommandTrigger.saveTime();
		postPonedCommandTrigger.setActive(true);
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
			message=recreateThingSpeak();
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
String recreateThingSpeak(){
	Serial.println(FPSTR(MESSAGE_THINGSPEAK_CHANNEL_CREATE_STARTED));
	String result="";

	String commandGet="api_key="+espSettingsBox.thSkUsrKey;
	//String commandSet="api_key="+espSettingsBox.thSkUsrKey;

	uint8_t countGet=0;
	uint8_t countSet=0;

	for(uint8_t i=0;i<ARRAY_SIZE(abstractItems);i++){
		AbstractItem* item=abstractItems[i];

		if(item->getAutoCreateChannel()){
			for(uint8_t j=0;j<item->getItemCount();j++){
				if(item->getFieldId(j)!=0){

					countGet++;
					commandGet+="&field"+item->getFieldIdStr(j)+"="+item->getDescr(j);

					//Serial.println(item->getName()+" "+item->getFIeld);

					if(item->getSetAllowed(j)){
						countSet++;
						//commandSet+="&field"+item->getFieldIdStr(j)+"="+item->getDescr(j);
					}
				}
			}
		}
	}

	if(countGet!=0){

		commandGet+="&name=";
		commandGet+=espSettingsBox.DeviceLocation+" "+espSettingsBox.DeviceId;
		commandGet+="&description=";
		commandGet+=espSettingsBox.DeviceDescription+" "+espSettingsBox.DeviceKind;

		String getResult=wifiHelper.executeFormPostRequest(FPSTR(MESSAGE_THINGSPEAK_CREATE_CHANNEL_URL),commandGet);
		espSettingsBox.saveThingSpeakChannelCreation(getResult);
		delay(10);
		deviceHelper.printDeviceDiagnostic();
		//Serial.println(getResult);
	}


	if(countSet!=0){
			//---------------------------------------------------------------------------
			for(uint8_t i=0;i<ARRAY_SIZE(abstractItems);i++){
				AbstractItem* item=abstractItems[i];
				boolean doSave=false;

				if(item->getAutoCreateChannel()){
					for(uint8_t j=0;j<item->getItemCount();j++){
						if(item->getFieldId(j)!=0 && item->getSetAllowed(j)){
							String channel="channels/"+String(espSettingsBox.thSkChId)
									+"/subscribe/fields/field"+item->getFieldIdStr(j)
									+"/"+espSettingsBox.thSkRKey;

							//Subscribes
							//channels/623698/subscribe/fields/field1/XQ4QSQQKEKMRJ4DK
							//channels/623698/subscribe/fields/field2/XQ4QSQQKEKMRJ4DK
							//publish
							//channels/623698/publish/fields/field1/N9EQ8RTYQ7ZXYR8T
							//channels/623698/publish/fields/field2/N9EQ8RTYQ7ZXYR8T

							item->setQueue(j, channel);
							doSave=true;
							delay(10);
						}
					}

					if(doSave){
						espSettingsBox.saveAbstractItemToFile(item);
					}
				}

				deviceHelper.printDeviceDiagnostic();
			}
			//---------------------------------------------------------------------------
		deviceHelper.printDeviceDiagnostic();
	}

	result+=String(countGet)+" каналов записи;";
	result+=" "+String(countSet)+" каналов управления;";

	return result;
}

void processThingSpeakPost(){
	if(espSettingsBox.isThingSpeakEnabled){
		Serial.println(FPSTR(MESSAGE_THINGSPEAK_SEND_STARTED));

		uint8_t count=0;

		String baseUrl=FPSTR(MESSAGE_THINGSPEAK_BASE_URL);
		String params="";

		for(uint8_t i=0;i<ARRAY_SIZE(abstractItems);i++){
			params+=constructThingSpeakParameters(abstractItems[i]);
			count++;
		}

		if(params!=""){

			wifiHelper.executeGetRequest(baseUrl+espSettingsBox.thSkWKey+params);
		}

		Serial.print(count);
		Serial.println(FPSTR(MESSAGE_DONE));
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		deviceHelper.printDeviceDiagnostic();
	}
}

void sendAbstractItemToThingSpeak(AbstractItem* item){
	String baseUrl=FPSTR(MESSAGE_THINGSPEAK_BASE_URL);
		baseUrl+=espSettingsBox.thSkWKey;
	String url=item->constructGetUrl(baseUrl, FPSTR(MESSAGE_THINGSPEAK_FIELD_VAL_FOR_REQUEST));
	wifiHelper.executeGetRequest(url);
}

String constructThingSpeakParameters(AbstractItem* item){
	return item->constructGetUrl("", FPSTR(MESSAGE_THINGSPEAK_FIELD_VAL_FOR_REQUEST));
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
