package com.balabas.smarthouse.server.entity.model;

public interface IEntityFieldCommandButton {

	IEntity getEntity();
	
	IEntityField<?> getEntityField();
	
	String getButtonText();
	
	String getActionCallback();
	
}
