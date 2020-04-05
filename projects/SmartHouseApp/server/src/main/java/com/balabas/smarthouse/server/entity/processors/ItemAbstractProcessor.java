package com.balabas.smarthouse.server.entity.processors;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface ItemAbstractProcessor <T extends IItemAbstract>{

	boolean check(T item, String parameter);
	
	void process(T item, String parameter);
}
