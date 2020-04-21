package com.balabas.smarthouse.server.entity.model.validator;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IValidateService {

	boolean validate(IItemAbstract item, String valStr);

	List<IValidator> getValidators(IItemAbstract item);
}
