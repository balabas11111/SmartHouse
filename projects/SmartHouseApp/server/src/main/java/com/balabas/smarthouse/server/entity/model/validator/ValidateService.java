package com.balabas.smarthouse.server.entity.model.validator;

import java.util.Collections;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

@Service
public class ValidateService implements IValidateService {

	@Autowired(required = false)
	private Set<IValidator> validators;

	@Override
	public List<IValidator> getValidators(IItemAbstract item) {
		if (item == null || validators == null || validators.isEmpty()) {
			return Collections.emptyList();
		}
		return validators.stream().filter(v -> v.isTarget(item)).collect(Collectors.toList());

	}

	@Override
	public boolean validate(IItemAbstract item, String valStr) {
		boolean result = true;

		for (IValidator v : getValidators(item)) {
			result = v.validate(item) && result;
		};

		return result;
	}
}
