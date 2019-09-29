package com.balabas.smarthouse.telegram.bot.renderers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.telegram.telegrambots.meta.api.methods.send.SendMessage;

import com.balabas.smarthouse.server.model.Entity;
import com.balabas.smarthouse.server.model.EntityClass;
import com.balabas.smarthouse.server.model.ModelConstants;
import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.telegram.bot.message.Emoji;
import com.balabas.smarthouse.telegram.bot.message.InlineKeyboardBuilder;
import com.balabas.smarthouse.telegram.bot.message.ReplyContext;

import lombok.Getter;

import static com.balabas.smarthouse.server.util.ObjectUtil.getBooleanValue;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SEND_DATA_TO_DEVICE;

@Component
public class EntityRendererToggleButtons implements EntityRenderer<Entity> {

	public static final String TOOGLE_BUTTON_DATA_ON = "{\"on\":1}";
	public static final String TOOGLE_BUTTON_DATA_OFF = "{\"on\":0}";
	public static final String TOOGLE_BUTTON_STR = "%s %s \n\n состояние %s";
	
	public static final String VALUE_ENTITY_TURN_ON = "Включить";
	public static final String VALUE_ENTITY_TURN_OFF = "Отключить";
	
	public static final String VALUE_ENTITY_STATE_ON = "Вкл";
	public static final String VALUE_ENTITY_STATE_OFF = "Отк";
	
	@Getter
	private EntityClass type = EntityClass.TOGGLE_BUTTON;
	
	@Autowired
	private InlineKeyboardBuilder inlineKeyboardBuilder;

	@Override
	public SendMessage render(Entity item, Long chatId) {
		Boolean on = getBooleanValue(item.getValue(ModelConstants.ENTITY_FIELD_ON));
		String text = String.format(TOOGLE_BUTTON_STR, 
						Emoji.STAR, item.getDescription(),getTextByOnOffValue(on));
		
		String onAction = 
				Action.fromEntity(ACTION_TYPE_SEND_DATA_TO_DEVICE, item, TOOGLE_BUTTON_DATA_ON).getCallbackData();
		String offAction = 
				Action.fromEntity(ACTION_TYPE_SEND_DATA_TO_DEVICE, item, TOOGLE_BUTTON_DATA_OFF).getCallbackData();
		
		return ReplyContext.createMsg(inlineKeyboardBuilder.getToggleOnOffInlineKeyboard(
				text,VALUE_ENTITY_TURN_ON, VALUE_ENTITY_TURN_OFF,
				onAction, offAction),
				null, chatId, text, true);
		
	}
	/*
	private Emoji getEmojiByOnOffValue(Boolean on) {
		return (on == null)?
					Emoji.ERROR:
					(on?Emoji.SUN:Emoji.STOP);
	}
	*/
	private String getTextByOnOffValue(Boolean on) {
		return (on == null)?
					"":
					(on?VALUE_ENTITY_STATE_ON:VALUE_ENTITY_STATE_OFF);
	}

	
}
