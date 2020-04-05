package com.balabas.smarthouse.server.entity.behaviour;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface ItemAbstractProcessor <T extends IItemAbstract>{

	boolean process(T item, String parameter);
}
