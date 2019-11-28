/*
 * DisplayManager.cpp
 *
 *  Created on: Oct 8, 2019
 *      Author: Vitaliy_Vlasov
 */

#include <display/DisplayManager.h>

DisplayManager::DisplayManager(PageToDisplayAdapter* displayAdapter,
		DisplayPage** pages, unsigned char pageCount, SettingsStorage* conf) {
	this->displayAdapter = displayAdapter;
	this->pages = pages;
	this->pageCount = pageCount;
	this->conf = conf;
	this->currentPage = DISPLAY_MANAGER_STATUS_PAGE_INDEX;


}

void DisplayManager::init() {
	initDone = true;

	conf->setOnDeviceStatusChanged([this](){switchToStatusPageReturnToPrevious();});
	conf->setDeviceStatus((char*)DEVICE_STATUS_INITIALIZING);

	int initDoneCount = 1;

	for(int i = 1; i <= this->pageCount; i++){
		if((this->pages[i-1])->init(this->displayAdapter)){
			initDoneCount++;
		}
	}
	if(initDoneCount>1) {
		setPageChangeEnabled();
	}
	Serial.print(FPSTR("DisplayManager pages="));
	Serial.println(initDoneCount);


}

void DisplayManager::switchToStatusPage(bool backToPrev) {
	this->currentPage = DISPLAY_MANAGER_STATUS_PAGE_INDEX;

	displayAdapter->setPowerOn();
	renderStatusPage();

	if(backToPrev && this->currentPage != this->previousPage){
		this->currentPage = this->previousPage;
		renderCurrentPage();
	}
}

void DisplayManager::switchToNextPage() {
	if(this->pageChangeEnabled && this->initDone){
		this->previousPage = this->currentPage;

		if(this->currentPage == this->pageCount){
			switchToStatusPage(false);
			this->currentPage =0;
		} else {
			Serial.println(FPSTR("page incremented"));
			this->currentPage++;
			renderCurrentPage();
		}
	}
}

void DisplayManager::setPageChangeEnabled(bool pageChangeEnabled) {
	this->pageChangeEnabled = pageChangeEnabled;
}

void DisplayManager::renderCurrentPage() {
	if(!(this->pageChangeEnabled && this->initDone )){
		return;
	}
	DisplayPage* page = getCurrentPage();

	if(page!=nullptr) {
		Serial.print(FPSTR("Page->render() "));
		Serial.println(this->currentPage);
		page->render();
	} else{
		Serial.println(FPSTR("ERROR NO PAGE"));
	}
}

DisplayPage* DisplayManager::getCurrentPage() {
	int index = this->currentPage-1;
	if(index<0){
		 //this->currentPage = 1;
		 index = 0;
	}
	/*Serial.print(FPSTR("getCurrentPage()"));
	Serial.println(this->currentPage);
	*/
	return this->pages[index];
}

void DisplayManager::loop() {
	this->displayAdapter->loop();
}

void DisplayManager::renderStatusPage() {
	if(!this->initDone){
		return;
	}
	EntityJsonRequestResponse* resp = new EntityJsonRequestResponse();

	JsonArray& pageData = resp->getResponse().createNestedArray(DISPLAY_DATA);

	pageData.add(EMPTY_LINE);
	if(WiFiUtils::isConnected()){
		pageData.add(conf->getCurrentIp());
	}
	pageData.add(conf->getDeviceStatus());

	this->displayAdapter->renderPage(DISPLAY_MANAGER_STATUS_PAGE_HEADER, pageData);

	delete resp;
}

void DisplayManager::switchToStatusPageReturnToPrevious() {
	switchToStatusPage(true);
}

