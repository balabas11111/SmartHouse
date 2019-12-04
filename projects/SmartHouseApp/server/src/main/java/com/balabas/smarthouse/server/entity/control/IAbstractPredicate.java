package com.balabas.smarthouse.server.entity.control;

import com.balabas.smarthouse.server.entity.model.IIdentifiable;

public interface IAbstractPredicate extends IIdentifiable {

	PredicatePreffix getPreffix();
	boolean test();
}
