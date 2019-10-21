package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.ToString;

@ToString
@AllArgsConstructor
public class EntityFieldEnabledValue<T> implements IEntityFieldEnabledValue<T> {

	@Getter
	private T value;
	
	@Getter
	private String description;
	
	@Getter
	private String actionDescription;
	
	@Getter
	private Emoji emoji;
	
}
