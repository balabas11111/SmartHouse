package com.balabas.smarthouse.server.view;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import com.balabas.smarthouse.server.entity.model.IEntityFieldValue;
import com.google.common.collect.Lists;

import lombok.Getter;

public class DeviceValueActionHolder {

	@Getter
	private Map<String, List<IEntityFieldValue>> entityFieldValues = new LinkedHashMap<>();
	
	@Getter
	private Map<String, List<Action>> entityFieldActions = new LinkedHashMap<>();
	
	public void addFieldValue(IEntityFieldValue entityFieldValue) {
		String entityName = entityFieldValue.getEntityField().getEntity().getName();
		
		if(!entityFieldValues.containsKey(entityName)) {
			entityFieldValues.put(entityName, Lists.newArrayList());
		}
		
		entityFieldValues.get(entityName).add(entityFieldValue);
	}
	
	public void addFieldAction(String entityName, List<Action> actions) {
		
		if(!entityFieldActions.containsKey(entityName)) {
			entityFieldActions.put(entityName, Lists.newArrayList());
		}
		
		entityFieldActions.get(entityName).addAll(actions);
	}
}
