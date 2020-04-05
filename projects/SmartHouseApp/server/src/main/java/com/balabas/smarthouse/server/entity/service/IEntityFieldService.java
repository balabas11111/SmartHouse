package com.balabas.smarthouse.server.entity.service;

import java.util.Collection;
import java.util.Date;
import java.util.List;
import java.util.Optional;

import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.view.Action;

@SuppressWarnings("rawtypes")
public interface IEntityFieldService {

	List<Action> getActionsForEntityField(String actionName, IEntityField entityField);
	
	List<IEntityFieldEnabledValue> getCommandsForEntityField(IEntityField entityField);

	boolean isButton(IEntityField entityField);

	void saveAll(List<IEntityFieldValue> values);
	
	void saveAllValues(Collection<IEntityField> values);

	List<EntityFieldValue> getLastEntityFieldValuesForDevice(Long deviceId);

	List<EntityFieldValue> getLastEntityFieldValuesForEntity(Long entityId);

	void deleteEntityFieldValuesForDevice(Long deviceId);

	List<EntityFieldValue> getEntityFieldValuesForEntityField(Long entityFieldId);
	
	List<EntityFieldValue> getEntityFieldValuesForEntityField(Long entityFieldId, Date afterDate);
	
	List<EntityFieldValue> getEntityFieldValuesForEntityField(Long entityFieldId, Date afterDate, Date beforeDate);

	Date selectMinEntityFieldValueDate(Long entityFieldId);

	Date selectMaxEntityFieldValueDate(Long entityFieldId);

	Optional<IEntityField> getEntityFieldById(Long entityFieldId);

	void deleteEntityFieldsForDevice(Long deviceId);

	List<IEntityFieldValue> mapFieldToValue(Collection<IEntityField> changedSources);

	IEntityFieldValue getEntityFieldValue(IEntityField entityField);

}
