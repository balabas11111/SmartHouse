package com.balabas.smarthouse.server.entity.service;

import java.util.Collection;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.view.Action;

@SuppressWarnings("rawtypes")
public interface IEntityFieldService {

	Float getMinAllowedDiff();
	
	boolean isValueChanged(Float val1, Float val2);
	
	List<Action> getActionsForEntityField(String actionName, IEntityField entityField);
	
	List<IEntityFieldEnabledValue> getCommandsForEntityField(IEntityField entityField);

	boolean isButton(IEntityField entityField);

	void save(IEntityFieldValue entityFieldValue);
	
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

	Action getActionForEntityFieldBoolean(String actionName, IEntityField<Boolean> entityField,
			boolean requiredState);

	List<IEntityFieldValue> getEntityFieldValuesByDateRange(IEntityField entityField, Date date1, Date date2);
	
	Boolean isEntityFieldValuesListGrows(List<IEntityFieldValue> values);

	void insertEntityFieldIncorrectValue(IEntityField entityField, String value);

	List<IEntityFieldValue> getEntityFieldValuesLessThanDates(IEntityField entityField, Date date1, Date date2);

	IEntityFieldValue getEntityFieldValuesLessThanDate(IEntityField entityField, Date date);

	Map<String, IEntityFieldValue> getNearestValueToDate(Set<IEntityField> entityFields, long current);

	Date getOldestEntityFieldValue(IEntityField entityField);

	long saveIfDifferent(IEntityFieldValue value);

}
