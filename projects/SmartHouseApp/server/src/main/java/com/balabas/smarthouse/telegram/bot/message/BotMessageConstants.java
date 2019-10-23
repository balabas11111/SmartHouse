package com.balabas.smarthouse.telegram.bot.message;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

import lombok.experimental.UtilityClass;

@UtilityClass
public class BotMessageConstants {
	
	public static final String BOT_MESSAGE_DEFAULT = "%s %s";

	public static final String BOT_TEMPLATES_PATH = "telegram/templates/text/";
	
	public static final String BOT_REGISTERED_MSG = "%s \"%s\" запущен в %tT";
	
	public static final String DEVICE_REGISTERED_MSG = "%s <b>Зарегистрировано устройство </b> \" %s \" время %tT";
	
	public static final String DONT_UNDERSTAND_MSG = "%s Я не понял что имелось ввиду:  %s";
	
	public static final String MESSAGE_SERVER_ERROR = "%s Ошибка сервера.  %s";
	public static final String MESSAGE_DEVICE_ERROR = "%s Ошибка устройства.  %s";
	
	public static final String MESSAGE_DEVICE_REFRESHED = "%s Устройство обновлено.  %s";
	public static final String MESSAGE_DEVICE_DATA_SAVED = "%s Данные сохранены.  %s";
	public static final String MESSAGE_DATA_WAS_SENT_TO_DEVICE = "%s Данные отправлены на устройство.  %s";
	
	public static final String BUTTON = "%s %s";
	
	public static final String VIEW_DEVICES_BUTTON = "%s Просмотр показателей";
	public static final String EDIT_DEVICES_BUTTON = "%s Редактировать";
	
	public static final String EDIT_DEVICE_SELECT_ENTITY = "%s Редактировать %s. Выберите датчик для редактирования";
	public static final String EDIT_DEVICE_SELECT_FIELD = "%s Редактировать %s. Выберите поле для редактирования";
	public static final String EDIT_DEVICE_SELECTED_FIELD = Emoji.MEMO.toString() + 
			" %s\n Редактируем поле (%s) - <b>%s</b>\n Поточное значение %s\n"
			+ "введите новое значение и нажмите кнопку отправить";
	
	public static final String EDIT_DEVICE_SELECT_ENTITY_FIELD_BUTTON = "(%s) Редактировать : %s";
	
	public static final String HELLO_MSG = "%s Привет %s, набери %s для просмотра устройств";
	
	public static final String UNAUTHORIZED_MSG = "%s No Access = No success. Suck, man";
	
	public static final String GROUP_VIEW_HEADER_MSG = "%s <b>%s</b>      %s <code>%s</code> \n";
	public static final String GROUP_COMMAND_HEADER_MSG = Emoji.JOYSTICK.toString() + 
								"<b>Управление</b>      %s <b>%s</b>";
	public static final String ENTITY_DISPLAY_HEADER_MSG = "--------------------\n %s <b>%s</b> :     ";
	
	public static final String SELECT_DEVICE_MSG = "%s Выберите устройство";
	public static final String SERVER_SELECT_DEVICE_VIEW_MSG = "%s Сервер %s";
	public static final String SERVER_SELECT_DEVICE_EDIT_MSG = "%s Сервер %s - Редактировать параметры";
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
	public static final String COMMAND_ROOT = "/";
	public static final String COMMAND_SETUP = "/setup";
	public static final String COMMAND_HELP = "/help";
	
	public static final String TOOGLE_STATE = "сост";
	public static final String TOOGLE_ON = "Включено";
	public static final String TOOGLE_OF = "Отключено";
	
	public static final String HELP_MESSAGE = Emoji.HAND_OK.toString() + " Справка "
			+ COMMAND_SETUP + " - " + "Установки устройства" + " \n"
			+ COMMAND_HOME + " - " + "Датчики / управление устройством" + " \n"
			+ COMMAND_HELP + " - " + "Справка по коммандам" + " \n";
	
}
