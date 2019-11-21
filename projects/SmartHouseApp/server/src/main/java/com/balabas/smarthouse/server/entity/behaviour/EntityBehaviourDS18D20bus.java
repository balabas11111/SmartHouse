package com.balabas.smarthouse.server.entity.behaviour;

import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Component
public class EntityBehaviourDS18D20bus implements IEntityBehaviour {

	private final String DS18D20_NAME = "ds18d20";
	private final Integer DS18D20_BAD_VALUE_1 = -127;
	private final Integer DS18D20_BAD_VALUE_2 = 85;

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

			return isValueCorrect(val.intValue());
		} catch (Exception e) {
			log.error(e);
			return false;
		}
	}

	@Override
	public boolean isValueCorrect(String value) {
		try {
			return isValueCorrect(Float.valueOf(value).intValue());
			
		} catch(Exception e) {
			log.error(e);
			return false;
		}
	}
	
	private boolean isValueCorrect(Integer value) {
		return value!=null && !DS18D20_BAD_VALUE_1.equals(value) && !DS18D20_BAD_VALUE_2.equals(value);
	}

}
