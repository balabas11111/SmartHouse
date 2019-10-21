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

}
