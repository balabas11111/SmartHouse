package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

public interface IEntityFieldEnabledValue<T> {

	T getValue();
	Emoji getEmoji();
	String getDescription();
	String getActionDescription();
}
