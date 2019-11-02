package com.balabas.smarthouse.server.entity.model;

import java.util.Date;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IEntityFieldValue {

	Long getId();
	IEntityField getEntityField();
	Date getDate();

	String getValueStr();
	
	Object getValue();
}
