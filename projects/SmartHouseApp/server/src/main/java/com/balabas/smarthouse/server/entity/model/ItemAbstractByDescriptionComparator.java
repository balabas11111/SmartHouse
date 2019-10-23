package com.balabas.smarthouse.server.entity.model;

import java.util.Comparator;

public class ItemAbstractByDescriptionComparator implements Comparator<ItemAbstract> {

	@Override
	public int compare(ItemAbstract o1, ItemAbstract o2) {
		if (o1 == null && o2 == null) {
			return 0;
		}

		String d1 = o1.getDescription();
		String d2 = o2.getDescription();

		if (d1 == null && d2 == null) {
			return 0;
		}

		if (d1 != null && d2 != null) {
			return d1.compareToIgnoreCase(d2);
		}
		if (d2 != null) {
			return -1;
		}
		if(d1!=null) {
			return 1;
		}

		return 0;
	}

}
