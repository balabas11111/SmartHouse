package com.balabas.smarthouse.server.entity.model.descriptor;

import lombok.experimental.UtilityClass;

@UtilityClass
public class EntityClassConstants {

	public static final String EMPTY_STR = "";
	
	public static final String TRUE = "true";
	public static final String FALSE = "false";
	
	public static final String HIGH = "1";
	public static final String LOW = "0";
	
	public static final String ENTITY_CLASS_PERSISTOR              = "ecP";
	public static final String ENTITY_CLASS_EDITORS                = "ecE";
	public static final String ENTITY_CLASS_SUBSTANCE              = "ecS";
	public static final String ENTITY_CLASS_LEVEL                  = "ecL";
	public static final String ENTITY_CLASS_VIEV                   = "ecV";
	public static final String ENTITY_CLASS_TYPE                   = "ecCT";
	public static final String ENTITY_CLASS_PUSH_VALUE_FROM_SERVER = "ecPu";
	public static final String ENTITY_FIELD_TIME_TO_LIVE           = "ttl";
	
	//Entity default fields
	public static final String ENTITY_FIELD_ENTITY_MANAGER_ID = "mId";
	public static final String ENTITY_FIELD_DESCRIPTION       = "de";
	public static final String ENTITY_FIELD_MEASURE           = "me";
	public static final String ENTITY_FIELD_SENSOR_ITEMS      = "si";
	
	public static final String ENTITY_FIELD_GROUP       = "group";
	public static final String ENTITY_FIELD_NAME       = "name";
}
