package com.balabas.smarthouse.telegram.bot.message;

public class BotMessageConstants {

	public static final String BOT_REGISTERED_MSG = "%s \"%s\" запущен в %tT";
	
	public static final String DEVICE_REGISTERED_MSG = "%s <b>Зарегистрировано устройство </b> \" %s \" время %tT";
	
	public static final String DONT_UNDERSTAND_MSG = "%s Я не понял что имелось ввиду:  %s";
	
	public static final String MESSAGE_SERVER_ERROR = "%s Ошибка сервера.  %s";
	public static final String MESSAGE_DEVICE_ERROR = "%s Ошибка устройства.  %s";
	
	public static final String MESSAGE_DEVICE_REFRESHED = "%s Устройство обновлено.  %s";
	public static final String MESSAGE_DATA_WAS_SENT_TO_DEVICE = "%s Данные отправлены на устройство.  %s";
	
	public static final String BUTTON = "%s %s";
	
	public static final String REFRESH_BUTTON = "%s Список устройств";
	
	public static final String HELLO_MSG = "%s Привет %s, набери %s для просмотра устройств";
	
	public static final String UNAUTHORIZED_MSG = "%s No Access = No success. Suck, man";
	
	public static final String GROUP_DISPLAY_HEADER_MSG = "%s <b>%s</b> \n %s %s \n\n";
	public static final String SELECT_DEVICE_MSG = "%s Выберите устройство";
	public static final String SERVER_SELECT_DEVICE_MSG = "%s Сервер %s";
	public static final String NO_DEVICE_MSG = "%s На сервере не зарегистрировано ни одно устройство";
	public static final String SELECT_GROUP_MSG = 
	        "%s Устройство \n"+
	        "<b>: %s </b>\n"+
	        ": %s ";
	public static final String SPACE = " ";
	public static final String USER = "Пользователь";
	public static final String SELECTED = "Выбрано";
	public static final String NOT_FOUND = "Не найдено";
	
	public static final String COMMAND_HOME = "/home";
	
	public static final String TOOGLE_STATE = "сост";
	public static final String TOOGLE_ON = "Включено";
	public static final String TOOGLE_OF = "Отключено";
	
	private BotMessageConstants(){}
}
