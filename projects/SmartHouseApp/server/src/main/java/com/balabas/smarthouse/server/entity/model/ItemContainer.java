package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
public abstract class ItemContainer<T extends IItemAbstract> extends ItemAbstract implements IItemContainer<T> {

	private Set<T> children;
	protected String parentName;

	T getChildByName(String childName) {
		return this.children.stream().filter(c -> c.getName().equals(childName)).findFirst().orElse(null);
	}

	boolean hasChildWithName(String childName) {
		return getChildByName(childName) != null;
	}
}
