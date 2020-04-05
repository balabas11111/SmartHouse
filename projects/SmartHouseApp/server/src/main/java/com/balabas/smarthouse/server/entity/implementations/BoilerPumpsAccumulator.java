package com.balabas.smarthouse.server.entity.implementations;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.virtual.IVirtualEntityService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;

import lombok.Getter;
import lombok.Setter;
import lombok.extern.log4j.Log4j2;

@Component
@Log4j2
@SuppressWarnings("rawtypes")
public class BoilerPumpsAccumulator {

	private static final ItemAbstractDto boilerEntity = new ItemAbstractDto(Emoji.BATTERY, "heatAccu",
			"Теплоакамулятор");

	private static final ItemAbstractDto heatAccuTop = new ItemAbstractDto(Emoji.THERMOMETER, "heatAccuTop",
			"1. Верх бака");
	private static final ItemAbstractDto heatAccuMiddle = new ItemAbstractDto(Emoji.THERMOMETER, "heatAccuMiddle",
			"2. Середина бака");
	private static final ItemAbstractDto heatAccuDown = new ItemAbstractDto(Emoji.THERMOMETER, "heatAccuDown",
			"3. Низ бака");

	private static final ItemAbstractDto boilerOutTemp = new ItemAbstractDto(Emoji.THERMOMETER, "boilerOutTemp",
			"4. Температура дымохода");

	private static final ItemAbstractDto pumpsEntity = new ItemAbstractDto(Emoji.BULB, "pumps", "Насосы отопления");

	private static final ItemAbstractDto pumpBoiler = new ItemAbstractDto(Emoji.BULB, "pumpBoiler", "Насос котел");
	private static final ItemAbstractDto pumpElectroBoiler = new ItemAbstractDto(Emoji.BULB, "pumpElectroBoiler",
			"Насос електробойлер");
	private static final ItemAbstractDto pumpFloor = new ItemAbstractDto(Emoji.BULB, "pumpFloor", "Насос пол");

	private static final ItemAbstractDto firering = new ItemAbstractDto(Emoji.FIRE, "firering", "Розжиг в процессе");

	private static final String MAX_BOILER = "maxB";
	private static final String MAX_OUT_TEMP = "maxO";

	@Getter
	private final String defaultParameter = "{\"" + MAX_BOILER + "\":24, \"" + MAX_OUT_TEMP + "\":40}";

	public static class IEntityHeater {
		@Getter
		@Setter
		private IEntity heatAccuEntity;

		@Getter
		@Setter
		private IEntityField heatAccuTop;
		@Getter
		@Setter
		private IEntityField heatAccuMiddle;
		@Getter
		@Setter
		private IEntityField heatAccuDown;

		@Getter
		@Setter
		private IEntityField boilerOutTemp;

		@Getter
		@Setter
		private IEntity pumpEntity;

		@Getter
		@Setter
		private IEntityField<Boolean> pumpBoiler;
		@Getter
		@Setter
		private IEntityField<Boolean> pumpElectroBoiler;
		@Getter
		@Setter
		private IEntityField<Boolean> pumpFloor;

		@Getter
		@Setter
		private IEntityField<Boolean> fireingInProgress;;

		@Getter
		@Setter
		private boolean stateChangeRequired = false;
		
		@Getter
		@Setter
		private boolean floorPumpRequiredState = false;
		
		@Getter
		@Setter
		private boolean boilerPumpRequiredState = false;
		
		@Getter
		@Setter
		private JSONObject params;
	}

	@Autowired
	protected IVirtualEntityService virtualEntityService;
	
	@Autowired
	protected IDeviceManageService deviceService;
	
	public IEntityHeater checkHeater(IEntity item, String parameter) {
		JSONObject params = getParams(parameter);

		IEntityHeater h = getHeaterOrCreateNewOnEntityLevel(item);
		h.setParams(params);
		checkStateChangeRequired(h);

		return h;
	}
	
	public boolean executeStateChange(IEntityHeater heater) {
		boolean result = true;

		if (heater.isStateChangeRequired()) {
			result = changeEntityFieldState(heater.getPumpBoiler(), heater.isBoilerPumpRequiredState());
			result = changeEntityFieldState(heater.getPumpFloor(), heater.isFloorPumpRequiredState()) && result;
		}

		return result;
	}
	
	private JSONObject getParams(String parameter) {
		if (parameter == null || parameter.isEmpty()) {
			parameter = getDefaultParameter();
		}

		return new JSONObject(parameter);
	}
	
	@SuppressWarnings({ "unchecked" })
	private IEntityHeater getHeaterOrCreateNewOnEntityLevel(IEntity entity) {
		IEntityHeater h = new IEntityHeater();

		IEntity heatAccu = getEntity(entity, boilerEntity);
		IEntity pumps = getEntity(entity, pumpsEntity);

		h.setHeatAccuEntity(heatAccu);
		h.setPumpEntity(pumps);

		h.setHeatAccuTop(getFieldFloat(heatAccu, heatAccuTop));
		h.setHeatAccuMiddle(getFieldFloat(heatAccu, heatAccuMiddle));
		h.setHeatAccuDown(getFieldFloat(heatAccu, heatAccuDown));
		h.setBoilerOutTemp(getFieldFloat(heatAccu, boilerOutTemp));

		h.setPumpBoiler(getFieldBoolean(pumps, pumpBoiler));
		h.setPumpElectroBoiler(getFieldBoolean(pumps, pumpElectroBoiler));
		h.setPumpFloor(getFieldBoolean(pumps, pumpFloor));

		h.setFireingInProgress(getFieldBoolean(pumps, firering));

		return h;
	}

	private boolean checkStateChangeRequired(IEntityHeater heater) {
		boolean result = false;
		int max = heater.getParams().getInt(MAX_BOILER);

		boolean hasPowerInAccu = heater.getHeatAccuTop().getValueAsFloat() > max
				|| heater.getHeatAccuMiddle().getValueAsFloat() > max
				|| heater.getHeatAccuDown().getValueAsFloat() > max;

		boolean isHeating = heater.getBoilerOutTemp().getValueAsFloat() > heater.getParams().getInt(MAX_OUT_TEMP);

		boolean floorPumpIsOn = heater.getPumpFloor().getValue();
		boolean boilerPumpIsOn = heater.getPumpBoiler().getValue();

		boolean isFireringInProgress = heater.getFireingInProgress().getValue();

		boolean floorPumpRequiredState = isHeating || isFireringInProgress || hasPowerInAccu;
		boolean boilerPumpRequiredState = isHeating || isFireringInProgress;

		result = floorPumpIsOn != floorPumpRequiredState || boilerPumpIsOn != boilerPumpRequiredState;

		heater.setStateChangeRequired(result);
		
		heater.setBoilerPumpRequiredState(boilerPumpRequiredState);
		heater.setFloorPumpRequiredState(floorPumpRequiredState);
		
		if(result) {
			log.info("Pump state change required");
		}
		
		return result;
	}

	public boolean changeEntityFieldState(IEntityField<Boolean> field, boolean requiredState) {
		if(requiredState != field.getValue()) {
			log.info("Change pump state name =" + field.getName() + " value = " + requiredState);
			//TODO: implement me
		}
		
		return true;
	}

	protected IEntity getEntity(IEntity entity, ItemAbstractDto item) {
		return virtualEntityService.getVirtualEntityOrCreateNew(entity.getGroup(), item);
	}

	protected IEntityField getFieldBoolean(IEntity entity, ItemAbstractDto item) {
		return virtualEntityService.getVirtualEntityFieldBooleanOrCreateNew(entity, item);
	}

	protected IEntityField getFieldFloat(IEntity entity, ItemAbstractDto item) {
		return virtualEntityService.getVirtualEntityFieldFloatOrCreateNew(entity, item);
	}

}
