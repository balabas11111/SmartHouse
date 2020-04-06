package com.balabas.smarthouse.server.entity.processors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.virtual.IVirtualEntityService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Component
@Log4j2
public class BoilerPumpsAccumulator implements IEntityAbstractChangeBoxProcessor<IEntityHeater, IEntity> {

	@Getter
	private final String defaultParameter = "{\"" + IEntityHeater.MAX_BOILER + "\":24, \"" + IEntityHeater.MAX_OUT_TEMP + "\":40}";
	
	@Getter
	@Autowired
	protected IVirtualEntityService virtualEntityService;
	
	@Autowired
	protected IDeviceManageService deviceService;
	
	@Override
	public IEntityHeater buildChangeBox(IEntity entity) {
		return (IEntityHeater)(new IEntityHeater()).build(this, entity);
	}

	@Override
	public boolean executeChange(IEntityHeater heater) {
		boolean result = true;
		//TODO: move this method into box
		if (heater.isChange()) {
			result = changeEntityFieldState(heater.getPumpBoiler(), heater.isBoilerPumpRequiredState());
			result = changeEntityFieldState(heater.getPumpFloor(), heater.isFloorPumpRequiredState()) && result;
		}

		return result;
	}


	public boolean changeEntityFieldState(IEntityField<Boolean> field, boolean requiredState) {
		if(requiredState != field.getValue()) {
			log.info("Change pump state name =" + field.getName() + " value = " + requiredState);
			//TODO: implement me and move to default method
		}
		
		return true;
	}

}
