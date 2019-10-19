package com.balabas.smarthouse.server.entity.model;

import java.util.Optional;

import com.balabas.smarthouse.server.view.Action;

import lombok.Getter;

public class EntityFieldCommandButton implements IEntityFieldCommandButton {

	@Getter
	private IEntity entity;
	
	@Getter
	private IEntityField<?> entityField;
	
	@Getter
	private String actionCallback;
	
	@Getter
	private String buttonText;
	
	public EntityFieldCommandButton(String actionName, IEntity entity, IEntityField<?> entityField) {
		this.entity = entity;
		this.entityField = entityField;
		
		this.actionCallback = new Action(actionName, entityField.buildDataForCallBack(), entity.getDeviceName(), entity.getGroupName(), entity.getName()).getCallbackData();
		this.buttonText = Optional.ofNullable(getEntityField().getEmoji()).orElse(Emoji.EMPTY_EMOJI).toString()
				+ entity.getDescription() + " : " + getEntityField().getActionDescription();
		
	}
}
