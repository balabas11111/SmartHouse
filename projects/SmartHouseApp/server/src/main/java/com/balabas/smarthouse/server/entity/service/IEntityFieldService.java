package com.balabas.smarthouse.server.entity.service;

import java.util.Date;
import java.util.List;

import com.balabas.smarthouse.server.entity.model.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.Action;

@SuppressWarnings("rawtypes")
public interface IEntityFieldService {

	List<Action> getActionsForEntityField(String actionName, IEntityField entityField);
	
	List<IEntityFieldEnabledValue> getCommandsForEntityField(IEntityField entityField);

	boolean isButton(IEntityField entityField);

	void saveAll(List<EntityFieldValue> values);

	List<EntityFieldValue> getLastEntityFieldValuesForDevice(Long deviceId);

	List<EntityFieldValue> getLastEntityFieldValuesForEntity(Long entityId);

	void deleteEntityFieldValuesForDevice(Long deviceId);

	List<EntityFieldValue> getEntityFieldValuesForEntityField(Long entityFieldId);
	
	List<EntityFieldValue> getEntityFieldValuesForEntityField(Long entityFieldId, Date afterDate);
	
	List<EntityFieldValue> getEntityFieldValuesForEntityField(Long entityFieldId, Date afterDate, Date beforeDate);

	Date selectMinEntityFieldValueDate(Long entityFieldId);

	Date selectMaxEntityFieldValueDate(Long entityFieldId);

}
