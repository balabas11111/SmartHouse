/*
 * RequestManager.cpp
 *
 *  Created on: 8 февр. 2019 г.
 *      Author: Vitaliy
 */

#include <EntityLib/RequestManager.h>
#include <EntityLib/Url_Registerer.h>

RequestManager::RequestManager(Url_Registerer* registerer) {
	registerer->on(FPSTR(ONE_PARAM_URL_GET), HTTP_GET, [this](){
		//executeJsonGet(registerer->getParameter("data"));
	});
	registerer->on(FPSTR(ONE_PARAM_URL_POST), HTTP_POST, [this](){
		//executeJsonPost(registerer->getParameter("data"));
	});
}

RequestManager::~RequestManager() {
	// TODO Auto-generated destructor stub
}

uint8_t RequestManager::registerPaths() {
}

uint16_t RequestManager::registerExecutors(RequestExecutor* executors,
		uint8_t count) {
}

Request RequestManager::executeJson(String target, String page, String json) {
}

Request RequestManager::executeJsonGet(String getText) {
}

Request RequestManager::executeJsonPost(String postText) {
}

RequestExecutor* RequestManager::getExecutors() {
}

uint8_t RequestManager::getExecutorCount() {
}

Request RequestManager::executeRequest(Request* request) {
}

Request RequestManager::executeStringToThreeParams(String executeText) {
}
