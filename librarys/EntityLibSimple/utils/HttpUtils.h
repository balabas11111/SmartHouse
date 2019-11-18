/*
 * HttpUtils.h
 *
 *  Created on: 5 ����. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_UTILS_HTTPUTILS_H_
#define LIBRARIES_ENTITYLIBSIMPLE_UTILS_HTTPUTILS_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#endif

#include <EntityJsonRequestResponse.h>
#include <utils/JsonObjectUtil.h>
#include <WiFi/HttpConstants.h>

#define EMPTY_STR " "
#define QUESTION_STR "?"
#define EQUAL_STR "="
#define AND_STR "&"

#define URL_GET "GET "
#define URL_GET_JSON "GET JSON "

#define URL_POST "POST "
#define URL_POST_JSON "POST JSON "

#define PARAM_URL "url"

class HttpUtils {
public:
	HttpUtils(){}
	virtual ~HttpUtils() {
	}

	static void addAsFormParameter(String& paramName, String& paramValue, String& params){
		bool first = params.length()==0;

		if(first){
			params += QUESTION_STR;
		} else{
			params += AND_STR;
		}

		params += paramName;
		params += EQUAL_STR;
		params += paramValue;
	}

	static int executeGetRequest(String& url, String& params, String& result) {

		int httpCode = executeGetRequestBase(URL_GET, url, params, result);
		printStrResponse(result);

		return httpCode;
	}

	static int executeGetRequest(String& url, String& params, JsonObject& result) {

		String resultStr;

		int httpCode = executeGetRequestBase(URL_GET_JSON, url, params, resultStr);

		printJsonResponse(resultStr, result);

		return httpCode;
	}

	static int executeGetRequest(String& url, JsonObject& params, String& result) {

		String paramsStr;

		jsonToStrAsParams(params, paramsStr);

		int httpCode = executeGetRequestBase(URL_GET_JSON, url, paramsStr, result);

		printStrResponse(result);

		return httpCode;
	}

	static int executeGetRequest(String& url, JsonObject& params,
			JsonObject& result) {

		String paramsStr;
		String resultStr;

		jsonToStrAsParams(params, paramsStr);

		int httpCode = executeGetRequestBase(URL_GET_JSON, url, paramsStr, resultStr);

		printJsonResponse( resultStr, result);

		return httpCode;
	}

	static int executeGetRequest(EntityJsonRequestResponse* req, const char* urlParamName) {
		String url = req->getRequest().get<char*>(urlParamName);
		req->getRequest().remove(urlParamName);

		return executeGetRequest(url,req->getRequest(),req->getResponse());
	}

	static int executePostRequest(String& url, String& params,
			String& result) {

		int httpCode = executePostRequestBase(URL_POST, url, params, result, (char*)CONTENT_TYPE_APPLICATION_FORM_URL_ENCODED);
		printStrResponse( result);

		return httpCode;
	}

	static int executePostRequest(String& url, String& params,
			JsonObject& result) {
		String resultStr;

		int httpCode = executePostRequestBase(URL_POST, url, params, resultStr, (char*)CONTENT_TYPE_APPLICATION_FORM_URL_ENCODED);

		printJsonResponse(resultStr, result);

		return httpCode;
	}

	static int executePostRequest(String& url, JsonObject& params,
			String& result) {

		String paramsStr;
		params.printTo(paramsStr);

		int httpCode = executePostRequestBase(URL_POST_JSON, url, paramsStr, result, (char*)CONTENT_TYPE_APPLICATION_JSON);

		//printStrResponse(result);

		return httpCode;
	}

	static int executePostRequest(String& url, JsonObject& params,
			JsonObject& result) {

		String paramsStr;
		String resultStr;

		params.printTo(paramsStr);

		int httpCode = executePostRequestBase(URL_POST_JSON, url, paramsStr, resultStr,	(char*)CONTENT_TYPE_APPLICATION_JSON);

		printJsonResponse(resultStr, result);

		return httpCode;
	}

	static int executePostRequest(EntityJsonRequestResponse* req, const char* urlParamName) {
		String url = req->getRequest().get<char*>(urlParamName);
		req->getRequest().remove(urlParamName);

		return executePostRequest(url,req->getRequest(),req->getResponse());
	}

private:
	static void jsonToStrAsParams(JsonObject& params, String& paramsStr, bool isGet = true) {
		/*if(*params ==nullptr){
			return;
		}*/
		int index = 0;

		for (const auto& kvp : params) {
			if (index != 0) {
				paramsStr += AND_STR;
			}

			paramsStr += kvp.key;
			paramsStr += EQUAL_STR;
			paramsStr += kvp.value.as<const char*>();

			index++;
		}

		if(isGet && index>0){
			paramsStr = QUESTION_STR +paramsStr;
		}
	}
	static int executePostRequestBase(const char* reqType,String& url, String& params,
			String& result, char* contentType) {
		UNUSED(reqType);
		if(WiFi.status()!= WL_CONNECTED){
			return -1;
		}

		/*Serial.println(FPSTR("--------------------"));
		printReqDetails(reqType, url, params);*/
		Serial.println(url);

		delay(0);

		HTTPClient http;

		http.begin(url);

		http.addHeader(HEADER_CONTENT_TYPE, contentType);

		int httpCode = http.POST(params);
		result = http.getString();

		printHttpCode(httpCode);
		printStrResponse(result);
		http.end();

		return httpCode;
	}
	static int executeGetRequestBase(const char* reqType, String& url, String& params,
			String& result) {
		if(WiFi.status()!= WL_CONNECTED){
			return -1;
		}
		printReqDetails(reqType, url, params);

		String paramsStr = (params != nullptr && params.length() > 1) ?	params : "";

		Serial.print(FPSTR("process url ="));
		Serial.print(url);
		Serial.print(FPSTR(" params ="));
		Serial.println(paramsStr);

		String getUrl = String(url) +paramsStr;

		Serial.print(FPSTR("process Get url ="));
		Serial.println(getUrl);

		delay(0);

		HTTPClient http;
		http.begin(getUrl);

		int httpCode = http.GET();
		result = http.getString();

		printHttpCode(httpCode);

		http.end();

		return httpCode;
	}
	static void printReqDetails(const char* reqType, String& url) {
		Serial.print(reqType);
		Serial.print(FPSTR(" url= "));
		Serial.println(url);
	}
	static void printReqDetails(const char* reqType, String& url,
			String& params) {
		printReqDetails(reqType, url);
		Serial.print(FPSTR("paramsStr = "));
		Serial.println(params);
	}

	static void printReqDetails(const char* reqType, String& url,
			JsonObject& params) {
		printReqDetails(reqType, url);
		JsonObjectUtil::print(params);
	}

	static void printHttpCode(int httpCode){
		Serial.print(FPSTR("HttpCode = "));
		Serial.println(httpCode);
	}

	static void printStrResponse(String& result) {
		Serial.print(FPSTR("resultStr = "));
		Serial.println(result);
	}

	static void printJsonResponse(String& resultStr,
			JsonObject& result) {
		//printStrResponse(resultStr);
		JsonObjectUtil::clone(result, resultStr);
		JsonObjectUtil::print("Result JSON ", result);
	}
};

#endif /* LIBRARIES_UTILS_HTTPUTILS_H_ */
