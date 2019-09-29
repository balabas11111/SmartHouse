package com.balabas.smarthouse.telegram.bot.renderers;

import org.telegram.telegrambots.meta.api.methods.send.SendMessage;

import com.balabas.smarthouse.server.model.EntityClass;
import com.balabas.smarthouse.server.model.ValueContainer;

public interface EntityRenderer<T extends ValueContainer> {

	EntityClass getType();
	
	default boolean isType(EntityClass r) {
		return getType().equals(r);
	}
	
	SendMessage render(T item, Long chatId);
}
