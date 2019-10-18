package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.fasterxml.jackson.annotation.JsonIgnore;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
public abstract class ItemContainer<T extends IItemAbstract> extends ItemAbstract implements IItemContainer<T> {

	@JsonIgnore
	protected Set<T> children;
	@JsonIgnore
	protected String parentName;

	@Override
	public T getChild(String childName) {
		return this.children.stream().filter(c -> c.getName().equals(childName)).findFirst().orElse(null);
	}

	@Override
	public boolean hasChild(String childName) {
		return getChild(childName) != null;
	}
}
