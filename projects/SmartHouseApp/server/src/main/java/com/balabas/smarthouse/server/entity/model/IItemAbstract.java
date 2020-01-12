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
	}
	
	default void setDescriptionIfEmpty(String description) {
		if (StringUtils.isEmpty(getDescription())) {
			setDescription(description);
		}
	}
	default boolean equalsAsValue(Object val1, Object val2) {
		boolean result = false;
		
		if(val1 == null && val2 == null) {
			return true;
		}
		if(val1!=null) {
			return val1.equals(val2);
		}
		if(val2!=null) {
			return val2.equals(val1);
		}
		
		return result;
	}
	
	String getEmojiDescriptionByDescriptionField();
	String getEmojiDescriptionByDescriptionFieldWithParent();
	
	boolean isVirtualized();
	void setVirtualized(Boolean value);
	
	void setParent(IItemAbstract parent);
	
}
