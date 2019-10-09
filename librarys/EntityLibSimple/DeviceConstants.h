/*
 * NetConstants.h
 *
 *  Created on: Sep 20, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_WIFI_NETCONSTANTS_H_
#define LIBRARIES_ENTITYLIBSIMPLE_WIFI_NETCONSTANTS_H_

#include "Arduino.h"

//device statuses
#define DEVICE_STATUS_INITIALIZING "Initializing"
#define DEVICE_STATUS_LOAD_ENTITIES "Loading Entities"
#define DEVICE_STATUS_CONNECTING "Connecting WiFi"
#define DEVICE_STATUS_DISCONNECTED "DisConnected"
#define DEVICE_STATUS_CONNECTED "Connected"
#define DEVICE_STATUS_DEPLOYING "Deploy urls"
#define DEVICE_STATUS_REGISTERING "Registering"
#define DEVICE_STATUS_UPDATE_SENSORS "Update sensors"
#define DEVICE_STATUS_READY "Ready"
#define DEVICE_STATUS_RESTARTING "Restart"

// default json keys for values in request context
#define GROUP "group"
#define NAME "name"
#define PARAM "param"
#define DATA "data"
#define JSON "json"
#define BODY "body"

#define EMPTY_LINE ""

class DeviceConstants {
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_WIFI_NETCONSTANTS_H_ */
