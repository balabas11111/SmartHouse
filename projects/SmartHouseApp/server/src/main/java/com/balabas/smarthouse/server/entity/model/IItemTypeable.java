package com.balabas.smarthouse.server.entity.model;

import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;

public interface IItemTypeable {

	ItemType getType();

	void setType(ItemType itemType);

}
