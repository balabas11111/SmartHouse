package com.balabas.smarthouse.server.view;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.google.common.collect.Lists;

import lombok.Getter;

@SuppressWarnings("rawtypes")
public class DeviceEntityFieldActionHolder {
	
	@Getter
	private Map<String, List<IEntityField>> entityFieldsCalculated = new LinkedHashMap<>();
	
	@Getter
	private Map<String, List<IEntityField>> entityFields = new LinkedHashMap<>();
	
	@Getter
	private Map<String, List<IEntityField>> entityFieldsImageable = new LinkedHashMap<>();
	
	@Getter
	private Map<String, List<Action>> entityFieldActions = new LinkedHashMap<>();
	
	public void addFieldValue(IEntityField entityField) {
		String entityName = entityField.getEntity().getName();
		
		Map<String, List<IEntityField>> targetMap = 
				!entityField.isButton()?entityFields:entityFieldsImageable;
		
		if(!targetMap.containsKey(entityName)) {
			targetMap.put(entityName, Lists.newArrayList());
		}
		
		targetMap.get(entityName).add(entityField);
	}
	
	public void addFieldAction(String entityName, List<Action> actions) {
		
		if(!entityFieldActions.containsKey(entityName)) {
			entityFieldActions.put(entityName, Lists.newArrayList());
		}
		
		entityFieldActions.get(entityName).addAll(actions);
	}
	
	public List<Action> getActionByName(String actionName) {
		List<Action> result = getEntityFieldActions().get(actionName);
		
		return result;
	}
	
	public void sort() {
		entityFieldsCalculated = IDeviceManageService.sort(entityFieldsCalculated);
		entityFields = IDeviceManageService.sort(entityFields);
		entityFieldsImageable = IDeviceManageService.sort(entityFieldsImageable);
		//entityFieldActions = sort(entityFieldActions);
	}
	
}
