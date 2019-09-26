package com.balabas.smarthouse.telegram.bot.service;

public class BotMessageConstants {

	public static final String BOT_REGISTERED_MSG = "%s \"%s\" запущен в %tT";
	
	public static final String DEVICE_REGISTERED_MSG = "%s <b>Зарегистрировано устройство </b> \" %s \" время %tT";
	
	public static final String DONT_UNDERSTAND_MSG = "%s - Я не понял че надо  %s";
	
	public static final String BUTTON = "%s %s";
	
	public static final String DEVICE_BUTTON = "%s %s";
	public static final String GROUP_BUTTON = "%s %s";
	
	public static final String HELLO_MSG = "%s Привет %s, набери %s для просмотра устройств";
	
	public static final String UNAUTHORIZED_MSG = "%s No Access = No success. Suck, man";
	
	public static final String SELECT_DEVICE_MSG = "%s Выберите устройство";
	public static final String SELECT_GROUP_MSG = "%s Выберите группу устройства: %s";
	
	public static final String SPACE = " ";
	public static final String USER = "Пользователь";
	public static final String SELECTED = "Выбрано";
	public static final String NOT_FOUND = "Не найдено";
	
	public static final String COMMAND_HOME = "/home";
	
	private BotMessageConstants(){}
}
