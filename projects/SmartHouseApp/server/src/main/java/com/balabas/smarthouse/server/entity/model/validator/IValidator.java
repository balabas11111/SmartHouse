package com.balabas.smarthouse.server.entity.model.validator;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IValidator {

	boolean isTarget(IItemAbstract item);
	
	boolean validate(IItemAbstract item);
}
