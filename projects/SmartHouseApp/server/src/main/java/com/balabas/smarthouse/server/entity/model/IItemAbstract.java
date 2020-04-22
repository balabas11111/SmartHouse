package com.balabas.smarthouse.server.entity.model;

import java.util.List;
import java.util.Optional;

import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.descriptor.AbstractItemType;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

public interface IItemAbstract extends IIdentifiable, INameable, IDescriptionable{

	String getDescriptionByDescriptionField();
	
	Emoji getEmoji();
	Emoji getEmojiOrDefault(Emoji defaultEmoji);
	
	void setEmoji(Emoji emoji);
	
	String getItemClassId();
	
	String getEmojiDescriptionByDescriptionField();
	String getEmojiDescriptionByDescriptionFieldWithParent();
	
	boolean isVirtualized();
	void setVirtualized(Boolean value);
	
	void setParent(IItemAbstract parent);
	
	String getParentNamesChain();

	List<IItemAbstract> getAllChildren();
	
	ItemType getItemType();
	
	default AbstractItemType getItemAbstractType() {
		return AbstractItemType.UNKNOWN;
	}
	
	default String getItemUid() {
		StringBuilder builder = new StringBuilder();
		builder.append(getItemClassId());
		builder.append(Long.toString(Optional.ofNullable(getId()).orElse(0L)));
		builder.append(getName());
		return builder.toString();
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
	default List<IItemAbstract> getItemAndAllChildren() {
		List<IItemAbstract> result = this.getAllChildren();
		result.add(0, this);
		
		return result;
	}

	static void setEmojiNameDescription(IItemAbstract item, Emoji emoji, String name, String description) {
		item.setEmoji(emoji);
		item.setName(name);
		item.setDescription(description);
	}
	
	 String getFullName();
	
}
