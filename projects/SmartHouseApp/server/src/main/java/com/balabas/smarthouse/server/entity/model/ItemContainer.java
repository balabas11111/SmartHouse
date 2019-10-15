package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
public class ItemContainer<T extends IItemAbstract> extends ItemAbstract implements IItemContainer<T> {

	private Set<T> children;

	T getChildByName(String childName) {
		return this.children.stream().filter(c -> c.getName().equals(childName)).findFirst().orElse(null);
	}

	boolean hasChildWithName(String childName) {
		return getChildByName(childName) != null;
	}
}
