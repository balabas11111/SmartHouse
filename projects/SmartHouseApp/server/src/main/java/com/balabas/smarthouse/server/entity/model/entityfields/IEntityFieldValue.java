package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Date;

@SuppressWarnings("rawtypes")
public interface IEntityFieldValue extends IFloatProvider  {

	Long getId();
	IEntityField getEntityField();
	Date getDate();

	String getValueStr();
	
	Object getValue();
}
