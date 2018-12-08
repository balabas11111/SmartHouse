#include "Arduino.h"
//The setup function is called once at startup of the sketch
#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])

#include "EspSettingsBox.h"
#include "ESP_Consts.h"
#include "I2Chelper.h"
#include "PinDigital.h"
#include "PinDigitalNoInt.h"
#include "DeviceHelper.h"
#include "BME280_Sensor.h"
#include "DS18D20_Sensor.h"
#include "DisplayHelperAbstract.h"
#include "WiFiHelper.h"
#include "HTTPClient.h"

EspSettingsBox espSettingsBox;
I2Chelper i2cHelper(SCL,SDA,false);
DisplayHelperAbstract displayHelper(&espSettingsBox);

HTTPClient http;
WebServer server(80);

WiFiHelper wifiHelper(&espSettingsBox, &displayHelper, &server,postInitWebServer,false);

PinDigitalNoInt acMeter(FPSTR(SENSOR_acMeter),25,onAcMeterChanged,INPUT,CHANGE,HIGH,HIGH);
PinDigitalNoInt acMeter2(FPSTR(SENSOR_acMeter2),26,onAcMeter2Changed,INPUT,CHANGE,HIGH,LOW);


PinDigitalNoInt buttonMenu(FPSTR(SENSOR_buttonMenu),33,onMenuButtonChanged);
PinDigital lampLeft(FPSTR(SENSOR_lampLeft),32,onLeftLampChanged,OUTPUT,CHANGE,acMeter.isOn(),LOW);
PinDigital lampRight(FPSTR(SENSOR_lampRight),13,onLeftLampChanged,OUTPUT,CHANGE,HIGH,LOW);


TimeTrigger sensorsTrigger(measureSensors);

BME280_Sensor bmeMeasurer(FPSTR(SENSOR_bmeMeasurer));
DS18D20_Sensor ds18d20Measurer(FPSTR(SENSOR_ds18d20Measurer), 19);
DS18D20_Sensor ds18d20Boiler(FPSTR(SENSOR_ds18d20Boiler), 23);

Loopable* loopArray[]={&wifiHelper,&sensorsTrigger,&buttonMenu,&acMeter};

AbstractItem* abstractItems[]={&lampLeft,&lampRight,&bmeMeasurer,&ds18d20Measurer,&ds18d20Boiler,&acMeter,&acMeter2};


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

	//attachInterrupts();
	espSettingsBox.init();
	i2cHelper.init();

	wifiHelper.init();

	bmeMeasurer.init();
	ds18d20Measurer.init();
	ds18d20Boiler.init();

	sensorsTrigger.start(espSettingsBox.refreshInterval*1000);

	Serial.println(FPSTR(MESSAGE_DEVICE_STARTED));

	measureSensors();

	deviceHelper.printDeviceDiagnostic();

	Serial.println(FPSTR("Device started3"));
}

// The loop function is called in an endless loop
void loop()
{
	deviceHelper.loop();
	measureAC();
}

void on_buttonMenuInterrupt(){buttonMenu.processInterrupt();}
void on_lampLeftInterrupt(){lampLeft.processInterrupt();}
void on_acMeterInterrupt(){acMeter.processInterrupt();}

void onMenuButtonChanged(){
	Serial.println("onMenuButtonChanged");
	//lampLeft.change();

}

void onLeftLampChanged(){
	Serial.println("onLeftLampChanged");
}

void onAcMeterChanged(){
	uint8_t val=acMeter.getValInt();

	Serial.println("AcMeter on="+String(acMeter.isOn()));
	//lampLeft.setVal(val);
}

void onAcMeter2Changed(){
	Serial.println("onAcMeterChanged="+String(acMeter.isOn()));
}

void measureSensors(){
	/*Serial.println(FPSTR("----Measure sensors----"));
	deviceHelper.update(abstractItems, ARRAY_SIZE(abstractItems));

	bmeMeasurer.print();
	ds18d20Measurer.print();
	*/
}

void measureAC(){
	lampLeft.turnOnOff(acMeter.isOn());
}

void postInitWebServer(){
	Serial.println(FPSTR("-------Start postInitWebServer-----"));

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

String getAllSensorsJson(){
	return deviceHelper.getJson(abstractItems, ARRAY_SIZE(abstractItems));
}
