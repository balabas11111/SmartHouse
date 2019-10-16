package com.balabas.smarthouse.telegram.bot.renderers;

import org.telegram.telegrambots.meta.api.methods.send.SendMessage;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityClass;

public interface EntityRenderer<T extends IEntity> {

	EntityClass getType();
	
	default boolean isType(EntityClass r) {
		return getType().equals(r);
	}
	
	SendMessage render(T item, Long chatId);
}
