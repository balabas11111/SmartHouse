package com.balabas.smarthouse.server.entity.model;

import java.util.Comparator;

public class ItemAbstractByDescriptionComparator implements Comparator<ItemAbstract> {

	@Override
	public int compare(ItemAbstract o1, ItemAbstract o2) {
		return ItemAbstract.compareByDescription(o1, o2);
	}

}
