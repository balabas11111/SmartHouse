package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Map;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IEntityFieldValueChangeDispatcher {

	void dispatchEntityFieldValueChange(IEntityField sourceEntityField, Map<Long, IEntityField> uniqueChangedTargets);
}
