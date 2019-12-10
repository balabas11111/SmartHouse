package com.balabas.smarthouse.server.entity.control;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

public interface IEntityFieldPredicate extends IAbstractPredicate{

	IEntityField<?> getEntityField();
	void setEntityField(IEntityField<?> entityField);
	
	Object setValue();
}
