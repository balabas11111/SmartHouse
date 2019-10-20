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
		
		String deviceName = entity.getDeviceName();
		String groupName = "";//entity.getGroupName();
		String entityId = Integer.toString(entity.getRemoteId());//entity.getName();
		
		this.actionCallback = new Action(actionName, entityField.buildDataForCallBack(),
				deviceName, groupName, entityId).getCallbackData();
		this.buttonText = Optional.ofNullable(getEntityField().getEmoji()).orElse(Emoji.EMPTY_EMOJI).toString()
				+ entity.getDescription() + " : " + getEntityField().getActionDescription();
		
	}
}
