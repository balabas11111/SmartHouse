package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

public interface IItemAbstract {

	Long getId();
	void setId(Long id);
	
	String getName();
	void setName(String name);
	
	String getDescription();
	void setDescription(String description);
	
	Emoji getEmoji();
	void setEmoji(Emoji emoji);
	
}
