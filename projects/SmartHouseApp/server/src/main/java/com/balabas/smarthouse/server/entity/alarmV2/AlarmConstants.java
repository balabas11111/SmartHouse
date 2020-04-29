package com.balabas.smarthouse.server.entity.alarmV2;

public class AlarmConstants {

	public static final String DEFAULT_DESCRIPTION = "Тревога изменилась с %s на %s";
	
	public static final String CHECKER_DESCRIPTION_DEVICE_DISCONNECTED = "Проверка подключения Устройства";
	
	public static final String CHECKER_DESCRIPTION_DS18D20_DISCONNECTED = "Проверка подключения DS18D20";
	public static final String CHECKER_DESCRIPTION_ENTITY_FIELD_TOO_HIGH = "Значение поля > максимального";
	public static final String CHECKER_DESCRIPTION_ENTITY_FIELD_TOO_LOW = "Значение поля < минимального";
	public static final String CHECKER_DESCRIPTION_ENTITY_FIELD_TOO_LOW_WARN = "Значение поля < минимального";
	
	public static final String CHECKER_DESCRIPTION_ENTITY_FIELD_GROWS = "Значение поля растет или падает";
	
	public static final String CHECKER_DESCRIPTION_ENTITY_FIELD_INT_VALUE = "Значение поля {\"ok\":0, \"warning\":1, \"alarm\":2}";
	public static final String CHECKER_DESCRIPTION_ENTITY_FIELD_INT_VALUE_DEFAULT = "{\"ok\":0, \"warning\":1, \"alarm\":2}";
	
	public static final String CHECKER_DESCRIPTION_HEATER_ENTITY = "Проверка системы отопления";
}
