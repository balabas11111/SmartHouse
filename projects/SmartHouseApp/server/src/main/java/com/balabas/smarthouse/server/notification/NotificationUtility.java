package com.balabas.smarthouse.server.notification;

import lombok.experimental.UtilityClass;

@UtilityClass
public class NotificationUtility {

	public static final String MSG_PART_SPLITTER = " : ";
	public static final String MSG_DEVICE_REREGISTERED = "Устройство переподключено";
	public static final String MSG_DEVICE_REGISTERED = "Устройство зарегистрировано";
	
	public static final String splitNotificationParts(String...parts) {
		return String.join(MSG_PART_SPLITTER, parts);
	}
	
}
