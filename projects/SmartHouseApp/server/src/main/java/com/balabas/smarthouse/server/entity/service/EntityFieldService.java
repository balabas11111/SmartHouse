package com.balabas.smarthouse.server.entity.service;

import java.util.Collections;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.IEntityFieldValueRepository;
import com.balabas.smarthouse.server.view.Action;

import lombok.extern.log4j.Log4j2;

@Service
@SuppressWarnings({ "rawtypes", "unchecked" })
@Log4j2
public class EntityFieldService implements IEntityFieldService {

	@Autowired
	IEntityFieldValueRepository entityFieldValueRepository;

	@Override
	public void saveAll(List<EntityFieldValue> values) {
		// entityFieldValueRepository.saveAll(values);

		for (EntityFieldValue entityFieldValue : values) {
			if (entityFieldValue.getEntityField() != null && entityFieldValue.getEntityField().getId() != null) {
				try {
					entityFieldValueRepository.saveEntityFieldValue(entityFieldValue);
				} catch (Exception e) {
					log.error(e);
				}
			} else {
				log.error("Error save entityFieldValue id  is null name = "
						+ Optional.ofNullable(entityFieldValue.getEntityField().getName()).orElse("null"));
			}
		}
	}

	@Override
	public List<Action> getActionsForEntityField(String actionName, IEntityField entityField) {
		return getCommandsForEntityField(entityField).stream()
				.map(ef -> Action.fromEntityFieldEnabledValue(actionName, ef)).collect(Collectors.toList());
	}

	@Override
	public List<IEntityFieldEnabledValue> getCommandsForEntityField(IEntityField entityField) {
		if (!Boolean.class.equals(entityField.getClazz()) || entityField.getEnabledValues() == null
				|| entityField.getEnabledValues().isEmpty() || entityField.isReadOnly()) {
			return Collections.emptyList();
		}

		IEntityFieldEnabledValue currentValue = entityField.getEntityFieldEnabledValueByCurrentValue();

		Set<IEntityFieldEnabledValue> evals = entityField.getEnabledValues();

		List<IEntityFieldEnabledValue> result = evals.stream()
				.filter(ev -> ev.getViewClass() != null && ev.getViewClass().isButton()
						&& (currentValue == null || (currentValue != null && !ev.equals(currentValue))))
				.collect(Collectors.toList());

		return result;
	}

	@Override
	public boolean isButton(IEntityField entityField) {
		IEntityFieldEnabledValue enVal = entityField.getEntityFieldEnabledValueByCurrentValue();

		return enVal != null && enVal.getViewClass() != null && Boolean.class.equals(entityField.getClazz())
				&& entityField.getViewClass().isButton();
	}

	@Override
	public List<EntityFieldValue> getLastEntityFieldValuesForDevice(Long deviceId) {
		return entityFieldValueRepository.getLastEntityFieldValuesForDevice(deviceId);
	}

	@Override
	public List<EntityFieldValue> getLastEntityFieldValuesForEntity(Long entityId) {
		return entityFieldValueRepository.getLastEntityFieldValuesForEntity(entityId);
	}

	@Override
	public void deleteEntityFieldValuesForDevice(Long deviceId) {
		entityFieldValueRepository.deleteEntityFieldValuesForDevice(deviceId);
	}

}
