package com.balabas.smarthouse.server.view;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Container;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.util.SortingUtility;
import com.google.common.collect.Lists;

import lombok.Getter;
import lombok.Setter;

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
	
	@Getter @Setter
	private AlarmV2Container alarmContainer;
	
	public AlarmV2Container getAlarms() {
		return getAlarmContainer();
	}
	
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
		entityFieldsCalculated = SortingUtility.sort(entityFieldsCalculated);
		entityFields = SortingUtility.sort(entityFields);
		entityFieldsImageable = SortingUtility.sort(entityFieldsImageable);
		//entityFieldActions = sort(entityFieldActions);
	}
	
}
