package com.balabas.smarthouse.server;

import com.balabas.smarthouse.server.entity.model.IDevice;

import lombok.experimental.UtilityClass;

@UtilityClass
public class DeviceMessageConstants {

	public static final String MSG_DEVICE_REGISTERED = "Устройство регистрируется : %s";
	public static final String MSG_DEVICE_RECONNECTED = "Устройство передподключено : %s";
	public static final String MSG_DEVICE_BACK_FROM_DISCONNECT = "Устройство снова поставляет данные : %s";
	public static final String MSG_DEVICE_INITIALIZED = "Устройство инициализировано : %s";
	public static final String MSG_DEVICE_DISCONNECTED = "Устройство отключено : %s";
	
	public static final String MSG_DEVICE_ALARM_STARTED = "Устройство : <b> %s </b>. РЕЖИМ ТРЕВОГИ";
	public static final String MSG_DEVICE_ALARM_FINISHED = "Устройство : %s. Режим тревоги прекращен";
	public static final String MSG_ENTITY_ALARM_FINISHED = "Датчик : %s - в норме ";
	
	public static String buildMessage(String format, Object...strings) {
		return String.format(format, strings);
	}
	
	public static String buildMessage(String format, IDevice device) {
		return String.format(format, device.getDescription());
	}
}
