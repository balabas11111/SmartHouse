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
	
	//attributes
	public static final String ATTR_ID            = "id";
	public static final String ATTR_SERVER_NAME   = "serverName";
	public static final String ATTR_DATE_BEFORE   = "beforeDate";
	public static final String ATTR_DATE_AFTER    = "afterDate";
	public static final String ATTR_CHART_DATA_Y  = "chartDataY";
	public static final String ATTR_CHART_DATA    = "chartData";
	
	//urls
	public static final String URL_REDIRECT_PREFFIX   = "redirect:";
	
	//urls view chart data
	public static final String URL_ENTITY_FIELD_CHART = "/entityField";
	public static final String URL_LIST_ALL_CHARTS    = "/listChart";
	public static final String URL_EDIT_CHART         = "/editChart";
	public static final String URL_SAVE_CHART         = "/saveView";
	public static final String URL_VIEW_CHART         = "/viewChart";
	
	public static final String URL_REDIRECT_ENTITY_FIELD_FORMAT = "redirect:/entityField?id=%d&afterDate=%d";
	public static final String URL_REDIRECT_VIEW_CHART_FORMAT = "redirect:/viewChart?id=%d&afterDate=%d";
	
	//messages
	public static final String MSG_NEW_CHART_PREFFIX = "Новый график";
	
	public static final String MSG_NEW_DEVICE        = "Новое устройство";
	public static final String MSG_EDIT_DEVICE       = "Редактрирование устройства";
	public static final String MSG_NEW_GROUP         = "Новая группа";
	public static final String MSG_EDIT_GROUP        = "Редактрирование группы";
	public static final String MSG_NEW_ENTITY        = "Новый датчик";
	public static final String MSG_EDIT_ENTITY       = "Редактрирование датчика";
	public static final String MSG_NEW_ENTITY_FIELD  = "Новый показатель";
	public static final String MSG_EDIT_ENTITY_FIELD = "Редактрирование показателя";
	public static final String MSG_NEW_CALC_ENTITY_FIELD  = "Новый вычисляемый показатель";
	public static final String MSG_EDIT_CALC_ENTITY_FIELD = "Редактрирование вычисляемого показателя";
	
	//model properties
	public static final String PROP_PAGE_HEADER      = "pageHeader";
	public static final String PROP_BASE_URL         = "baseUrl";
}
