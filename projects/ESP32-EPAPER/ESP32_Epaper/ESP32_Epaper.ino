    /////////////////////////////////////////////////////////////////
   //             ESP32 E-PAPER THERMOMETER             v1.00     //
  //       Get the latest version of the code here:              //
 //        http://educ8s.tv/esp32-e-paper-thermometer           //
/////////////////////////////////////////////////////////////////

#include <GxEPD.h>
#include <GxGDEP015OC1/GxGDEP015OC1.cpp>
#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "BitmapGraphics.h"

#include <Fonts/FreeSansBold24pt7b.h>

#define ONE_WIRE_BUS 15

float tempC = 0;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

GxIO_Class io(SPI, SS, 22, 21);
GxEPD_Class display(io, 16, 4);

void setup() {
  
  Serial.begin(9600);
  display.init();
  sensors.begin();
  
  display.drawExampleBitmap(gImage_splash, 0, 0, 200, 200, GxEPD_BLACK);
  display.update();
  delay(3000);
  
  display.drawExampleBitmap(gImage_gui, 0, 0, 200, 200, GxEPD_BLACK);
  display.update();

  display.drawExampleBitmap(gImage_gui, sizeof(gImage_gui), GxEPD::bm_default | GxEPD::bm_partial_update);
}

void loop() {
  
  sensors.requestTemperatures();
  tempC = sensors.getTempCByIndex(0);
  
  showPartialUpdate(tempC);
  printTemperatureToSerial();
  
  delay(5000);
}

void showPartialUpdate(float temperature)
{
  String temperatureString = String(temperature,1);
  const char* name = "FreeSansBold24pt7b";
  const GFXfont* f = &FreeSansBold24pt7b;
  
  uint16_t box_x = 60;
  uint16_t box_y = 60;
  uint16_t box_w = 90;
  uint16_t box_h = 100;
  uint16_t cursor_y = box_y + 16;

  display.setRotation(45);
  display.setFont(f);
  display.setTextColor(GxEPD_BLACK);

  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(box_x, cursor_y+38);
  display.print(temperatureString); 
  display.updateWindow(box_x, box_y, box_w, box_h, true);
}

void printTemperatureToSerial()
{
  Serial.print(tempC);
  Serial.print(" C");
  Serial.print("\t");
}
