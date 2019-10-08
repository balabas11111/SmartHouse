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
#include "WiFi/WiFiSettingsBox.h"
#include "WiFiUtils.h"
#include "display/PageToDisplayAdapter.h"
#include "display/DisplayPage.h"
#include "EntityJsonRequestResponse.h"

#define DISPLAY_MANAGER_STATUS_PAGE_INDEX 0
#define DISPLAY_MANAGER_STATUS_PAGE_HEADER "Balabas-soft"
#define DISPLAY_MANAGER_STATUS_PAGE_EMPTY_LINE ""

class DisplayManager {
public:
	DisplayManager(PageToDisplayAdapter* displayAdapter, DisplayPage* pages[], unsigned char pageCount, WiFiSettingsBox* conf);
	virtual ~DisplayManager(){};
	virtual void init();

	virtual void switchToStatusPageReturnToPrevious();
	virtual void switchToStatusPage(bool backToPrev);
	virtual void switchToNextPage();

	void setPageChangeEnabled(bool pageChangeEnabled = true);

	void loop();
protected:
	virtual void renderCurrentPage();
	virtual void renderStatusPage();
private:
	PageToDisplayAdapter* displayAdapter;
	DisplayPage** pages;
	unsigned char pageCount = 0;
	WiFiSettingsBox* conf;
	unsigned char currentPage = DISPLAY_MANAGER_STATUS_PAGE_INDEX;
	unsigned char previousPage = DISPLAY_MANAGER_STATUS_PAGE_INDEX;

	bool initDone = false;
	bool pageChangeEnabled = false;
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_DISPLAY_DISPLAYMANAGER_H_ */
