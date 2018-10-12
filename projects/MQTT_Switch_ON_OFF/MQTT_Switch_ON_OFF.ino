#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "EspSettingsBox.h"
//#include "MqttHelper.h"
//#include "WiFiHelper.h"
#include "AbstractEvent.h"

#include <ESP8266WebServer.h>
#include "PinDigital.h"
#include "PinEvent.h"
#include "TimeTrigger.h"

#include "BH1750.h"

#define FIRMVARE_VERSION "b@l@bas-soft SENSORZ v0.0.5"
//Robot Dyn//vem  // 12E
#define D0_PIN 16 //GPIO016 ////beeper
#define D1_PIN 5  //GPIO05  //DallasTemp
#define D2_PIN 4  //GPIO04  //OLED //SDA //blue
#define D3_PIN 0  //GPIO00  //OLED //SCL //green
#define D4_PIN 2  //GPIO02  //RedLed
#define D5_PIN 14 //GPIO14  //DHT
#define D6_PIN 12 //GPIO12  //DallasTemp red led2
#define D7_PIN 13 //GPIO13  //GreenLed
#define D8_PIN 15 //GPIO15  //button
#define RX_PIN 3
#define TX_PIN 1

#define SD3_PIN 10
#define SD2_PIN 9

const char *ssid = "balabasKiev5"; // Имя роутера
const char *pass = "wuWylKegayg2wu22"; // Пароль роутера

const char *mqtt_server = "m23.cloudmqtt.com"; // Имя сервера MQTT
const int mqtt_port = 10186; // Порт для подключения к серверу MQTT
const char *mqtt_user = "tpheglmk"; // Логи для подключения к серверу MQTT
const char *mqtt_pass = "QgYRfVzDhQ31"; // Пароль для подключения к серверу MQTT

const char *baseTopic="topic/";
const char *topic_sub_s1="topic/switch1";
const char *topic_sub_s2="topic/switch2";

const char *topicBasePath="topic";

#define BUFFER_SIZE 100

WiFiClient wclient;
ESP8266WebServer server ( 80 );
PubSubClient client(mqtt_server, mqtt_port, callback, wclient);

EspSettingsBox espSettingsBox("/values.txt","",true);
/*
WiFiHelper wiFiHelper(&espSettingsBox,&clearDisplay,displayLineFunction,&ledOnFunction,&ledOffFunction,&setupServerUrlsFunction);

MqttHelper mqttHelper(&espSettingsBox,processEvent,wclient);
*/
void clearDisplay(){};
void displayLineFunction(String message,int row,int column){};
void ledOnFunction(){};
void ledOffFunction(){};
void setupServerUrlsFunction(ESP8266WebServer server){};

TimeTrigger lmTrigger(0,1000,true,measureLm);

PinDigital buttonLeft("SwitchLeft",D7_PIN,processEvent);
PinDigital buttonRight("SwitchRight",D6_PIN,processEvent);

PinDigital lampLeft("LampLeft",OUTPUT,D5_PIN,processEvent,CHANGE,HIGH);
PinDigital lampRight("LampRight",OUTPUT,D8_PIN,processEvent,CHANGE,HIGH);

PinDigital IrDetector("IrDetector",D3_PIN,processEvent);

BH1750 lightMeter;

PinEvent processEvent(PinEvent event){
	//PIN_SC:01:001:onOffEvent:Button1
	/*
	  //test Urls
		http://192.168.0.178/runCommand?command=PIN_SC:14:1:Switch1:dispatchedByHtml
		http://192.168.0.178/runCommand?command=PIN_SC:14:0:Switch1:dispatchedByHtml
		http://192.168.0.178/runCommand?command=PIN_SC:15:1:Switch1:dispatchedByHtml
		http://192.168.0.178/runCommand?command=PIN_SC:15:0:Switch1:dispatchedByHtml
		http://192.168.0.178/runCommand?command=PIN_SC:15:0:Switch1:dispatchedByHtml

		http://192.168.0.178/runCommand?command=GET_ANALOG:17:0:lightLevSens:extCommand
		http://192.168.0.178/runCommand?command=GET_PIR_STATE:0:0:IrDetector:extCommand
		http://192.168.0.178/runCommand?command=PIN_GET_STATE:14:0:IrDetector:extCommand
		http://192.168.0.178/runCommand?command=GET_PIR_STATE:15:0:IrDetector:extCommand

	*/

	Serial.println("ProcessCommand "+event.getText());
	PinEvent newEvent=PinEvent();

	if(!event.isValid()){
		return newEvent;
	}

	if(event.isEventOfKind(PIN_EVENT_STATE_GET)){
		if(!newEvent.isValid())
			newEvent=lampLeft.processEventNow(event);
		if(!newEvent.isValid())
			newEvent=lampRight.processEventNow(event);
		if(!newEvent.isValid())
			newEvent=buttonLeft.processEventNow(event);
		if(!newEvent.isValid())
			newEvent=buttonRight.processEventNow(event);
	}

	if(event.isEventOfKind(PIN_EVENT_STATE_SET)){
		if(!newEvent.isValid())
			newEvent=lampLeft.processEventNow(event);
		if(!newEvent.isValid())
			newEvent=lampRight.processEventNow(event);

		newEvent.setIsBubble(false);
	}

	if(event.isEventOfKind(PIN_EVENT_STATE_UPDATED)){
		if(buttonLeft.isDispatcherOfEvent(event)){
			newEvent=lampLeft.constructPinEventSetState(event);
		}
		if(buttonRight.isDispatcherOfEvent(event)){
			newEvent=lampRight.constructPinEventSetState(event);
		}
		if(IrDetector.isDispatcherOfEvent(event)){
			Serial.println("ir change event processed");
		}
	}

	if(newEvent.isValid() && newEvent.isBubble())
		sendEventToTopic(newEvent);

	return newEvent;
}

void sendEventToTopic(PinEvent event){

	if(event.isValid()){
		String topic=String(baseTopic)+event.getDispatcherName();
		String msg=event.getText();

		publishToTopic((char*) topic.c_str(),msg);
	}
}

void setup() {
  Serial.begin(115200);
  delay(10);

  lightMeter.begin();

  setupWiFi();
  setupServer();

  Serial.println("Starting mqtt server");
  Serial.println();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
	connectMqttIfNotConnected();
	processMqtt();
  }

 buttonLeft.loop();
 buttonRight.loop();
 IrDetector.loop();
 lmTrigger.loop();
 server.handleClient();
}

void measureLm(){
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
}


void sendData() {
	static int counter = 0;

	  String payload = "{\"micros\":";
	  payload += micros();
	  payload += ",\"counter\":";
	  payload += counter;
	  payload += "}";

	  publishToTopic(baseTopic,payload);

	  ++counter;
	  delay(5000);

  delay(1);
}

boolean publishToTopic(const char *topic, String payload){

	boolean result=false;

	if (client.connected()){
		Serial.print("Sending payload: ");
		Serial.print(payload);

		result=client.publish(topic, (char*) payload.c_str());

		if (result) {
		  Serial.println("...ok");
		}
		else {
		  Serial.println("...failed");
		}
    }

	return result;
}

// Функция получения данных от сервера
void callback(char* topic, uint8_t* payload, unsigned int length) {

	String payloadIn = (char*)payload;
	String topicIn=String(topic);

	String msg;
		for(int i=0;i<length;i++){
			msg+=(char)payload[i];
		}

	String messageIn="Message received payload="+msg+" topic="+topic +" length="+length;

	Serial.println(messageIn);
	PinEvent result=processEvent(PinEvent(msg));
}

void processHttpCommand(){
	if(server.args()!=0 && server.hasArg("command")){
		String command=server.arg("command");

		//AbstractEvent event=AbstractEvent(command);
		PinEvent result=processEvent(PinEvent(command));

		delay(1);
		server.send ( 200, "text/html", "{\"status\":\"Ok\",\"command\":"+command+",\"result\":"+result.getText()+"}" );
	}

	server.send(500,"text/html","{\"status\":\"Failed\",\"command\":\"No command received\"}");
}

void processMqtt(){
	if (client.connected()){
	  client.loop();
	  //sendData();
	}
}

boolean isMqttConnected(){
	return client.connected();
}

void connectMqttIfNotConnected(){
	String clState=String(client.state());
	String clConnected=String(client.connected());
	String wifiState=String(wclient.status());
	String wifiConnected=String(wclient.connected());

	//Serial.println("Client State="+clState+" clConnected="+clConnected+" wifiState="+wifiState+" wifiConnected"+wifiConnected);

	if (!client.connected()) {

	    	  String clientName;
	    	  clientName += "esp8266 "+millis();

	    	if (client.connect((char*) clientName.c_str(),mqtt_user,mqtt_pass)) {
	    	    Serial.println("Connected to MQTT broker");
	    	    Serial.print("Base Publish Topic is: ");
	    	    Serial.println(baseTopic);

	    	    if (client.publish(baseTopic, "hello from ESP8266")) {
	    	      Serial.println("Publish ok");
	    	    }
	    	    else {
	    	      Serial.println("Publish failed");
	    	    }

	    	    subscribeToTopic(topic_sub_s1);
	    	    subscribeToTopic(topic_sub_s2);
	    	  }
	    	  else {
	    	    Serial.println("MQTT connect failed");
	    	    Serial.println("Will reset and try again...");
	    	    delay(5000);
	    	    //abort();
	    	  }

	    	Serial.println("IP="+WiFi.localIP().toString());
	    }
}

void subscribeToTopic(const char *topicIn){
	Serial.print("Subscribing to "+String(topicIn));
	client.subscribe(topicIn);
	Serial.println("   ...Subscribed");
}

void setupWiFi(){
	Serial.println("-----Setup WiFi-----");

	if(isWiFIConnected()){
		Serial.println("Disconnecting wifi");
		WiFi.disconnect(0);
	}

	WiFi.mode(WIFI_STA);
	WiFi.config(espSettingsBox.localIp,espSettingsBox.gateIp,
			espSettingsBox.subnetIp,espSettingsBox.dnsIp,espSettingsBox.dnsIp2);
	WiFi.begin ( const_cast<char*>(espSettingsBox.ssid.c_str()),
			const_cast<char*>(espSettingsBox.password.c_str()) );

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( WiFi.SSID() );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

}

void connectWifiIfNotConnected(){

	Serial.println("connectWifiIfNotConnected");

	while(!isWiFIConnected()){
		connectWiFi();

		if(!isWiFIConnected()){
			delay(5000);
		}
	}
}

boolean isWiFIConnected(){
	return WiFi.status() == WL_CONNECTED;
}

void connectWiFi(){
	if (!isWiFIConnected()) {
	    Serial.print("Connecting to ");
	    Serial.print(ssid);
	    Serial.println("...");
	    WiFi.begin(ssid, pass);

	    if (WiFi.waitForConnectResult() != WL_CONNECTED) return;
	    Serial.println("WiFi connected");

		server.on ( "/runCommand", HTTP_GET, processHttpCommand );
		server.begin();

		Serial.println(WiFi.localIP());
	  }
}

void setupServer(){
	Serial.println("SetupServer");

	server.on ( "/runCommand", HTTP_GET, processHttpCommand );
	server.begin();

	Serial.println(WiFi.localIP());
}
