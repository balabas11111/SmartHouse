package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

import com.balabas.smarthouse.server.entity.model.IEntityField;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;

@SuppressWarnings("rawtypes")
public interface IEntityFieldEnabledValue<T> {

	
	IEntityField getEntityField();
	void setEntityField(IEntityField entityField);
	
	T getValue();
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
