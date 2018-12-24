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
#include "Loopable.h"
#include "Initializable.h"
#include "EspSettingsBox.h"
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
#include <AbstractItem.h>
#include <DisplayHelperAbstract.h>

#define NOT_CONNECT_ITERATIONS 100
#define RE_CONNECT_DELAY 250

class WiFiHelper:public Initializable,public Loopable {

public:
	#ifdef ESP8266
	WiFiHelper(EspSettingsBox *_settingsBox, DisplayHelper *_displayHelper/*, PinDigital *_signalPin*/,
			ESP8266WebServer *_server,std::function<void(void)> _serverPostInitFunc,boolean _disconnectOnStartIfConnected){

		server=_server;
		construct(_settingsBox, _displayHelper,_serverPostInitFunc,_disconnectOnStartIfConnected);
	}
	#endif
	#ifdef ESP32
	WiFiHelper(EspSettingsBox *_settingsBox,DisplayHelperAbstract *_displayHelper,
			WebServer *_server,std::function<void(void)> _serverPostInitFunc,boolean _disconnectOnStartIfConnected){

		server=_server;
		construct(_settingsBox, _displayHelper,_serverPostInitFunc,_disconnectOnStartIfConnected);
	}
	#endif


	void construct(EspSettingsBox *_settingsBox, DisplayHelper *_displayHelper/*, PinDigital *_signalPin*/,
			std::function<void(void)> _serverPostInitFunc,boolean _disconnectOnStartIfConnected){
		espSettingsBox=_settingsBox;
		displayHelper=_displayHelper;
		//signalPin=_signalPin;
		serverPostInitFunc=_serverPostInitFunc;
		disconnectOnStartIfConnected=_disconnectOnStartIfConnected;

		initStaticPages=true;
		initFilesManager=true;

		initialized=false;
	}

	boolean initialize(boolean _init){
		if(_init){
			Serial.println(FPSTR(MESSAGE_WIFIHELPER_INIT_CONNECTION));

			displayDetails();
			if(WiFi.status()==3){
				Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_DISCONNECTING));
#ifdef ESP8266
					WiFi.disconnect(1);
#endif
#ifdef ESP32
					WiFi.disconnect();
#endif
			}

			if(espSettingsBox->isAccesPoint){
				startAsAccessPoint();
			}else{
				startAsClient();
			}

			setupServer();

		}
		initialized=_init;
		return true;
	}

	void printHeap(){
		Serial.print(FPSTR("heap="));
		Serial.println(ESP.getFreeHeap());
	}

	boolean loop(){
		if(initialized){
			if(!isWiFIConnected()){
				connectToWiFiIfNotConnected();
			}
			server->handleClient();
		}
		return initialized;
	}

	//---------------------------------------------------------------------------------------
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
		Serial.println(WiFi.status());
		//printHeap();
		server->onNotFound([this](){handleNotFound();});
		//printHeap();
		server->serveStatic("/", SPIFFS, String(FPSTR(ESPSETTINGSBOX_DEFAULT_PAGE)).c_str());
		//printHeap();
		if(initStaticPages){
			initStaticPagesInWebFolder();
		}
		//printHeap();
		if(initFilesManager){
			initDefaultFiles();
		}
		//printHeap();
		if(serverPostInitFunc!=nullptr){
			Serial.println(FPSTR(MESSAGE_WIFIHELPER_POST_INIT_WEB_SERV_HANDLERS));
			serverPostInitFunc();
			printHeap();
		}
		server->begin();
		Serial.println(FPSTR(MESSAGE_WIFIHELPER_SERVER_SETUP_COMPLETED));
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));

	}

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
		File dir =SPIFFS.open("/");

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


				server->serveStatic(String(file.name()).substring(start-1).c_str(), SPIFFS, file.name());

				Serial.print(FPSTR("...deployed"));
			}

			Serial.println();




#ifdef ESP32
		file=dir.openNextFile();
#endif

		}
		Serial.println(FPSTR(MESSAGE_HORIZONTAL_LINE));
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
	}
	//-----------------------------------------------------------------------

	void handleNotFound(){
		/*if(!handleFileRead(server->uri()))
			  server->send(404, "text/plain", "FileNotFound");
*/
		Serial.println(FPSTR("Handle not found"));
		Serial.println(server->uri());
		server->send(404,FPSTR(CONTENT_TYPE_TEXT_HTML), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_TEXT_NOT_FOUND));
	}

	void handleTest(){
		server->send ( 200, FPSTR(CONTENT_TYPE_TEXT_HTML), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_TEXT_I_M_HERE));
	}
	//-----------------------------------------------------------------------

	boolean startAsAccessPoint(){
		WiFi.mode(WIFI_AP);
		displayLine(FPSTR(MESSAGE_WIFIHELPER_START_AP),3,0);
		displayLine(FPSTR(MESSAGE_WIFIHELPER_ACCESS_POINT),5,0);

		setupApEvents();

		Serial.println(FPSTR(MESSAGE_WIFIHELPER_STARTING_ACCESS_POINT));
		WiFi.softAP(const_cast<char*>(espSettingsBox->ssidAP.c_str()));

		Serial.print (FPSTR(MESSAGE_WIFIHELPER_SOFT_AP));
		Serial.println ( espSettingsBox->ssidAP);
		Serial.print ( FPSTR(MESSAGE_WIFIHELPER_WIFI_IP));
		Serial.println ( WiFi.softAPIP() );

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_EQ));
		Serial.println(WiFi.status());
		Serial.println ( FPSTR(MESSAGE_HORIZONTAL_LINE));

		startedAsAP=true;

		return true;
	}

	boolean startAsClient(){
		displayLine(FPSTR(MESSAGE_WIFIHELPER_CONNECT_TO),2,0);
		displayLine(espSettingsBox->ssid,5,0);
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_ESP_SETTINGS_BOX_SSID));
		Serial.println(espSettingsBox->ssid);
		Serial.print(FPSTR(MESSAGE_WIFIHELPER_ESP_SETTINGS_BOX_PASSWORD));
		Serial.println(espSettingsBox->password);

		//WiFi.disconnect(0);
		WiFi.mode(WIFI_STA);

		setupSTAevents();

		if(espSettingsBox->staticIp){
			WiFi.config(espSettingsBox->localIp,espSettingsBox->gateIp,
							espSettingsBox->subnetIp,espSettingsBox->dnsIp,espSettingsBox->dnsIp2);
		}else{
			WiFi.config(0U,0U,0U,espSettingsBox->dnsIp,espSettingsBox->dnsIp2);
		}

		WiFi.begin ( const_cast<char*>(espSettingsBox->ssid.c_str()),
				const_cast<char*>(espSettingsBox->password.c_str()) );

		connectToWiFiIfNotConnected();

		return true;
	}
//------------------------------------------------------------------------------
	void setupApEvents(){
		//WiFi.onSoftAPModeStationConnected(&onApConnected);
		//WiFi.onSoftAPModeStationDisconnected(&onApDisconnected);
	}

	void onApConnected(const WiFiEventSoftAPModeStationConnected& evt) {
	  Serial.print(FPSTR("AP connected: MAC="));
	  Serial.println(macToString(evt.mac));

	}

	void onApDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) {
	  Serial.print(FPSTR("AP disconnected: "));
	  Serial.println(macToString(evt.mac));
	}
//------------------------------------------------------------------------------
	void setupSTAevents(){
		//WiFi.onStationModeConnected(&onStationModeConnected);
		//WiFi.onStationModeDisconnected(&onStationModeDisconnected);
		//WiFi.onStationModeDHCPTimeout(&onStationModeDHCPTimeout);
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
//------------------------------------------------------------------------------
	String macToString(const unsigned char* mac) {
	  char buf[20];
	  snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
	           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	  return String(buf);
	}
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
	boolean isWiFIConnected(){
		return WiFi.status() == WL_CONNECTED;
	}

	void connectToWiFiIfNotConnected(){
		uint8_t count=0;
		displayDetails();

		wl_status_t oldstatus=WiFi.status();

		while(!isWiFIConnected() ){
			if(count>=NOT_CONNECT_ITERATIONS || oldstatus!=WiFi.status()){
				displayDetails();
				count=0;
			}else{
				Serial.print(FPSTR("."));
				delay(RE_CONNECT_DELAY);
				count++;
			}

			oldstatus=WiFi.status();
		}
		Serial.println();
		displayDetails();
	}

	void displayDetails(){
		delay(1);
		Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_DIAGNOSTIC));
		printHeap();
#ifdef ESP8266
		WiFi.printDiag(Serial);
#endif
		wl_status_t status=WiFi.status();

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_STATUS_EQ));
		Serial.print(FPSTR(" ("));
		Serial.print(status);
		Serial.print(FPSTR(")  ="));
		if(status>5 || status<0){
			Serial.println(FPSTR(WL_UNKNOWN_STATUS));
		}else{
			Serial.println(FPSTR(WIFIHELPER_WIFI_STATUSES[status]));
		}

		Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_MODE));
		Serial.println(FPSTR(WIFIHELPER_WIFI_MODES[WiFi.getMode()]));

		Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_PHYMODE));
		Serial.println(FPSTR(WIFIHELPER_PHY_MODES[WiFi.getPhyMode()]));

		Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_CHANNEL));
		Serial.println(WiFi.channel());

		Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_SlEEPMODE));
		Serial.println(FPSTR(WIFIHELPER_SlEEP_MODES[WiFi.getSleepMode()]));

		Serial.println(FPSTR(MESSAGE_WIFIHELPER_WIFI_AUTOCONNECT));
		Serial.println(WiFi.getAutoConnect());

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_SSSID_EQ));
		Serial.println ( WiFi.SSID() );

		Serial.print(FPSTR(MESSAGE_WIFIHELPER_WIFI_IP_TYPE_EQ));

		if(espSettingsBox->staticIp){
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

	void checkAuthentication(){
		if(espSettingsBox->settingsUser.length()!=0 && espSettingsBox->settingsPass.length()!=0){
			//Serial.println("Authentication is REQUIRED for setupPage");
			if(!server->authenticate(const_cast<char*>(espSettingsBox->settingsUser.c_str()),
				const_cast<char*>(espSettingsBox->settingsPass.c_str()))){
				return server->requestAuthentication();
			}else{
				//Serial.println("User entered correct User/pass");
			}
		}else{
			//Serial.println("Authentication is not required for setupPage");
		}
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
	//-------------------------------
	String executeGetRequest(String url){
		if(url==""){
			return "";
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

		yield();

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
		//fileManager section
		void initDefaultFiles(){

			Serial.println(FPSTR(MESSAGE_WIFIHELPER_DEPLOY_FILEMANAGER_EDIT));
			/*#ifdef ESP8266
				Dir dir = SPIFFS.openDir("/");
				while (dir.next()) {
				  String fileName = dir.fileName();
				  size_t fileSize = dir.fileSize();
				  //Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
				}
			#else
				listDir(SPIFFS, "/", 0);
			#endif
*/
			server->on("/list", HTTP_GET, [this](){handleFileList();});
			//load editor
			server->on(FPSTR(URL_EDIT), HTTP_GET, [this](){
				if(!handleFileRead(FPSTR(MESSAGE_WIFIHELPER_EDIT_HTML_PAGE))) server->send(404, FPSTR(CONTENT_TYPE_TEXT_PLAIN), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_FILE_NOT_FOUND));
			});
			server->on(FPSTR(URL_INDEX), HTTP_GET, [this](){
				if(!handleFileRead(FPSTR(MESSAGE_WIFIHELPER_INDEX_HTML_PAGE))) server->send(404, FPSTR(CONTENT_TYPE_TEXT_PLAIN), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_FILE_NOT_FOUND));
			});
			server->on(FPSTR(URL_SETTINGS), HTTP_GET, [this](){
				if(!handleFileRead(FPSTR(MESSAGE_WIFIHELPER_SETTINGS_HTML_PAGE))) server->send(404, FPSTR(CONTENT_TYPE_TEXT_PLAIN), FPSTR(MESSAGE_WIFIHELPER_HTTP_STATUS_FILE_NOT_FOUND));
			});
			//create file
			//server->on("/edit", HTTP_PUT, [this](){handleFileCreate();});
			//delete file
			server->on(FPSTR(URL_EDIT), HTTP_DELETE, [this](){handleFileDelete();});
			//first callback is called after the request has ended with all parsed arguments
			//second callback handles file uploads at that location
			server->on(FPSTR(URL_EDIT), HTTP_POST, [this](){ server->send(200, FPSTR(CONTENT_TYPE_TEXT_PLAIN), ""); }, [this](){handleFileUpload();});

			  //get heap status, analog input value and all GPIO statuses in one json call
			/*  server->on("/all", HTTP_GET, [this](){
			    String json = "{";
			    json += "\"heap\":"+String(ESP.getFreeHeap());
			    json += ", \"analog\":"+String(analogRead(A0));
			#ifdef ESP8266
			    json += ", \"gpio\":"+String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
			#endif
			    json += "}";
			    server->send(200, "text/json", json);
			    json = String();
			  });
				*/
			  Serial.println(FPSTR(MESSAGE_WIFIHELPER_HTTP_SERVER_STARTED));
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
		  if(server->hasArg("download")) return FPSTR(CONTENT_TYPE_APPLICATION_OCTED_STREAM);
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

		bool handleFileRead(String path){
		  Serial.println("handleFileRead: " + path);
		  if(path.endsWith("/")) path += "index.htm";
		  String contentType = getContentType(path);
		  String pathWithGz = path + ".gz";
		  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
		    if(SPIFFS.exists(pathWithGz))
		      path += ".gz";
		    File file = SPIFFS.open(path, "r");
		    //size_t sent =
		    server->streamFile(file, contentType);
		    file.close();
		    return true;
		  }
		  return false;
		}

		void handleFileUpload(){
		  if(server->uri() != FPSTR(URL_EDIT)) return;

		  HTTPUpload& upload = server->upload();

		  if(upload.status == UPLOAD_FILE_START){
		    String filename = upload.filename;
		    if(!filename.startsWith("/")) filename = "/"+filename;
		    Serial.print("handleFileUpload Name: "); Serial.println(filename);
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
		  if(server->args() == 0) return server->send(500, "text/plain", "BAD ARGS");
		  String path = server->arg(0);
		  Serial.println("handleFileDelete: " + path);
		  if(path == "/")
		    return server->send(500, "text/plain", "BAD PATH");
		  if(!SPIFFS.exists(path))
		    return server->send(404, "text/plain", "FileNotFound");
		  SPIFFS.remove(path);
		  server->send(200, "text/plain", "");
		  path = String();
		}

		void handleFileCreate(){
		  if(server->args() == 0)
		    return server->send(500, "text/plain", "BAD ARGS");
		  String path = server->arg(0);
		  Serial.println("handleFileCreate: " + path);
		  if(path == "/")
		    return server->send(500, "text/plain", "BAD PATH");
		  if(SPIFFS.exists(path))
		    return server->send(500, "text/plain", "FILE EXISTS");
		  File file = SPIFFS.open(path, "w");
		  if(file)
		    file.close();
		  else
		    return server->send(500, "text/plain", "CREATE FAILED");
		  server->send(200, "text/plain", "");
		  path = String();
		}

		void returnFail(String msg) {
		  server->send(500, "text/plain", msg + "\r\n");
		}

		#ifdef ESP8266
		void handleFileList() {
		  if(!server->hasArg("dir")) {
		    returnFail("BAD ARGS Dir is missed in params");
		    return;
		  }

		  String path = server->arg("dir");
		  Serial.println("handleFileList: " + path);
		  Dir dir = SPIFFS.openDir(path);
		  path = String();

		  String output = "[";
		  while(dir.next()){
		    File entry = dir.openFile("r");
		    if (output != "[") output += ',';
		    bool isDir = false;
		    output += "{\"type\":\"";
		    output += (isDir)?"dir":"file";
		    output += "\",\"name\":\"";
		    output += String(entry.name()).substring(1);
		    output += "\"}";
		    entry.close();
		  }

		  output += "]";
		  server->send(200, "text/json", output);
		}
		#else
		void handleFileList() {
		  if(!server->hasArg("dir")) {
		    returnFail("BAD ARGS");
		    return;
		  }
		  String path = server->arg("dir");
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
		    output += (entry.isDirectory()) ? "dir" : "file";
		    output += "\",\"name\":\"";
		    // Ignore '/' prefix
		    output += entry.name()+1;
		    output += "\"";
		    output += "}";
		    entry.close();
		  }
		  output += "]";
		  server->send(200, "text/json", output);
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

		//end fileManager

		boolean isAP(){
			return startedAsAP;
		}

		IPAddress getIp(){
			if(espSettingsBox->isAccesPoint){
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

private:
#ifdef ESP8266
	ESP8266WebServer *server;
#endif
#ifdef ESP32
	WebServer *server;
#endif
	//String name;
	EspSettingsBox *espSettingsBox;
	DisplayHelper *displayHelper;
	//PinDigital *signalPin;
	std::function<void(void)> serverPostInitFunc;
	boolean disconnectOnStartIfConnected;

	//std::function<void(void)> handleHttpWidget;
	//std::function<PinEvent(PinEvent)> processPinEvent;

	boolean initStaticPages;
	boolean initFilesManager;

	File fsUploadFile;
	HTTPClient http;

	boolean startedAsAP=false;

};


#endif /* LIBRARIES_TIMETRIGGER_WIFIHELPER_H_ */
