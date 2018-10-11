#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "balabasKiev5"; // Имя роутера
const char *pass = "wuWylKegayg2wu22"; // Пароль роутера

const char *mqtt_server = "m23.cloudmqtt.com"; // Имя сервера MQTT
const int mqtt_port = 10186; // Порт для подключения к серверу MQTT
const char *mqtt_user = "tpheglmk"; // Логи для подключения к серверу MQTT
const char *mqtt_pass = "QgYRfVzDhQ31"; // Пароль для подключения к серверу MQTT

const char *topic="topic/test1";
const char *topic_sub="topic/test21";

const int led = 5; // диод на плате

#define BUFFER_SIZE 100

int tm = 300;
float temp = 0;

WiFiClient wclient;
PubSubClient client(mqtt_server, mqtt_port, callback, wclient);

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("Starting mqtt server");
  Serial.println();

  //pinMode(led, OUTPUT);
}

// Функция отправки показаний
void sendData() {
	static int counter = 0;

	  String payload = "{\"micros\":";
	  payload += micros();
	  payload += ",\"counter\":";
	  payload += counter;
	  payload += "}";

	  if (client.connected()){
	    Serial.print("Sending payload: ");
	    Serial.println(payload);

	    if (client.publish(topic, (char*) payload.c_str())) {
	      Serial.println("Publish ok");
	    }
	    else {
	      Serial.println("Publish failed");
	    }
	  }
	  ++counter;
	  delay(5000);

  delay(1);
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
  // проверяем из нужного ли нам топика пришли данные
  if(String(topic).equals(String(topic_sub)))
  {
     Serial.println("received from subscribe queue"); // выводим в сериал порт подтверждение, что мы получили топик test/2
  }
}

void loop() {
  // подключаемся к wi-fi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) return;
    Serial.println("WiFi connected");
  }

  // подключаемся к MQTT серверу
  if (WiFi.status() == WL_CONNECTED) {

	connectMqtt();
	processMqtt();

  }

  delay(5000);
}

void processMqtt(){
	if (client.connected()){
	  client.loop();
	  //sendData();
	}
}

void connectMqtt(){
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
	    	    Serial.print("Publish Topic is: ");
	    	    Serial.println(topic);

	    	    if (client.publish(topic, "hello from ESP8266")) {
	    	      Serial.println("Publish ok");
	    	    }
	    	    else {
	    	      Serial.println("Publish failed");
	    	    }

	    	    subscribeToTopic(topic);
	    	    subscribeToTopic(topic_sub);
	    	  }
	    	  else {
	    	    Serial.println("MQTT connect failed");
	    	    Serial.println("Will reset and try again...");
	    	    delay(5000);
	    	    //abort();
	    	  }
	    }
}

void subscribeToTopic(const char *topicIn){
	Serial.print("Subscribing to "+String(topicIn));
	client.subscribe(topicIn);
	Serial.println("   ...Subscribed");
}

