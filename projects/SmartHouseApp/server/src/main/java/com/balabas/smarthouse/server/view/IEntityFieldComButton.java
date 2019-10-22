package com.balabas.smarthouse.server.view;

import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;

@SuppressWarnings("rawtypes")
public interface IEntityFieldComButton {

	IEntityFieldEnabledValue getEntityFieldEnabledValue();
	
	String getButtonText();
	
	String getActionCallback();
	
}
