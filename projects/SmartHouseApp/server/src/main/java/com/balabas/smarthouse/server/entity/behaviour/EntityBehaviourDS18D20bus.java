package com.balabas.smarthouse.server.entity.behaviour;

import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;

@Component
public class EntityBehaviourDS18D20bus implements IEntityBehaviour {

	private final String DS18D20_NAME = "ds18d20";

	@Override
	public EntityType getEntityType() {
		return EntityType.DS18D20;
	}

	@Override
	public boolean isTarget(IEntity entity) {
		return entity != null && !StringUtils.isEmpty(entity.getName()) && entity.getName().contains(DS18D20_NAME);
	}

	@Override
	public boolean isValueCorrect(IEntityFieldValue value) {
		if (!Number.class.isAssignableFrom(value.getEntityField().getClazz())) {
			return false;
		}
		try {
			Number val = (Number) value.getValue();

			return val != null && val.intValue() != -127 && val.intValue() != 85;
		} catch (Exception e) {
			return false;
		}
	}

}
