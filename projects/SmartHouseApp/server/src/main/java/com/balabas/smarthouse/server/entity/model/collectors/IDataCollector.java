package com.balabas.smarthouse.server.entity.model.collectors;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.exception.BadValueException;

@SuppressWarnings("rawtypes")
public interface IDataCollector extends IItemAbstract {

	
	IEntityField getEntityField();
	void setEntityField(IEntityField entityField);
	
	Set<IEntityField> getEntityFields();
	void setEntityFields(Set<IEntityField> entityFields);
	
	String getTransformerName();
	void setTransformerName(String transformerName);
	
	IDataTransformer getTransformer();
	void setTransformer(IDataTransformer transformer);
	
	boolean isImpacted(IEntityField field);
	
	void apply(IEntityField field) throws BadValueException;
}
