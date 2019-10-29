package com.balabas.smarthouse.telegram.bot.message;

import java.util.Optional;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.Getter;

@SuppressWarnings("rawtypes")
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
	
	public ActionContext(IEntity entity) {
		this.emoji = Optional.ofNullable(entity.getEmoji()).orElse(Emoji.PAGER);
		this.description = entity.getDescription();
	}
	
	public ActionContext(IEntityField entityField) {
		String description = entityField.getDescription();
		
		if(StringUtils.isEmpty(entityField.getDescription())
				&& !StringUtils.isEmpty(entityField.getDescriptionField())) {
			
			IEntityField descrField = entityField.getEntity().getEntityField(entityField.getDescriptionField());
			
			if(descrField!=null && descrField.getValue()!=null && !StringUtils.isEmpty(descrField.getValueStr())) {
				description = descrField.getValueStr();
			}
		}
		
		this.emoji = Optional.ofNullable(entityField.getEmoji()).orElse(Emoji.EMPTY_EMOJI);
		this.description = "(" + entityField.getName() + ") " + description;
	}
}
