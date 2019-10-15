package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.Map;

public interface IEntityFieldDescriptor {

	EntityFieldClassType getEntityClassType();
	EntityFieldClassView getEntityClassView();
	Long getTimeToLive();
	boolean isReadOnly();
	Map<String, IFieldEnabledValue> getEnabledValues();
	Emoji getEmoji();
}
