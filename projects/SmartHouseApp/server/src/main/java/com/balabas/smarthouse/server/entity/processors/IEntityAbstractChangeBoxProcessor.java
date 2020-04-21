package com.balabas.smarthouse.server.entity.processors;

import java.util.function.Supplier;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.virtual.IVirtualEntityService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;

@SuppressWarnings("rawtypes")
public interface IEntityAbstractChangeBoxProcessor<T extends IEntityAbstractChangeBox, S extends IItemAbstract> extends ItemAbstractProcessor<S>, IEntityAbstractProvider  {

	Supplier<T> getSupplier();
	
	IVirtualEntityService getVirtualEntityService();
	
	IDeviceManageService getDeviceService();
	
	default boolean check(S item, String parameter) {
		T h = buildChangeBox(item, parameter);
		return h.check();
	}
	
	default void process(S item, String parameter) {
		T h = buildChangeBox(item, parameter);
		h.check();
		
		executeChange(h);
	}
	
	default boolean executeChange(T box) {
		boolean result = true;
		if (box.isChange()) {
			result = box.executeChange(this);
		}

		return result;
	}
	
	default T buildChangeBox(S item, String parameter) {
		T h = getSupplier().get();
		h.build(this, item);
		JSONObject params = getParams(parameter);
		params = params==null ? new JSONObject(h.getDefaultParameter()) : params;
		h.setParams(params);

		return h;
	}
	
	default JSONObject getParams(String parameter) {
		if (parameter == null || parameter.isEmpty()) {
			return null;
		}

		return new JSONObject(parameter);
	}
	
	default IEntity getEntity(IEntity entity, ItemAbstractDto item) {
		if( entity == null ) {
			return null;
		}
		Group group = entity.getGroup();
		return getVirtualEntityService().getVirtualEntityOrCreateNew(group, item);
	}
	
	default IEntityField getFieldBoolean(IEntity entity, ItemAbstractDto item, ItemAbstractDto enabledValueTrue,
			ItemAbstractDto enabledValueFalse) {
		return getVirtualEntityService().getVirtualEntityFieldBooleanOrCreateNew(entity, item, enabledValueTrue, enabledValueFalse);
	}

	@SuppressWarnings("unchecked")
	default IEntityField getFieldBoolean(IEntity entity, ItemAbstractDto item) {
		return getVirtualEntityService().getVirtualEntityFieldBooleanOrCreateNew(entity, item);
	}

	default IEntityField getFieldFloat(IEntity entity, ItemAbstractDto item) {
		return getVirtualEntityService().getVirtualEntityFieldFloatOrCreateNew(entity, item);
	}
	
	default boolean changeEntityFieldState(IEntityField<Boolean> field, boolean requiredState) {
		if(requiredState != field.getValue()) {
			return getDeviceService().setEntityFieldBooleanValueSendToDeviceIfNotVirtual(field, requiredState);
		}
		
		return true;
	}

}
