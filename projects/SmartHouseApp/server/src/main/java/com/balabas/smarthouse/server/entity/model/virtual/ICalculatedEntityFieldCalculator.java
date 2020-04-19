package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Collection;

import com.balabas.smarthouse.server.entity.model.IDescriptionable;
import com.balabas.smarthouse.server.entity.model.INameable;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;

@SuppressWarnings("rawtypes")
public interface ICalculatedEntityFieldCalculator extends INameable, IDescriptionable, IParameterDescriptable {

	IEntityField calculate(IEntityField targetEntityField, Collection<IEntityField> sourceEntityFields, String parameter);

	default String getName() {
		return this.getClass().getSimpleName();
	}
	default void setName(String name) {	}
	
	default String getParameterDescription() {
		return "";
	}
	default void setDescription(String description) {	}

	IEntityFieldValue calculateField(IEntityField targetEntityField, Collection<IEntityFieldValue> values, String parameter);
}
