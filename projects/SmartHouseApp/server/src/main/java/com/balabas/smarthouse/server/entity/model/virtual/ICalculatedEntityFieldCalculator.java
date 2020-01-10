package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Collection;

import com.balabas.smarthouse.server.entity.model.IDescriptionable;
import com.balabas.smarthouse.server.entity.model.INameable;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface ICalculatedEntityFieldCalculator extends INameable, IDescriptionable {

	IEntityField calculate(IEntityField targetEntityField, Collection<IEntityField> sourceEntityFields);

	default void setName(String name) {	}
	default void setDescription(String description) {	}
}
