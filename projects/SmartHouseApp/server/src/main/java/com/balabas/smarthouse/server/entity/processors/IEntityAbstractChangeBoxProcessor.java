package com.balabas.smarthouse.server.entity.processors;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.virtual.IVirtualEntityService;

@SuppressWarnings("rawtypes")
public interface IEntityAbstractChangeBoxProcessor<T extends IEntityAbstractChangeBox, S extends IItemAbstract> extends ItemAbstractProcessor<S>, IEntityAbstractProvider  {

	String getDefaultParameter();
	
	IVirtualEntityService getVirtualEntityService();
	
	T buildChangeBox(S item);
	
	boolean executeChange(T box);
	
	default T buildChangeBox(S item, String parameter) {
		JSONObject params = getParams(parameter);

		T h = buildChangeBox(item);
		h.setParams(params);

		return h;
	}
	
	default JSONObject getParams(String parameter) {
		if (parameter == null || parameter.isEmpty()) {
			parameter = getDefaultParameter();
		}

		return new JSONObject(parameter);
	}
	
	default boolean check(S item, String parameter) {
		JSONObject params = getParams(parameter);

		T h = buildChangeBox(item);
		h.setParams(params);
		return h.check();
	}
	
	default void process(S item, String parameter) {
		T h = buildChangeBox(item, parameter);
		h.check();
		
		executeChange(h);
	}
	
	default IEntity getEntity(IEntity entity, ItemAbstractDto item) {
		return getVirtualEntityService().getVirtualEntityOrCreateNew(entity.getGroup(), item);
	}

	default IEntityField getFieldBoolean(IEntity entity, ItemAbstractDto item) {
		return getVirtualEntityService().getVirtualEntityFieldBooleanOrCreateNew(entity, item);
	}

	default IEntityField getFieldFloat(IEntity entity, ItemAbstractDto item) {
		return getVirtualEntityService().getVirtualEntityFieldFloatOrCreateNew(entity, item);
	}
}
