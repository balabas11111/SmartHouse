package com.balabas.smarthouse.telegram.bot.message;

import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;

@Component
public class ItemRendererBuilder {

	public void renderEntityByFieldDescriptors(IEntity entity, StringBuilder builder) {
		// TODO Auto-generated method stub
	}
	
	public SendMessage buildGroupCommandInterface(IGroup entity, Long chatId) {
		// TODO Auto-generated method stub
		
		return null;
	}
}
