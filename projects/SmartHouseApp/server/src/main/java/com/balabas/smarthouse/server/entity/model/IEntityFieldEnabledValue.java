package com.balabas.smarthouse.server.entity.model;

public interface IEntityFieldEnabledValue<T> {

	T getValue();
	Emoji getEmoji();
	String getDescription();
	String getActionDescription();
}
