/*
 * DisplayToOledAdapter.h
 *
 *  Created on: 16 нояб. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYTOOLEDADAPTER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYTOOLEDADAPTER_H_

#include "PageToDisplayAdapter.h"
#include <iarduino_OLED_txt.h>

extern uint8_t SmallFontRus[];

class DisplayToOledAdapter: public PageToDisplayAdapter {
public:
	DisplayToOledAdapter(){};
	virtual ~DisplayToOledAdapter(){};

	virtual void setPowerOn(bool powerOn = true) {
		PageToDisplayAdapter::setPowerOn(powerOn);
	}

	virtual bool init() override{
		Serial.println("DisplayToOledAdapter");
		myOLED = new iarduino_OLED_txt(0x78);
		myOLED->begin();                                    // Инициируем работу с дисплеем.
		myOLED->setFont(SmallFontRus);                      // Указываем шрифт который требуется использовать для вывода цифр и текста.
		myOLED->setCoding(TXT_UTF8);

		myOLED->print("Hello !!SmartHouse", 3,3);

		setPowerOn(true);
		Serial.println("DisplayToOledAdapter - done");
		return true;
	};

	virtual void clear() override{
		myOLED->clrScr();
	};

	virtual void toStartPosition() override{
		//Serial.println(FPSTR("============"));
		yPosition = 0;
	}

	virtual void print(const char* str) override{
		if(str!=nullptr) {
			//Serial.print(str);
			myOLED->print(str, 0, yPosition);
		}
	};
	virtual void printNextLine() override{
		yPosition ++;
	};
private:
	iarduino_OLED_txt* myOLED = nullptr;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYTOOLEDADAPTER_H_ */
