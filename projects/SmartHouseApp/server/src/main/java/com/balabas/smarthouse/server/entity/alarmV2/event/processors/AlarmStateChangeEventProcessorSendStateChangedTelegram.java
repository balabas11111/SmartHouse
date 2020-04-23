package com.balabas.smarthouse.server.entity.alarmV2.event.processors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.telegram.bot.service.BotService;

import lombok.Getter;

@Component
public class AlarmStateChangeEventProcessorSendStateChangedTelegram extends AlarmStateChangeEventProcessor {

	@Getter
	private final String processorDescription = "Отправить сообщение о изменении статуса в телеграм";
	
	@Autowired
	private BotService bot;
	
	@Override
	public void process(IItemEvent event) {
		alarmStateService.putSingleStateDescription(event);
		
		bot.sendHtmlMessageToAllUsers(event.getAlarm().getViewDescriptor().getHint());
	}

}
