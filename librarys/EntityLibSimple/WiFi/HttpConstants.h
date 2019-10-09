/*
 * HttpConstants.h
 *
 *  Created on: Oct 9, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_WIFI_HTTPCONSTANTS_H_
#define LIBRARIES_ENTITYLIBSIMPLE_WIFI_HTTPCONSTANTS_H_

#include "Arduino.h"

#define HTTP_PREFFIX  "http://"
//headers
#define HEADER_CONTENT_TYPE           "Content-Type"
#define HEADER_LOCATION               "Location"
#define HEADER_AUTHORIZATION          "Authorization"

//http Root URLs
#define URL_ROOT  "/"
#define URL_INFO  "/info"
#define URL_DATA  "/data"
#define URL_DIR   "/dir"
#define URL_FILES "/files"

#define URL_INDEX "/index"
#define URL_SETUP "/setup"

#define URL_INDEXHTM "/index.htm"
#define URL_SETUPHTM "/setup.htm"

#define URL_INDEXHTM_GZ "/index.htm.gz"
#define URL_SETUPHTM_GZ "/setup.htm.gz"

#define RESPONSE_KEY_Server "Server"
#define RESPONSE_MSG_ServerName "ESP Sync Web Server"

//response keys
#define JSONKEY_files       "files"
#define JSONKEY_heap        "heap"

//default responses
#define RESPONSE_MSG_badArgs           "BAD ARGS"
#define RESPONSE_MSG_badPath           "BAD Path"
#define RESPONSE_MSG_fileNotFound      "FileNotFound"

#define CONTENT_TYPE_APPLICATION_JSON "application/json"
#define CONTENT_TYPE_APPLICATION_FORM_URL_ENCODED "application/x-www-form-urlencoded"

//content types
const PROGMEM char CONTENT_TYPE_TEXT_HTML[] = "text/html";
const PROGMEM char CONTENT_TYPE_JSON[] = "application/json";
const PROGMEM char CONTENT_TYPE_FORM_URL_ENCODED[] = "application/x-www-form-urlencoded";
const PROGMEM char CONTENT_TYPE_TEXT_PLAIN[] = "text/plain";
const PROGMEM char CONTENT_TYPE_APPLICATION_OCTED_STREAM[] = "application/octet-stream";
const PROGMEM char CONTENT_TYPE_TEXT_CSS[] = "text/css";
const PROGMEM char CONTENT_TYPE_TEXT_JSON[] = "text/json";
const PROGMEM char CONTENT_TYPE_APPLICATION_JAVASCRIPT[] = "application/javascript";
const PROGMEM char CONTENT_TYPE_IMAGE_PNG[] = "image/png";
const PROGMEM char CONTENT_TYPE_IMAGE_GIF[] = "image/gif";
const PROGMEM char CONTENT_TYPE_IMAGE_JPEG[] = "image/jpeg";
const PROGMEM char CONTENT_TYPE_IMAGE_XICON[] = "image/x-icon";
const PROGMEM char CONTENT_TYPE_TEXT_XML[] = "text/xml";
const PROGMEM char CONTENT_TYPE_APPLICATION_XPDF[] = "application/x-pdf";
const PROGMEM char CONTENT_TYPE_APPLICATION_XZIP[] = "application/x-zip";
const PROGMEM char CONTENT_TYPE_APPLICATION_XGZIP[] = "application/x-gzip";

const PROGMEM char CONTENT_TYPE_JSON_UTF8[] = "application/json; charset=UTF-8";
const PROGMEM char CONTENT_TYPE_TEXT_JSON_UTF8[] = "text/json; charset=UTF-8";
const PROGMEM char CONTENT_TYPE_TEXT_HTML_UTF8[] = "text/html; charset=UTF-8";
const PROGMEM char CONTENT_TYPE_TEXT_XML_UTF8[] = "text/xml; charset=UTF-8";
const PROGMEM char CONTENT_TYPE_TEXT_PLAIN_UTF8[] = "text/plain; charset=UTF-8";

//Authentication constants
const PROGMEM char AUTH_REALM_ADMIN_MESSAGE[] = "Enter Admin login/password for: ";
const PROGMEM char AUTH_REALM_USER_MESSAGE[] = "Enter User login/password for: ";
// the Content of the HTML response in case of Unautherized Access Default:empty
const PROGMEM char AUTH_FAILED_MESSAGE[] = "<html>403. Authentication Failed</html>";
class HttpConstants {
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_WIFI_HTTPCONSTANTS_H_ */
