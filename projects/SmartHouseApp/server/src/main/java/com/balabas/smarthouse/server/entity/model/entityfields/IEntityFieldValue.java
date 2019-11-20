package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Date;

@SuppressWarnings("rawtypes")
public interface IEntityFieldValue {

	Long getId();
	IEntityField getEntityField();
	Date getDate();

	String getValueStr();
	
	Object getValue();
}
