#include "Arduino.h"
#include "TimeTrigger.h"

//SHT21 import
#include <Wire.h>
#include "HTU21D.h"

#define D2_PIN 4  //GPIO04  //OLED //SDA //blue
#define SDA 4  //GPIO04  //OLED //SDA //blue
#define D3_PIN 0  //GPIO00  //OLED //SCL //green
#define SCL 0  //GPIO00  //OLED //SCL //green

TimeTrigger sensorTrigger(0,10000,true);

HTU21D mySensor;

float SHT21_temperature;
float SHT21_humidity;



void setup()
{
	Serial.begin(115200);
	init_SHT21_Sensor();
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	if(sensorTrigger.checkTrigger()){

		check_STH21_Sensor();
		display_STH21_SensorData();
		publish_STH21_SensorData();

		sensorTrigger.saveTime();
	}

}

void init_SHT21_Sensor(){
	mySensor.begin(SDA,SCL);
	Serial.println("SHT_21_started");
}

void check_STH21_Sensor(){
	Serial.println("StartCheck_SHT21");
	SHT21_humidity=mySensor.readHumidity();
	SHT21_temperature=mySensor.readTemperature();
	Serial.println("CompletedCheck_SHT21");
	Serial.println("--------------------");
}

void display_STH21_SensorData(){
	Serial.println("SHT21_Humidity = "+String(SHT21_humidity));
	Serial.println("SHT21_temperature = "+String(SHT21_temperature));
}

void publish_STH21_SensorData(){
	Serial.println("Publish humidity");
	Serial.println(mySensor.readHumidity());
	Serial.println(mySensor.readTemperature());
}
