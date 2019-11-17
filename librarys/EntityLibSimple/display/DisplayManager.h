/*
 * DisplayManager.h
 *
 *  Created on: Oct 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYMANAGER_H_

#include "Arduino.h"
#include "ArduinoJson.h"
#include "functional"
#include "SettingsStorage.h"
#include "utils/WiFiUtils.h"
#include "display/PageToDisplayAdapter.h"
#include "display/DisplayPage.h"
#include "EntityJsonRequestResponse.h"
#include "DeviceConstants.h"

#define DISPLAY_MANAGER_STATUS_PAGE_INDEX 0
#define DISPLAY_MANAGER_STATUS_PAGE_HEADER "Balabas-soft"
#define DISPLAY_MANAGER_STATUS_PAGE_EMPTY_LINE ""

class DisplayManager {
public:
	DisplayManager(PageToDisplayAdapter* displayAdapter, DisplayPage** pages, unsigned char pageCount, SettingsStorage* conf);
	virtual ~DisplayManager(){}
	virtual void init();

	virtual void switchToStatusPageReturnToPrevious();
	virtual void switchToStatusPage(bool backToPrev);
	virtual void switchToNextPage() ;

	virtual void switchToNextNonStatusPage(){
		if(this->currentPage == this->pageCount) {
			this->currentPage = 0;
		}
		switchToNextPage();
		//Serial.println(FPSTR("next page"));
	}

	void setPageChangeEnabled(bool pageChangeEnabled = true);

	void loop();

	void switchToPage(int page) {
		this->currentPage = page;
		renderCurrentPage();
	}

	void powerOff(){
		displayAdapter->setPowerOn(0);
		Serial.println(FPSTR("Power OFF"));
	}

	void powerOn() {
		Serial.println(FPSTR("Power ON"));
		displayAdapter->setPowerOn(1);
	}

	bool isPowerOn(){
		return displayAdapter->isPowerOn();
	}

	void switchToNextPageWithInterval(unsigned long interval) {
		this->interval = interval;
		nextSwitchTime = millis() + interval;

		switchToNextNonStatusPage();
	}

	void switchToNextPageIfTriggered() {
		if(triggeredPageChange) {
			switchToNextPageOrTurnPowerOn();
			triggeredPageChange = false;
		}
	}

	void triggerPageChange() {
		triggeredPageChange = true;
		Serial.println(FPSTR("Triggered page change"));
	}

	void switchToNextPageOrTurnPowerOn() {
		if(displayAdapter->isPowerOn()) {
			displayAdapter->setPowerOn();
			switchToNextNonStatusPage();
		} else {
			displayAdapter->setPowerOn();
			renderCurrentPage();
		}
	}

	virtual void renderCurrentPage();

protected:
	unsigned long interval = 0;
	unsigned long nextSwitchTime = 0;


	virtual void renderStatusPage();
private:
	bool triggeredPageChange = false;

	PageToDisplayAdapter* displayAdapter;
	DisplayPage** pages;
	unsigned char pageCount = 0;
	SettingsStorage* conf;
	unsigned char currentPage = DISPLAY_MANAGER_STATUS_PAGE_INDEX;
	unsigned char previousPage = DISPLAY_MANAGER_STATUS_PAGE_INDEX;

	bool initDone = false;
	bool pageChangeEnabled = false;

};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYMANAGER_H_ */
