package com.balabas.smarthouse.server.entity.alarmV2.event.processors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.server.entity.service.IMessageSender;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Component
public class AlarmStateChangeEventProcessorSendStateChangedTelegram extends AlarmStateChangeEventProcessor {

	@Getter
	private final String processorDescription = "Отправить сообщение о изменении статуса в телеграм";
	
	@Autowired
	private IMessageSender bot;
	
	@Override
	public void process(IItemEvent event) {
		String message = event.getAlarm().getAsSingleString();
		
		bot.sendHtmlMessageToAllUsers(message);
		log.info("sent " + message);
	}

}
