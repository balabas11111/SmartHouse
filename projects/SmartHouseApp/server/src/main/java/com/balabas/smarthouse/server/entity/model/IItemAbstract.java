package com.balabas.smarthouse.server.entity.model;

import java.util.Optional;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

public interface IItemAbstract extends IIdentifiable, INameable{

	String getDescription();
	void setDescription(String description);
	
	String getDescriptionByDescriptionField();
	
	Emoji getEmoji();
	Emoji getEmojiOrDefault(Emoji defaultEmoji);
	
	void setEmoji(Emoji emoji);
	
	String getItemClassId();
	default String getItemId() {
		return getItemClassId() + Long.toString(Optional.ofNullable(getId()).orElse(0L)) + getName();
	}
	
	default void setDescriptionIfEmpty(String description) {
		if (StringUtils.isEmpty(getDescription())) {
			setDescription(description);
		}
	}
	String getEmojiDescriptionByDescriptionField();
	String getEmojiDescriptionByDescriptionFieldWithParent();
	
}
