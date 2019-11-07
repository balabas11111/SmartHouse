package com.balabas.smarthouse.server.entity.service;

import java.util.Collections;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.Action;

@Service
@SuppressWarnings({"rawtypes", "unchecked"})
public class EntityFieldService implements IEntityFieldService{
	
	@Override
	public List<Action> getActionsForEntityField(String actionName, IEntityField entityField) {
		return getCommandsForEntityField(entityField).stream()
				.map(ef -> Action.fromEntityFieldEnabledValue(actionName, ef)).collect(Collectors.toList());
	}

	@Override
	public List<IEntityFieldEnabledValue> getCommandsForEntityField(IEntityField entityField) {
		if (!Boolean.class.equals(entityField.getClazz()) || entityField.getEnabledValues() == null
				|| entityField.getEnabledValues().isEmpty()) {
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

}
