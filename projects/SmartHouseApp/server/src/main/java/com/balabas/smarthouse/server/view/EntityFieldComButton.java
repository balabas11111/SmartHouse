package com.balabas.smarthouse.server.view;

import java.util.Optional;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.Getter;

@SuppressWarnings("rawtypes")
public class EntityFieldComButton implements IEntityFieldComButton {

	@Getter
	private IEntityFieldEnabledValue entityFieldEnabledValue;
	
	@Getter
	private String actionCallback;
	
	@Getter
	private String buttonText;
	
	public EntityFieldComButton(String actionName, IEntityFieldEnabledValue entityFieldEnabledValue) {
		this.entityFieldEnabledValue = entityFieldEnabledValue;
		
		IEntityField entityField = entityFieldEnabledValue.getEntityField();
		IEntity entity = entityField.getEntity();
		
		String deviceName = entity.getGroup().getDevice().getName();
		String groupName = "";//entity.getGroupName();
		String entityId = Integer.toString(entity.getRemoteId());//entity.getName();
		
		this.actionCallback = new Action(actionName, entityFieldEnabledValue.buildDataForCallBack(),
				deviceName, groupName, entityId).getCallbackData();
		this.buttonText = Optional.ofNullable(entityFieldEnabledValue.getEmoji()).orElse(Emoji.EMPTY_EMOJI).toString()
				+ entity.getDescription() + " : " + entityFieldEnabledValue.getActionDescription();
		
	}
}
