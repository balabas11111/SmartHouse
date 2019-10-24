package com.balabas.smarthouse.server.entity.model.descriptor;

import lombok.experimental.UtilityClass;

@UtilityClass
public class EntityClassConstants {

	public static final String EMPTY_STR = "";
	
	public static final String TRUE = "true";
	public static final String FALSE = "false";
	
	public static final String HIGH = "1";
	public static final String LOW = "0";
	
	public static final String EDC_ENTITY_FIELDS          = "f";
	public static final String EDC_FIELD_ENABLED_VALUES   = "val";
	
	public static final String EDC_READ_ONLY              = "ro";
	public static final String EDC_TIME_TO_LIVE           = "ttl";
	
	public static final String EDC_SENDS_DATA_TO_TOPIC    = "sdt";
	public static final String EDC_READS_DATA_FROM_TOPIC  = "rdt";
	
	public static final String EDC_CLASS                  = "c";
	public static final String EDC_CLASS_VIEW             = "cv";
	
	public static final String EDC_FIELD_ID               = "i";
	public static final String EDC_FIELD_NAME             = "n";
	public static final String EDC_FIELD_DESCRIPTION      = "d";
	public static final String EDC_FIELD_EMOJI            = "img";
	public static final String EDC_FIELD_DESCR_FIELD      = "df";
	public static final String EDC_FIELD_ACTION_DESCR     = "ad";
	
	public static final String EDC_FIELD_BOOLEAN_VALUE_ON  = "true";
	public static final String EDC_FIELD_BOOLEAN_VALUE_OFF = "false";

	public static final String EDC_FIELD_BOOLEAN_VALUE_ON_DESCR      = "Включить";
	public static final String EDC_FIELD_BOOLEAN_VALUE_OFF_DESCR     = "Отключить";
	public static final String EDC_FIELD_BOOLEAN_VALUE_RESTART_DESCR = "Перезапустить";
	public static final String EDC_FIELD_COUNT_DESCR                 = "Количество";
	
	
	public static final String ENTITY_CLASS_PERSISTOR              = "ecP";
	public static final String ENTITY_CLASS_EDITORS                = "ecE";
	public static final String ENTITY_CLASS_SUBSTANCE              = "ecS";
	public static final String ENTITY_CLASS_LEVEL                  = "ecL";
	
	//Entity default fields
	public static final String ENTITY_FIELD_ENTITY_MANAGER_ID = "mId";
	public static final String ENTITY_FIELD_MEASURE           = "me";
	public static final String ENTITY_FIELD_SENSOR_ITEMS      = "si";
	
	public static final String ENTITY_FIELD_GROUP       = "group";
	public static final String ENTITY_FIELD_NAME       = "name";
}
