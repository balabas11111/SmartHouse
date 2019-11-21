package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Date;

@SuppressWarnings("rawtypes")
public interface IEntityFieldIncorrectValue {

	Long getId();
	
	IEntityField getEntityField();
	void setEntityField(IEntityField entityField);
	
	String getValue();
	void setValue(String value);
	
	Date getDate();
	void setDate(Date date);
}
