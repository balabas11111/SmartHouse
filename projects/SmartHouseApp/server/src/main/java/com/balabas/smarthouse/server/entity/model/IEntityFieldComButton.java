package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue.IEntityFieldEnabledValue;

@SuppressWarnings("rawtypes")
public interface IEntityFieldComButton {

	IEntityFieldEnabledValue getEntityFieldEnabledValue();
	
	String getButtonText();
	
	String getActionCallback();
	
}
