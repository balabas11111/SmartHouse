package com.balabas.smarthouse.server.entity.model;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

public interface IItemAbstract extends IIdentifiable, INameable{

	String getDescription();
	void setDescription(String description);
	
	Emoji getEmoji();
	void setEmoji(Emoji emoji);
	
	default void setDescriptionIfEmpty(String description) {
		if (StringUtils.isEmpty(getDescription())) {
			setDescription(description);
		}
	}
	
}
