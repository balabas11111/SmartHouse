package com.balabas.smarthouse.server.controller;

import lombok.experimental.UtilityClass;

@UtilityClass
public class ControllerConstants {

	
	public static final String CROSS_ORIGIN_4200 = "http://localhost:4200";
	public static final String CROSS_ORIGIN_80 = "http://localhost:80";
	public static final String MOCK = "/mock";
	public static final String API_V1 = "/api/v1";
	public static final String DEVICES_ROOT = "/devices";
	
	public static final String DEVICES_ROOT_V1 = API_V1 + DEVICES_ROOT;
	
	public static final String SECURITY_ROOT = "/security";
	
	public static final String TEMPLATE_DEVICES_VIEW = "devices/list.html";
	public static final String TEMPLATE_DEVICE_VIEW = "devices/edit.html";
}
