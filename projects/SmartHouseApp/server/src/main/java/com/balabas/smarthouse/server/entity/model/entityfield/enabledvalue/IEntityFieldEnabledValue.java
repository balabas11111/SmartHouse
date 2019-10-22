package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

import com.balabas.smarthouse.server.entity.model.EntityField;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;

@SuppressWarnings("rawtypes")
public interface IEntityFieldEnabledValue<T> {

	EntityField getEntityField();
	void setEntityField(EntityField entityField);
	
	T getValue();
	void setValue(T value);
	
	Emoji getEmoji();
	void setEmoji(Emoji emoji);
	
	String getDescription();
	void setDescription(String description);
	
	String getActionDescription();
	void setActionDescription(String actionDescription);
	
	void setValueStr(String value);
	
	EntityFieldClassView getViewClass();
	void setViewClass(EntityFieldClassView viewClass);
}
