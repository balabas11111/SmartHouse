#include "Arduino.h"
//The setup function is called once at startup of the sketch
#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])

#include "EspSettingsBox.h"
#include "ESP_Consts.h"
#include "I2Chelper.h"
#include "PinDigital.h"
#include "DeviceHelper.h"
#include "BME280_Sensor.h"
#include "DS18D20_Sensor.h"
#include "DisplayHelper.h"
#include "WiFiHelper.h"
#include "HTTPClient.h"

EspSettingsBox espSettingsBox;
I2Chelper i2cHelper(SCL,SDA,false);
DisplayHelper displayHelper(&espSettingsBox);

HTTPClient http;
WebServer server(80);

WiFiHelper wifiHelper(&espSettingsBox, &displayHelper, &server,postInitWebServer,false);

PinDigital buttonMenu(FPSTR(SENSOR_buttonMenu),33,onMenuButtonChanged);
PinDigital lampLeft(FPSTR(SENSOR_lampLeft),32,onLeftLampChanged,OUTPUT,CHANGE,HIGH,LOW);
PinDigital acMeter(FPSTR(SENSOR_acMeter),34,onAcMeterChanged,INPUT,CHANGE,HIGH,HIGH);

TimeTrigger sensorsTrigger(measureSensors);

BME280_Sensor bmeMeasurer(FPSTR(SENSOR_bmeMeasurer));
DS18D20_Sensor ds18d20Measurer(FPSTR(SENSOR_ds18d20Measurer), 0);

Loopable* loopArray[]={&wifiHelper,&sensorsTrigger,&buttonMenu,&acMeter};

AbstractItem* abstractItems[]={&lampLeft,&bmeMeasurer,&ds18d20Measurer,&acMeter};


DeviceHelper deviceHelper(loopArray,ARRAY_SIZE(loopArray),120000);

void attachInterrupts(){
	Serial.println(FPSTR("-----AttachInterrupts----"));

	attachInterrupt(buttonMenu.getPin(), on_buttonMenuInterrupt, CHANGE);
	attachInterrupt(lampLeft.getPin(), on_lampLeftInterrupt, CHANGE);
	attachInterrupt(acMeter.getPin(), on_acMeterInterrupt, CHANGE);
}

void setup()
{
	Serial.begin(115200);

	deviceHelper.startDevice(espSettingsBox.DeviceId);

	attachInterrupts();
	espSettingsBox.init();
	i2cHelper.init();

	wifiHelper.init();

	bmeMeasurer.init();
	ds18d20Measurer.init();
	sensorsTrigger.start(espSettingsBox.refreshInterval*1000);

	Serial.println(FPSTR(MESSAGE_DEVICE_STARTED));

	measureSensors();
}

// The loop function is called in an endless loop
void loop()
{
	deviceHelper.loop();
}

void on_buttonMenuInterrupt(){buttonMenu.processInterrupt();}
void on_lampLeftInterrupt(){lampLeft.processInterrupt();}
void on_acMeterInterrupt(){acMeter.processInterrupt();}

void onMenuButtonChanged(){
	Serial.println("onMenuButtonChanged");
	lampLeft.change();
}

void onLeftLampChanged(){
	Serial.println("onLeftLampChanged");
}

void onAcMeterChanged(){
	Serial.println("onAcMeterChanged="+String(acMeter.isOn()));
}

void measureSensors(){
	/*Serial.println(FPSTR("----Measure sensors----"));
	deviceHelper.update(abstractItems, ARRAY_SIZE(abstractItems));

	bmeMeasurer.print();
	ds18d20Measurer.print();
	*/
}

void postInitWebServer(){
	Serial.println(FPSTR("-------Start postInitWebServer-----"));

	server.on(FPSTR(URL_GET_SENSORS_CURRNT_VALUES), HTTP_GET, [](){
		if(server.hasArg(FPSTR(MESSAGE_SERVER_ARG_SENSOR))){
			String arg=server.arg(FPSTR(MESSAGE_SERVER_ARG_SENSOR));

			if(arg==FPSTR(MESSAGE_SERVER_ARG_VAL_ALL)){
				wifiHelper.checkAuthentication();
				server.send(200, FPSTR(CONTENT_TYPE_JSON_UTF8),
							deviceHelper.getJson(abstractItems, ARRAY_SIZE(abstractItems)));
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

	Serial.println(FPSTR("-------ENd postInitWebServer-----"));
}
