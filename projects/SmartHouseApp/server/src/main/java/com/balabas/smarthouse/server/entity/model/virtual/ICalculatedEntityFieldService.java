package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.List;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface ICalculatedEntityFieldService {
	
	void apply(IEntityField sourceEntityField);
	
	List<ICalculatedEntityField> getCalculatedEntityFieldsBySourceField(IEntityField sourceEntityField);

	List<ICalculatedEntityField> getCalculatedEntityFields();

	ICalculatedEntityField getCalculatedEntityField(Long id);

	Set<ICalculatedEntityFieldCalculator> getCalculators();
	
	Set<IEntityField> getAllSourceFields();

	Set<IEntityField> getAllTargetFields();

	ICalculatedEntityFieldCalculator getCalculator(String name);

	ICalculatedEntityField createNewCalculatedEntityField(String name, String description);

	void save(ICalculatedEntityField calcEntityField);

	ICalculatedEntityField getCalculatedEntityFieldById(Long id);
}
