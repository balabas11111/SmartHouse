package com.balabas.smarthouse.telegram.bot.message;

import java.util.Optional;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.State;

import lombok.Getter;

public class ActionContext {

	@Getter
	private Emoji emoji;
	@Getter
	private String description;
	
	public ActionContext(IDevice device) {
		this.emoji = Optional.ofNullable(device.getEmoji()).orElse(Emoji.PAGER);
		this.description = device.getDescription();
		
		if (!device.isInitialized()) {
    		emoji = Emoji.HOURGLASS;
    		description += " (Не инициализировано)";
    	} else if(State.TIMED_OUT.equals(device.getState())){
    		emoji = Emoji.GHOST;
    		description += " (Не подключено)";
    	} else if(State.FAILED.equals(device.getState())){
    		emoji = Emoji.RED_CIRCLE;
    		description += " (Ошибка данных)";
    	} else if(State.DISCONNECTED.equals(device.getState())){
    		emoji = Emoji.SKULL;
    		description += " (Ошибка подключения)";
    	}
	}
}
