/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_WIFIHELPER_H_
#define LIBRARIES_HELPERS_WIFIHELPER_H_


#include <ESP_Consts.h>
#include "Arduino.h"
#include "interfaces/Loopable.h"
#include "interfaces/Initializable.h"
#include "EspSettingsBox.h"
#include "EspSettingsUtil.h"
#include "FS.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include "ESP8266WebServer.h"
#include <ESP8266HTTPClient.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <SPIFFS.h>
#endif

#include <PinDigital.h>
#include <Display_Pageable/DisplayHelper.h>

#include "extraBoxes/EspSett_Own.h"

#define NOT_CONNECT_ITERATIONS 20
#define RE_CONNECT_DELAY 250

class WiFiHelper:public Initializable,public Loopable {

public:
	#ifdef ESP8266
	WiFiHelper(EspSettingsBox *_settingsBox,std::function<void(void)> _serverPostInitFunc,
				int port=80,boolean _disconnectOnStartIfConnected=true,boolean otaEnabled=true){

		serv=new ESP8266WebServer(port);

		if(otaEnabled){
			updater=new ESP8266HTTPUpdateServer(true);
		}

		construct(_settingsBox, nullptr,nullptr,_serverPostInitFunc,_disconnectOnStartIfConnected);
	}
	#endif
	#ifdef ESP32
	WiFiHelper(EspSettingsBox *_settingsBox,DisplayHelper *_displayHelper,
			WebServer *_server,std::function<void(void)> _serverInitEventFunc,std::function<void(void)> _serverPostInitFunc,boolean _disconnectOnStartIfConnected){

		server=_server;
		construct(_settingsBox, _displayHelper,_serverInitEventFunc,_serverPostInitFunc,_disconnectOnStartIfConnected);
	}
	#endif


	void construct(EspSettingsBox *_settingsBox, DisplayHelper *_displayHelper,
			std::function<void(void)> _serverInitEventFunc,std::function<void(void)> _serverPostInitFunc,
			boolean _disconnectOnStartIfConnected){

		this->espSettingsBox=_settingsBox;
		this->displayHelper=_displayHelper;

		this->serverInitEventFunc=_serverInitEventFunc;
		this->serverPostInitFunc=_serverPostInitFunc;

		this->disconnectOnStartIfConnected=_disconnectOnStartIfConnected;

		this->initStaticPages=false;
		this->initFilesManager=true;

		this->initialized=false;
		this->lastConnected=millis();
		this->oldstatus=WiFi.status();
	}

	boolean initialize(boolean _init){
		if(_init){
			Serial.println(FPSTR(MESSAGE_WIFIHELPER_INIT_CONNECTION));

			displayDetails();
			if(WiFi.status()==WL_CONNECTED && disconnectOnStartIfConnected){
				Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_DISCONNECTING));
#ifdef ESP8266
					WiFi.disconnect(1);
#endif
#ifdef ESP32
					WiFi.disconnect();
#endif
			}

			if(espSettingsBox->getExtraValueBoolean(ExtraBox_Own, OWN_isAccesPoint)){
				startAsAccessPoint();
			}else{
				startAsClient();
			}

			setupServer();
			initUpdater();

		}
		initialized=_init;
		return true;
	}

	boolean loop(){
		if(initialized){
			connectToWiFiIfNotConnected();
			serv->handleClient();
		}
		return initialized;
	}

//------------------------------------------------------------------------------
	boolean isWiFIConnected(){
		return WiFi.status() == WL_CONNECTED;
	}

	void connectToWiFiIfNotConnected(){
		uint8_t count=0;

		while(!isWiFIConnected() ){
			if(count==0 || count>=NOT_CONNECT_ITERATIONS || oldstatus!=WiFi.status()){
				displayDetails();
				count=1;
			}else{
				Serial.print(FPSTR("."));
				delay(RE_CONNECT_DELAY);
				count++;
			}

			oldstatus=WiFi.status();
		}
	}

	boolean checkAuthentication(){


//	&& espSettingsBox->getExtraValue(ExtraBox_Own, OWN_settingsPass.length()!=0
		if(espSettingsBox->getExtraValue(ExtraBox_Own, OWN_settingsUser).length()!=0
				&& espSettingsBox->getExtraValue(ExtraBox_Own, OWN_settingsPass).length()){
			//Serial.println("Authentication is REQUIRED for setupPage");
			if(!serv->authenticate(const_cast<char*>(espSettingsBox->getExtraValue(ExtraBox_Own, OWN_settingsUser).c_str()),
				const_cast<char*>(espSettingsBox->getExtraValue(ExtraBox_Own, OWN_settingsPass).c_str()))){
				serv->requestAuthentication();
				return false;
			}else{
				return true;
			}
		}else{
			return true;
		}
	}

	void printRequestDetails(){
		Serial.println(FPSTR("---Request details---"));
		Serial.print(FPSTR("url="));
		Serial.println(serv->uri());
		Serial.print(FPSTR("args count="));
		Serial.println(serv->args());
		for(int i=0;i<serv->args();i++){
			Serial.print(serv->argName(i));
			Serial.print(FPSTR("="));
			Serial.println(serv->arg(i));
		}
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
	}

	boolean isValidGetJsonRequest(){
		return serv->hasArg(FPSTR(MESSAGE_SERVER_ARG_NAME));
	}

	boolean isValidProcessJsonRequest(){
		return serv->hasArg(MESSAGE_SERVER_ARG_REMOTE_TARGET)
				&& serv->hasArg(MESSAGE_SERVER_ARG_REMOTE_PAGE)
				&& serv->hasArg(FPSTR(MESSAGE_SERVER_ARG_VAL_JSON));
	}

	String getPageParam(){
		return serv->hasArg(FPSTR(MESSAGE_SERVER_ARG_PAGE))?
				serv->arg(FPSTR(MESSAGE_SERVER_ARG_PAGE)):
				"";
	}

	String getEntityParam(){
		return serv->hasArg(FPSTR(MESSAGE_SERVER_ARG_ENTITY))?
				serv->arg(FPSTR(MESSAGE_SERVER_ARG_ENTITY)):
				"";
	}

	String getNameParam(){
		return serv->arg(FPSTR(MESSAGE_SERVER_ARG_NAME));
	}

	String getRemoteTargetParam(){
		return serv->arg(FPSTR(MESSAGE_SERVER_ARG_REMOTE_TARGET));
	}

	String getRemotePageParam(){
		return serv->arg(FPSTR(MESSAGE_SERVER_ARG_REMOTE_PAGE));
	}

	String getValJsonParam(){
		return serv->arg(FPSTR(MESSAGE_SERVER_ARG_VAL_JSON));
	}

	void setupServer(){
		//printHeap();
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		Serial.println(FPSTR(MESSAGE_WIFIHELPER_SETUP_SERVER));

#ifdef ESP8266
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_MODE_EQ));
		Serial.print(WiFi.getMode());
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_GET_AUTO_CONNECT));
		Serial.print(WiFi.getAutoConnect());
#endif
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_EQ));
		Serial.println(getWiFiStatusStr(WiFi.status()));

		//printHeap();
		if(initStaticPages){
			initStaticPagesInWebFolder();
		}
		//printHeap();
		initDefaultServerHandlers();
		//
		if(serverPostInitFunc!=nullptr){
			Serial.println(FPSTR(MESSAGE_WIFIHELPER_POST_INIT_WEB_SERV_HANDLERS));
			serverPostInitFunc();
			EspSettingsUtil::printHeap();
		}
		serv->begin();
		Serial.println(FPSTR(MESSAGE_WIFIHELPER_SERVER_SETUP_COMPLETED));
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		printHeap();
	}

	void initUpdater(){
		if(updater!=nullptr){
			updater->setup(serv,espSettingsBox->getExtraValue(ExtraBox_Own,OWN_settingsUser).c_str(),
					espSettingsBox->getExtraValue(ExtraBox_Own, OWN_settingsPass).c_str());
		}else{
			Serial.println(FPSTR("OTA Update is disabled on this device"));
		}
	}

	void returnFail(String msg) {
	  serv->send(500, FPSTR(CONTENT_TYPE_TEXT_PLAIN), msg + "\r\n");
	}
	//-------------------------------
	String executeGetRequest(String url){
		if(url==""){
			return "EmptyUrl";
		}

		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_GET));
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_URL_EQ));
		Serial.println(url);
		http.begin(url);
		delay(1);

		int httpCode = http.GET();                                                                  //Send the request
		delay(1);

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_EQ));
		Serial.println(httpCode);

		if (httpCode > 0) { //Check the returning code

		  String payload = http.getString();
		  delay(1);
		  http.end();

		  return payload;
		}

		Serial.print(FPSTR(MESSAGE_HORIZONTAL_LINE));

		http.end();
		return "";
	}

	void printHeap(){
		Serial.print(FPSTR(MESSAGE_DEVICE_FREE_HEAP));Serial.print(FPSTR(MESSAGE_EQUALS));Serial.print(ESP.getFreeHeap());
	}

	String executeFormPostRequest(String url,String body){
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_POST));
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_URL_EQ));
		Serial.println(url);
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_HTTP_BODY_EQ));
		Serial.println(body);

		boolean begin=http.begin(url);

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_HTTP_BEGIN_EQ));
		Serial.println(begin);

		delay(1);

		http.addHeader(FPSTR(CONTENT_TYPE),FPSTR(CONTENT_TYPE_FORM_URL_ENCODED));

		int httpCode = http.POST(body);
		yield();

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_EQ));
		Serial.println(httpCode);

		if (httpCode > 0) {

		  String payload = http.getString();

		  Serial.print(FPSTR(MESSAGE_WIFIHELPER_RESPONSE_EQ));
		  Serial.println(payload);
		  Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		  http.end();

		  return payload;
		}

		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

		http.end();
		return "";
	}
	//---------------------------------------------------------------------
	boolean isAP(){
		return startedAsAP;
	}

	IPAddress getIp(){
		if(espSettingsBox->getExtraValueBoolean(ExtraBox_Own, OWN_isAccesPoint)){
	#ifdef ESP8266
				return  WiFi.softAPIP();
	#endif
	#ifdef ESP32
				return WiFi.localIP();
	#endif
		}
		return WiFi.localIP();
	}

	String getIpStr(){
		return getIp().toString();
	}

	boolean getReconnected(){
		boolean result =this->reconnected;
		this->reconnected=false;

		return result;
	}

	ESP8266WebServer* server(){
		return this->serv;
	}

	//---------------------------------------------------------------------
		String getWiFiStatusStr(wl_status_t status){
			if(status>7 || status<0){
				return FPSTR(WL_UNKNOWN_STATUS);
			}else{
				return FPSTR(WIFIHELPER_WIFI_STATUSES[status]);
			}
		}

		String macToString(const unsigned char* mac) {
		  char buf[20];
		  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
				   mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		  return String(buf);
		}

		String formatBytes(size_t bytes){
		  if (bytes < 1024){
		    return String(bytes)+"B";
		  } else if(bytes < (1024 * 1024)){
		    return String(bytes/1024.0)+"KB";
		  } else if(bytes < (1024 * 1024 * 1024)){
		    return String(bytes/1024.0/1024.0)+"MB";
		  } else {
		    return String(bytes/1024.0/1024.0/1024.0)+"GB";
		  }
		}

		String getContentType(String filename){
		  if(serv->hasArg("download")) return FPSTR(CONTENT_TYPE_APPLICATION_OCTED_STREAM);
		  else if(filename.endsWith(".htm")) return FPSTR(CONTENT_TYPE_TEXT_HTML);
		  else if(filename.endsWith(".html")) return FPSTR(CONTENT_TYPE_TEXT_HTML);
		  else if(filename.endsWith(".css")) return FPSTR(CONTENT_TYPE_TEXT_CSS);
		  else if(filename.endsWith(".js")) return FPSTR(CONTENT_TYPE_APPLICATION_JAVASCRIPT);
		  else if(filename.endsWith(".png")) return FPSTR(CONTENT_TYPE_IMAGE_PNG);
		  else if(filename.endsWith(".gif")) return FPSTR(CONTENT_TYPE_IMAGE_GIF);
		  else if(filename.endsWith(".jpg")) return FPSTR(CONTENT_TYPE_IMAGE_JPEG);
		  else if(filename.endsWith(".ico")) return FPSTR(CONTENT_TYPE_IMAGE_XICON);
		  else if(filename.endsWith(".xml")) return FPSTR(CONTENT_TYPE_TEXT_XML);
		  else if(filename.endsWith(".pdf")) return FPSTR(CONTENT_TYPE_APPLICATION_XPDF);
		  else if(filename.endsWith(".zip")) return FPSTR(CONTENT_TYPE_APPLICATION_XZIP);
		  else if(filename.endsWith(".gz")) return FPSTR(CONTENT_TYPE_APPLICATION_XGZIP);
		  return FPSTR(CONTENT_TYPE_TEXT_PLAIN);
		}
/*
	void printHeap(){
		espSettingsBox->printHeap();
	}
*/
	void displayDetails(){
		delay(1);
		Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_DIAGNOSTIC));
		EspSettingsUtil::printHeap();
#ifdef ESP8266
		//WiFi.printDiag(Serial);
#endif
		wl_status_t status=WiFi.status();

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_EQ));
		Serial.print(FPSTR(" ("));
		Serial.print(status);
		Serial.print(FPSTR(")  ="));
		Serial.println(getWiFiStatusStr(status));

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_MODE));
		Serial.print(FPSTR(WIFIHELPER_WIFI_MODES[WiFi.getMode()]));

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_PHYMODE));
		Serial.print(FPSTR(WIFIHELPER_PHY_MODES[WiFi.getPhyMode()]));

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_CHANNEL));
		Serial.print(WiFi.channel());

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_SlEEPMODE));
		Serial.print(FPSTR(WIFIHELPER_SlEEP_MODES[WiFi.getSleepMode()]));

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_AUTOCONNECT));
		Serial.print(WiFi.getAutoConnect());

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_SSSID_EQ));
		Serial.println ( WiFi.SSID() );

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_IP_TYPE_EQ));

		if(espSettingsBox->getExtraValueBoolean(ExtraBox_Own, OWN_staticIp)){
			Serial.println ( FPSTR(MESSAGE_WIFIHELPER_WIFI_STATIC_IP_TYPE));
		}else{
			Serial.println ( FPSTR(MESSAGE_WIFIHELPER_WIFI_DNS_IP_TYPE));
		}

		Serial.print (FPSTR(MESSAGE_WIFIHELPER_WIFI_IP));
		Serial.println ( getIpStr() );

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_MAC));
#ifdef ESP8266
		Serial.println(WiFi.macAddress());
#endif
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

	}
protected:
	//---------------initializations----------------------------------------
	void initDefaultServerHandlers(){

		Serial.println(FPSTR("Init default server handlers"));

		serv->on(FPSTR(URL_LIST), HTTP_GET, [this](){handleFileList();});
		serv->on(FPSTR(URL_VIEW), HTTP_GET, [this](){handleFileView ();});
		serv->on(FPSTR(URL_TEST), HTTP_GET, [this](){handleTest();});

		//init HTM.GZ files return
		serv->on(FPSTR(URL_INDEX), HTTP_GET, [this](){
			if(!handleFileGzRead(FPSTR(MESSAGE_WIFIHELPER_INDEX_HTML_PAGE)))
				serv->send(404, FPSTR(CONTENT_TYPE_TEXT_PLAIN), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_FILE_NOT_FOUND));
		});
		serv->on(FPSTR(URL_SETTINGS), HTTP_GET, [this](){
			if(!handleFileGzRead(FPSTR(MESSAGE_WIFIHELPER_SETTINGS_HTML_PAGE)))
				serv->send(404, FPSTR(CONTENT_TYPE_TEXT_PLAIN), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_FILE_NOT_FOUND));
		});
		serv->on(FPSTR(URL_EDIT), HTTP_GET, [this](){
			if(!handleFileGzRead(FPSTR(MESSAGE_WIFIHELPER_EDIT_HTML_PAGE)))
				serv->send(404, FPSTR(CONTENT_TYPE_TEXT_PLAIN), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_FILE_NOT_FOUND));
		});
		serv->on(FPSTR(URL_ROOT), HTTP_GET, [this](){
			if(!handleFileGzRead(FPSTR(MESSAGE_WIFIHELPER_INDEX_HTML_PAGE)))
				serv->send(404, FPSTR(CONTENT_TYPE_TEXT_PLAIN), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_FILE_NOT_FOUND));
		});
		//serv->on(FPSTR(URL_EDIT), HTTP_PUT, [this](){handleFileCreate();});
		serv->on(FPSTR(URL_EDIT), HTTP_DELETE, [this](){handleFileDelete();});
		serv->on(FPSTR(URL_EDIT), HTTP_POST, [this](){ serv->send(200, FPSTR(CONTENT_TYPE_TEXT_PLAIN), ""); }, [this](){handleFileUpload();});

		serv->onNotFound([this](){handleNotFound();});

		Serial.println(FPSTR("Default serv handlers...done"));
	}

	boolean startAsAccessPoint(){
		startedAsAP=true;

		WiFi.mode(WIFI_AP);
		displayLine(FPSTR(MESSAGE_WIFIHELPER_START_AP),3,0);
		displayLine(FPSTR(MESSAGE_WIFIHELPER_ACCESS_POINT),5,0);

		initWiFiApEventFunctions();

		Serial.println(FPSTR(MESSAGE_WIFIHELPER_STARTING_ACCESS_POINT));
		Serial.print(FPSTR("OWN_ssidAP="));
		Serial.print(espSettingsBox->getExtraValue(ExtraBox_Own, OWN_ssidAP).c_str());
		Serial.print(FPSTR("OWN_password="));
		Serial.println(espSettingsBox->getExtraValue(ExtraBox_Own, OWN_password).c_str());

		WiFi.softAP((espSettingsBox->getExtraValue(ExtraBox_Own, OWN_ssidAP).c_str()),(espSettingsBox->getExtraValue(ExtraBox_Own, OWN_password).c_str()));

		Serial.print (FPSTR(MESSAGE_WIFIHELPER_SOFT_AP));
		Serial.println ( espSettingsBox->getExtraValue(ExtraBox_Own, OWN_ssidAP));
		Serial.print ( FPSTR(MESSAGE_WIFIHELPER_WIFI_IP));
		Serial.println ( WiFi.softAPIP() );

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_EQ));
		Serial.println(WiFi.status());
		Serial.println ( FPSTR(MESSAGE_HORIZONTAL_LINE));

		return true;
	}

	boolean startAsClient(){
		startedAsAP=false;

		/*displayLine(FPSTR(MESSAGE_WIFIHELPER_CONNECT_TO),2,0);
		displayLine(espSettingsBox->ssid,5,0);
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_ESP_SETTINGS_BOX_SSID));
		Serial.println(espSettingsBox->ssid);
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_ESP_SETTINGS_BOX_PASSWORD));
		Serial.println(espSettingsBox->password);
		*/

		WiFi.disconnect(0);
		WiFi.persistent(false);
		WiFi.mode(WIFI_STA);

		initWifiStaEventFunctions();

		//espSettingsBox->getExtraBox(ExtraBox_Own)->printDetails();

		Serial.print(FPSTR(" is_staticIp="));
		Serial.print(espSettingsBox->getExtraValueBoolean(ExtraBox_Own, OWN_staticIp));
		Serial.print(FPSTR(" OWN_ssid="));
		Serial.print(espSettingsBox->getExtraValue(ExtraBox_Own, OWN_ssid).c_str());
		Serial.print(FPSTR(" OWN_password="));
		Serial.println(espSettingsBox->getExtraValue(ExtraBox_Own, OWN_password).c_str());
		Serial.print(FPSTR(" OWN_localIp="));
		Serial.print(espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_localIp));
		Serial.print(FPSTR(" OWN_gateIp="));
		Serial.print(espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_gateIp));
		Serial.print(FPSTR(" OWN_subnetIp="));
		Serial.print(espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_subnetIp));
		Serial.print(FPSTR(" dnsIp="));
		Serial.print(espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_dnsIp));
		Serial.print(FPSTR(" dnsIp2="));
		Serial.println(espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_dnsIp2));

		if(espSettingsBox->getExtraValueBoolean(ExtraBox_Own, OWN_staticIp)){
			WiFi.config(
					espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_localIp),
					espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_gateIp),
					espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_subnetIp),
					espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_dnsIp),
					espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_dnsIp2)
					);
		}else{

			WiFi.config(0U,0U,0U,espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_dnsIp),
					espSettingsBox->getExtraValueIpAdress(ExtraBox_Own, OWN_dnsIp2));
		}

		WiFi.begin ( const_cast<char*>(espSettingsBox->getExtraValue(ExtraBox_Own, OWN_ssid).c_str()),
				const_cast<char*>(espSettingsBox->getExtraValue(ExtraBox_Own, OWN_password).c_str()) );

		connectToWiFiIfNotConnected();

		return true;
	}
	//---------------HTTP handlers-------------------------------------------
	void handleNotFound(){
		Serial.print(FPSTR("NOT found uri="));
		Serial.println(serv->uri());

		String corrUrl=getCorrectedUrl(String(serv->uri()).substring(1));

		if(corrUrl!=""){
			Serial.print(FPSTR("Corrected "));
			Serial.println(corrUrl);
			serv->sendHeader(FPSTR(HEADER_Location), corrUrl, true);
			serv->send ( 302, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "");
		}

		serv->send(404,FPSTR(CONTENT_TYPE_TEXT_HTML), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_TEXT_NOT_FOUND));
	}

	void handleTest(){
		  //get heap status, analog input value and all GPIO statuses in one json call
			/*  serv->on("/all", HTTP_GET, [this](){
				String json = "{";
				json += "\"heap\":"+String(ESP.getFreeHeap());
				json += ", \"analog\":"+String(analogRead(A0));
			#ifdef ESP8266
				json += ", \"gpio\":"+String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
			#endif
				json += "}";
				serv->send(200, "text/json", json);
				json = String();
			  });
				*/
		serv->send ( 200, FPSTR(CONTENT_TYPE_TEXT_HTML), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_TEXT_I_M_HERE));
	}

	bool handleFileRead(String path){
		Serial.println("handleFileRead: " + path);
		  String contentType = getContentType(path);
			  if(SPIFFS.exists(path)){
				File file = SPIFFS.open(path, "r");
				//size_t sent =
				serv->streamFile(file, contentType);
				file.close();
				return true;
			  }
		  return false;
	}

	bool handleFileGzRead(String path){
	  Serial.println("handleFileGzRead: " + path);
	  if(path.endsWith("/")) path += "index.htm";
	  String contentType = getContentType(path);
	  String pathWithGz = path + ".gz";
	  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
		if(SPIFFS.exists(pathWithGz))
		  path += ".gz";
		File file = SPIFFS.open(path, "r");
		//size_t sent =
		serv->streamFile(file, contentType);
		file.close();
		return true;
	  }
	  return false;
	}

	String getCorrectedUrl(String url){
		if(url.startsWith(FPSTR(URL_INDEX))){
			return FPSTR(URL_INDEX);
		}
		if(url.startsWith(FPSTR(URL_SETTINGS))){
			return FPSTR(URL_SETTINGS);
		}
		if(url.startsWith(FPSTR(URL_EDIT))){
			return FPSTR(URL_EDIT);
		}

		return "";
	}

	void handleFileUpload(){
	  if(serv->uri() != FPSTR(URL_EDIT)) return;

	  HTTPUpload& upload = serv->upload();

	  if(upload.status == UPLOAD_FILE_START){
		String filename = upload.filename;
		if(!filename.startsWith("/")) filename = "/"+filename;
		Serial.print(FPSTR("handleFileUpload Name: ")); Serial.println(filename);
		fsUploadFile = SPIFFS.open(filename, "w");
		filename = String();
	  } else if(upload.status == UPLOAD_FILE_WRITE){
		//DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
		if(fsUploadFile)
		  fsUploadFile.write(upload.buf, upload.currentSize);
	  } else if(upload.status == UPLOAD_FILE_END){
		if(fsUploadFile)
		  fsUploadFile.close();
		Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
	  }
	}

	void handleFileDelete(){
	  if(serv->args() == 0) return serv->send(500, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "BAD ARGS");
	  String path = serv->arg(0);
	  Serial.print(FPSTR("handleFileDelete: "));
	  Serial.println(path);

	  if(path == "/")
		return serv->send(500, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "BAD PATH");
	  if(!SPIFFS.exists(path))
		return serv->send(404, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "FileNotFound");
	  SPIFFS.remove(path);
	  serv->send(200, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "");
	  path = String();
	}

	void handleFileCreate(){
	  if(serv->args() == 0)
		return serv->send(500, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "BAD ARGS");
	  String path = serv->arg(0);
	  Serial.println("handleFileCreate: " + path);
	  if(path == "/")
		return serv->send(500, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "BAD PATH");
	  if(SPIFFS.exists(path))
		return serv->send(500, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "FILE EXISTS");
	  File file = SPIFFS.open(path, "w");
	  if(file)
		file.close();
	  else
		return serv->send(500, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "CREATE FAILED");
	  serv->send(200, FPSTR(CONTENT_TYPE_TEXT_PLAIN), "");
	  path = String();
	}

	void handleFileView(){
	  if(!serv->hasArg(FPSTR(SERVER_ARG_file))) {
		returnFail("BAD ARGS file is missed in params");
		return;
	  }

	  if(serv->hasArg(FPSTR(SERVER_ARG_file))){
		  handleFileRead(serv->arg(FPSTR(SERVER_ARG_file)));
	  }
	}

	#ifdef ESP8266
	void handleFileList() {

	  String path = (serv->hasArg(FPSTR(SERVER_ARG_dir)))?
			  serv->arg(FPSTR(SERVER_ARG_dir)):"/";

	  String kind = (serv->hasArg(FPSTR(SERVER_ARG_kind)))?
			  serv->arg(FPSTR(SERVER_ARG_kind)):FPSTR(SERVER_ARG_list);

	  Serial.print(FPSTR("handleFileList: "));
	  Serial.println(path);

	  Dir dir = SPIFFS.openDir(path);
	  path = String();

	  String output="";

	  if(kind!=FPSTR(SERVER_ARG_list)){
		  output += "{\"files\":[";
		  while(dir.next()){
			File entry = dir.openFile("r");
			if (output != "[") output += ',';
			bool isDir = false;
			output += "{\"type\":\"";
			output += (isDir)?FPSTR(SERVER_ARG_dir):FPSTR(SERVER_ARG_file);
			output += "\",\"name\":\"";
			output += String(entry.name());
			output += "\"}";
			entry.close();
		  }

		  output += "]}";

		  serv->send(200, FPSTR(CONTENT_TYPE_TEXT_JSON), output);
	  }else{
		  while(dir.next()){
			File entry = dir.openFile("r");
			bool isDir = false;
			output += String(entry.name());
			output += FPSTR(NEXT_LINE_RN);
			entry.close();
		  }
		  serv->send(200, FPSTR(CONTENT_TYPE_TEXT_PLAIN), output);
	  }

	}
	#else
	void handleFileList() {
	  if(!serv->hasArg("dir")) {
		returnFail("BAD ARGS");
		return;
	  }
	  String path = serv->arg("dir");
	  if(path != "/" && !SPIFFS.exists((char *)path.c_str())) {
		returnFail("BAD PATH");
		return;
	  }
	  File dir = SPIFFS.open((char *)path.c_str());
	  path = String();
	  if(!dir.isDirectory()){
		dir.close();
		returnFail("NOT DIR");
		return;
	  }
	  dir.rewindDirectory();

	  String output = "[";
	  for (int cnt = 0; true; ++cnt) {
		File entry = dir.openNextFile();
		if (!entry)
		break;

		if (cnt > 0)
		  output += ',';

		output += "{\"type\":\"";
		output += (entry.isDirectory()) ? "dir" : FPSTR(SERVER_ARG_file);
		output += "\",\"name\":\"";
		// Ignore '/' prefix
		output += entry.name()+1;
		output += "\"";
		output += "}";
		entry.close();
	  }
	  output += "]";
	  serv->send(200, "text/json", output);
	  dir.close();
	}

	void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
	  Serial.printf("Listing directory: %s\n", dirname);

	  File root = fs.open(dirname);
	  if (!root) {
		  Serial.println("Failed to open directory");
		return;
	  }
	  if (!root.isDirectory()) {
		  Serial.println("Not a directory");
		return;
	  }

	  File file = root.openNextFile();
	  while (file) {
		if (file.isDirectory()) {
			Serial.print("  DIR : ");
			Serial.println(file.name());
		  if (levels) {
			listDir(fs, file.name(), levels - 1);
		  }
		} else {
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("  SIZE: ");
			Serial.println(file.size());
		}
		file = root.openNextFile();
	  }
	}
	#endif
	//-----------------------------------------------------------------------
		void initStaticPagesInWebFolder(){
	#ifdef ESP8266
			deployStaticFolder(FPSTR(ESPSETTINGSBOX_DEFAULT_WEB_FOLDER),
								FPSTR(ESPSETTINGSBOX_DEPLOY_EXT),
								"",5);
	#endif
	#ifdef ESP32
			deployStaticFolder(FPSTR(ESPSETTINGSBOX_DEFAULT_WEB_FOLDER),
								FPSTR(ESPSETTINGSBOX_DEPLOY_EXT),
								"",5);
	#endif

			/*deployStaticFolder(FPSTR(ESPSETTINGSBOX_SETTINGS_PATH),
										FPSTR(ESPSETTINGSBOX_DEPLOY_EXT),
										FPSTR(ESPSETTINGSBOX_SETTINGS_DEPLOY_PATH));
										*/
		}

		void deployStaticFolder(String basePath,String extensions,String baseUrl,uint8_t start){
			Serial.print(FPSTR(MESSAGE_WIFIHELPER_DEPLOYING_PATH));
			Serial.println(basePath);
			Serial.print(FPSTR(MESSAGE_WIFIHELPER_AS_WEB_FILES));
			Serial.print(FPSTR(MESSAGE_WIFIHELPER_EXTENSIONS));
			Serial.println(extensions);
			Serial.print(FPSTR(MESSAGE_WIFIHELPER_TO_BE_DEPLOYED));
			Serial.print(FPSTR(MESSAGE_WIFIHELPER_BASE_URL_EQ));
			Serial.println(baseUrl);
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
			//        /js   /css     /html
	#ifdef ESP8266
			Dir dir=SPIFFS.openDir(basePath);

			while(dir.next()){
				File file=dir.openFile("r");
	#endif
	#ifdef ESP32
			File dir =SPIFFS.open(FPSTR(URL_ROOT));

			if(dir.isDirectory()){
				Serial.println(FPSTR("Base dir found"));
				dir.rewindDirectory();
			}else{
				Serial.println(FPSTR("NO base Base dir found"));
			}
			File file=dir.openNextFile();

			if(file){
				Serial.print(FPSTR("First file "));
				Serial.println(file.name());
			}else{
				Serial.println("File is invalid");
			}

			while(file){
	#endif
				String fileNameStr=String(file.name());

				String url=fileNameStr.substring(basePath.length());

				Serial.print(FPSTR("FIle="));
				Serial.print(fileNameStr);

				Serial.print(FPSTR(" size="));
				Serial.print(file.size());

				if(fileNameStr.startsWith(basePath)){
					Serial.print(FPSTR(" uri="));
					Serial.print(String(file.name()).substring(start-1).c_str());


					serv->serveStatic(String(file.name()).substring(start-1).c_str(), SPIFFS, file.name());

					Serial.print(FPSTR("...deployed"));
				}

				Serial.println();




	#ifdef ESP32
			file=dir.openNextFile();
	#endif

			}
			Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
		}
	//------------------------------
	//display helper functions
	boolean cleanDisplay(){
		if(displayHelper==nullptr){
			return false;
		}
		return displayHelper->clearDisplay();
	}

	boolean displayLine(String str,int row,int col){
		if(displayHelper==nullptr){
			return false;
		}
		return displayHelper->addStringToDisplay(str, row, col, FPSTR(MESSAGE_WIFIHELPER_NAME));
	}
//---------------------------------init event functions----------------------
	void initWiFiApEventFunctions(){
		reconnected=false;

		wiFiEventSoftAPModeProbeRequestReceivedHandler=WiFi.onSoftAPModeProbeRequestReceived([this](const WiFiEventSoftAPModeProbeRequestReceived& evt){onSoftAPModeProbeRequestReceived(evt);});
		onSoftAPModeStationConnectedHandler=WiFi.onSoftAPModeStationConnected([this](const WiFiEventSoftAPModeStationConnected& evt){onSoftAPModeStationConnected(evt);});
		onSoftAPModeStationDisconnectedHandler=WiFi.onSoftAPModeStationDisconnected([this](const WiFiEventSoftAPModeStationDisconnected& evt){onSoftAPModeStationDisconnected(evt);});

		initWiFiExternalEventFunctions();
	}

	void initWifiStaEventFunctions(){
		reconnected=false;

		stationModeConnectedHandler=WiFi.onStationModeConnected([this](const WiFiEventStationModeConnected& evt){onStationModeConnected(evt);});
		onStationModeDisconnectedHandler=WiFi.onStationModeDisconnected([this](const WiFiEventStationModeDisconnected& evt){onStationModeDisconnected(evt);});
		onStationModeDHCPTimeoutHandler=WiFi.onStationModeDHCPTimeout([this](){onStationModeDHCPTimeout();});
		onStationModeGotIPHandler=WiFi.onStationModeGotIP([this](const WiFiEventStationModeGotIP& evt){onStationModeGotIP(evt);});

		initWiFiExternalEventFunctions();
	}

	void initWiFiExternalEventFunctions(){
		if(serverInitEventFunc!=nullptr){
			Serial.println(FPSTR("Init event functions"));
			serverInitEventFunc();
		}

		Serial.println(FPSTR("Wifi Init functions complete"));
	}

	void onSoftAPModeProbeRequestReceived(const WiFiEventSoftAPModeProbeRequestReceived& evt) {
	  Serial.print(FPSTR("AP WiFiEventSoftAPModeProbeRequestReceived: MAC="));
	  Serial.print(macToString(evt.mac));
	  Serial.print(FPSTR(" rssi="));
	  Serial.println(evt.rssi);

	}

	void onSoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected& evt) {
	  Serial.print(FPSTR("AP connected: MAC="));
	  Serial.println(macToString(evt.mac));

	}

	void onSoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) {
	  Serial.print(FPSTR("AP disconnected: "));
	  Serial.println(macToString(evt.mac));
	}

	void onStationModeConnected(const WiFiEventStationModeConnected& evt){
		Serial.print(FPSTR("STA connected: "));
		Serial.print(FPSTR(" ssid="));
		Serial.print(evt.ssid);
		Serial.print(FPSTR(" bssid="));
		Serial.print(macToString(evt.bssid));
		Serial.print(FPSTR(" channel="));
		Serial.println(evt.channel);
	}

	void onStationModeDisconnected(const WiFiEventStationModeDisconnected& evt){
		Serial.print(FPSTR("STA DISconnected: "));
		Serial.print(FPSTR(" ssid="));
		Serial.print(evt.ssid);
		Serial.print(FPSTR(" bssid="));
		Serial.print(macToString(evt.bssid));
		Serial.print(FPSTR(" reason="));
		Serial.println(evt.reason);
	}

	void onStationModeDHCPTimeout(){
		Serial.print(FPSTR("STA DHCP timed out: "));
	}

	void onStationModeGotIP(const WiFiEventStationModeGotIP& evt){
		Serial.print(FPSTR("onStationModeGotIP IP="));
		Serial.println(evt.ip);
		reconnected=true;
	}

	//----------espSettings save-------------------------------------------
	/*String setEspSettingsBoxValuesC(){
		checkAuthentication();
		String page=server->arg(FPSTR(MESSAGE_SERVER_ARG_PAGE));

		espSettingsBox->beginSetSettingsValue(page);

		for(uint8_t i=0;i<server->args();i++){
			String argName=server->argName(i);
			String argVal=server->arg(i);

			if(!espSettingsBox->setSettingsValue(argName,argVal)){
				Serial.print(FPSTR("FAILED find target for argName="));
				Serial.print(argName);
				Serial.print(FPSTR(" argVal="));
				Serial.println(argVal);
			}
		}

		espSettingsBox->finishSetSettingsValue(page);

		return espSettingsBox->getJson(page);
	}*/

private:
#ifdef ESP8266
	ESP8266HTTPUpdateServer *updater;
	ESP8266WebServer *serv;
#endif
#ifdef ESP32
	WebServer *server;
#endif
	//String name;
	ulong lastConnected=0;
	wl_status_t oldstatus;

	EspSettingsBox *espSettingsBox;
	DisplayHelper *displayHelper;
	//PinDigital *signalPin;
	std::function<void(void)> serverInitEventFunc;
	std::function<void(void)> serverPostInitFunc;
	boolean disconnectOnStartIfConnected;

	//std::function<void(void)> handleHttpWidget;
	//std::function<PinEvent(PinEvent)> processPinEvent;

	boolean initStaticPages;
	boolean initFilesManager;

	File fsUploadFile;
	HTTPClient http;

	boolean startedAsAP=false;

	boolean reconnected=false;

	WiFiEventHandler wiFiEventSoftAPModeProbeRequestReceivedHandler;
	WiFiEventHandler onSoftAPModeStationConnectedHandler;
	WiFiEventHandler onSoftAPModeStationDisconnectedHandler;

	WiFiEventHandler stationModeConnectedHandler;
	WiFiEventHandler onStationModeDHCPTimeoutHandler;
	WiFiEventHandler onStationModeDisconnectedHandler;
	WiFiEventHandler onStationModeGotIPHandler;

	};
	/*
		void printWiFiStatus(wl_status_t status){
			Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_EQ));
			Serial.print(FPSTR(" ("));
			Serial.print(status);
			Serial.print(FPSTR(")  ="));
			if(status>5 || status<0){
				Serial.println(FPSTR(WL_UNKNOWN_STATUS));
			}else{
				Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS[status]));
			}

			Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_EQ));
			switch(status) {
			        case 3:
			        	Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_WL_CONNECTED));
			        	break;
			        case WL_NO_SSID_AVAIL:
			        	Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_WL_CONNECTED));
			        	break;
			        case 4:
			        	Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_WL_CONNECT_FAILED));
			        	break;
			        case 0:
			        	Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_WL_IDLE_STATUS));
			        	break;
			        case WL_SCAN_COMPLETED:
			        	Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_WL_SCAN_COMPLETED));
						break;
					default:
						Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_WL_DISCONNECTED));
			}
		}
		*/
	/*
#ifdef ESP32
	boolean startAsAccessPoint(){
		displayLine(FPSTR(MESSAGE_WIFIHELPER_START_AP),3,0);
		displayLine(FPSTR(MESSAGE_WIFIHELPER_ACCESS_POINT),5,0);

		Serial.println(FPSTR(MESSAGE_WIFIHELPER_STARTING_ACCESS_POINT));
		WiFi..beginAP(const_cast<char*>(espSettingsBox->ssidAP.c_str()));
		WiFi.configAP(espSettingsBox->apIp);

		Serial.print (FPSTR(MESSAGE_WIFIHELPER_SOFT_AP));
		Serial.println ( espSettingsBox->ssidAP);
		Serial.print ( FPSTR(MESSAGE_WIFIHELPER_WIFI_IP));
		Serial.println ( WiFi.localIP());

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_EQ));
		Serial.println(WiFi.status());
		Serial.println ( FPSTR(MESSAGE_HORIZONTAL_LINE));

		return true;
	}

	boolean startAsClient(){
		displayLine(FPSTR(MESSAGE_WIFIHELPER_CONNECT_TO),2,0);
		displayLine(espSettingsBox->ssid,5,0);
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_ESP_SETTINGS_BOX_SSID));
		Serial.println(espSettingsBox->ssid);
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_ESP_SETTINGS_BOX_PASSWORD));
		Serial.println(espSettingsBox->password);

		WiFi.disconnect();

		if(espSettingsBox->staticIp){
			WiFi.config(espSettingsBox->localIp);
		}else{
			//WiFi.config(0U,0U,0U,espSettingsBox->dnsIp,espSettingsBox->dnsIp2);
		}

		WiFi.begin ( const_cast<char*>(espSettingsBox->ssid.c_str()),
				const_cast<char*>(espSettingsBox->password.c_str()) );

		connectToWiFiIfNotConnected();

		return true;
	}
#endif
*/
//delpoy static folder
	//addFileHandlersForDir(base,basePath,baseUrl);
			/*
						Serial.print("deploy file url=");
						Serial.println(String(file.name()).substring(start-1).c_str());
						Serial.print(" file=");
						Serial.println(file.name());
			*/
						//file.close();
						/*
						if(!url.startsWith("/")){
							url="/"+url;
						}else{
						}

						url=baseUrl+url;

						File f=dir.openFile("r");
						size_t size=f.size();
						f.close();

						uint8_t extIndex=fileNameStr.lastIndexOf(".");
						String extension=fileNameStr.substring(extIndex+1);

						boolean add=extensions.indexOf(extension)!=-1;

						if(add){
							Serial.print(FPSTR(MESSAGE_WIFIHELPER_ADDED_FILE));
							Serial.print(fileNameStr);
							Serial.print(FPSTR(MESSAGE_WIFIHELPER_SIZE_EQ));
							Serial.print(size);
							Serial.print(FPSTR(MESSAGE_WIFIHELPER_EXT_EQ));
							Serial.print(extension);
							Serial.print(FPSTR(MESSAGE_WIFIHELPER_URL_EQ));
							Serial.println(url);

							server->serveStatic(url.c_str(), SPIFFS, file.name());
						}
						*/

#endif /* LIBRARIES_TIMETRIGGER_WIFIHELPER_H_ */
