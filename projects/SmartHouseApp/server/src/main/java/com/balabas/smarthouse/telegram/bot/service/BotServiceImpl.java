package com.balabas.smarthouse.telegram.bot.service;

import static java.lang.String.format;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.stream.Stream;

import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.telegram.telegrambots.meta.TelegramBotsApi;
import org.telegram.telegrambots.meta.generics.BotSession;

import com.balabas.smarthouse.server.notification.Notification;
import com.balabas.smarthouse.telegram.bot.AfterBotRegistration;
import com.balabas.smarthouse.telegram.bot.handler.SmartHouseBotHandler;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class BotServiceImpl implements BotService, InitializingBean {

	@Value("${telegram.bot.enabled}")
	private boolean botEnabled;
	
	@Getter
	private TelegramBotsApi api;

	@Getter
	@Autowired
	private SmartHouseBotHandler bot;

	@Autowired
	AuthService authService;

	@Override
	public void afterPropertiesSet() throws Exception {
		if(!botEnabled) {
			log.warn("Telegram bot is disabled");
			return;
		}
		this.api = new TelegramBotsApi();

		BotSession session = api.registerBot(bot);
		handleAfterRegistrationHook(bot, session);
	}

	@Override
	public void sendHtmlMessageToAllUsers(String text) {
		if(!botEnabled) {
			log.warn("Telegram DISABLED :"+text);
			return;
		}
		bot.sendHtmlMessageToAllUsers(text);
	}

	@Override
	public void sendTextMessageToAllUsers(String text) {
		if(!botEnabled) {
			log.warn("Telegram DISABLED :"+text);
			return;
		}
		bot.sendTextMessageToAllUsers(text);

	}

	private void handleAfterRegistrationHook(Object bot, BotSession botSession) {
		Stream.of(bot.getClass().getMethods())
				.filter(method -> method.getAnnotation(AfterBotRegistration.class) != null)
				.forEach(method -> handleAnnotatedMethod(bot, method, botSession));

	}

	private void handleAnnotatedMethod(Object bot, Method method, BotSession session) {
		try {
			if (method.getParameterCount() > 1) {
				log.warn(format("Method %s of Type %s has too many parameters", method.getName(),
						method.getDeclaringClass().getCanonicalName()));
				return;
			}
			if (method.getParameterCount() == 0) {
				method.invoke(bot);
				return;
			}
			if (method.getParameterTypes()[0].equals(BotSession.class)) {
				method.invoke(bot, session);
				return;
			}
			log.warn(format("Method %s of Type %s has invalid parameter type", method.getName(),
					method.getDeclaringClass().getCanonicalName()));
		} catch (InvocationTargetException | IllegalAccessException e) {
			log.error(format("Couldn't invoke Method %s of Type %s", method.getName(),
					method.getDeclaringClass().getCanonicalName()));
		}
	}
	
	@SuppressWarnings("rawtypes")
	@Override
	public void sendNotification(Notification notification) {
		if(!botEnabled) {
			log.warn("Telegram DISABLED :"+notification);
			return;
		}
		bot.sendNotificationToAllUsers(notification);
	}
	
}
