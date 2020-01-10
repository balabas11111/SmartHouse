package com.balabas.smarthouse.server.entity.model;

import java.util.Optional;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

public interface IItemAbstract extends IIdentifiable, INameable, IDescriptionable{

	String getDescriptionByDescriptionField();
	
	Emoji getEmoji();
	Emoji getEmojiOrDefault(Emoji defaultEmoji);
	
	void setEmoji(Emoji emoji);
	
	String getItemClassId();
	default String getItemId() {
		return getItemClassId() + Long.toString(Optional.ofNullable(getId()).orElse(0L)) + getName();
	}
	
	default void setNameDescriptionEmoji(IItemAbstract source) {
		setName(source.getName());
		setDescription(source.getDescription());
		setEmoji(source.getEmoji());
	}
	
	default void setDescriptionIfEmpty(String description) {
		if (StringUtils.isEmpty(getDescription())) {
			setDescription(description);
		}
	}
	String getEmojiDescriptionByDescriptionField();
	String getEmojiDescriptionByDescriptionFieldWithParent();
	
	boolean isVirtualized();
	void setVirtualized(Boolean value);
	
	void setParent(IItemAbstract parent);
	
}
