package com.balabas.smarthouse.server.entity.model.validator;

import org.springframework.stereotype.Component;
import org.thymeleaf.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.util.MathUtil;

@SuppressWarnings("rawtypes")
@Component
public class ValidatorDS18D20 implements IValidator {

	@Override
	public boolean isTarget(IItemAbstract item) {
		boolean result = false;
		
		if(ItemType.ENTITY_FIELD.equals(item.getItemType())) {
			IEntityField field = (IEntityField) item;
			
			if(field.getEntity()!=null) {
				IEntity entity = field.getEntity();
				
				if(!StringUtils.isEmpty(entity.getName()) && "ds18d20".equals(entity.getName())) {
					result = true;
				}
			}
		}
		
		return result;
	}
	
	@Override
	public boolean validate(IItemAbstract item) {
		boolean result = true;
		
		if(isTarget(item)) {
			IEntityField field = (IEntityField) item;
			
			try {
				Float flt = MathUtil.getAsFloat(field.getValueTmp());
				result = 85F != flt;
			}catch(Exception e) {}
			
		}
		
		return result;
	}
}
