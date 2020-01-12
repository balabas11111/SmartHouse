package com.balabas.smarthouse.server.entity.model.enabledvalue;

import com.balabas.smarthouse.server.entity.model.IIdentifiable;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;

@SuppressWarnings("rawtypes")
public interface IEntityFieldEnabledValue<T> extends IIdentifiable {

	Long getId();
	
	EntityField getEntityField();
	void setEntityField(EntityField entityField);
	
	T getValue();
	void setValue(T value);
	
	String getValueStr();
	
	Emoji getEmoji();
	void setEmoji(Emoji emoji);
	
	String getDescription();
	void setDescription(String description);
	
	String getActionDescription();
	void setActionDescription(String actionDescription);
	
	void setValueStr(String value);
	
	EntityFieldClassView getViewClass();
	void setViewClass(EntityFieldClassView viewClass);
	String buildDataForCallBack();
}
