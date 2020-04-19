package com.balabas.smarthouse.server.entity.processors;

import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.Getter;
import lombok.Setter;

@SuppressWarnings("rawtypes")
public class IEntityHeater extends IEntityAbstractChangeBox {

	public static final ItemAbstractDto boilerEntityDto = new ItemAbstractDto(Emoji.BATTERY, "heatAccu",
			"Теплоакамулятор");

	public static final ItemAbstractDto heatAccuTopDto = new ItemAbstractDto(Emoji.THERMOMETER, "heatAccuTop",
			"1. Верх бака");
	public static final ItemAbstractDto heatAccuMiddleDto = new ItemAbstractDto(Emoji.THERMOMETER, "heatAccuMiddle",
			"2. Середина бака");
	public static final ItemAbstractDto heatAccuDownDto = new ItemAbstractDto(Emoji.THERMOMETER, "heatAccuDown",
			"3. Низ бака");

	public static final ItemAbstractDto boilerOutTempDto = new ItemAbstractDto(Emoji.THERMOMETER, "boilerOutTemp",
			"4. Температура дымохода");

	public static final ItemAbstractDto pumpsEntityDto = new ItemAbstractDto(Emoji.BULB, "pumps", "Насосы отопления");

	public static final ItemAbstractDto pumpBoilerDto = new ItemAbstractDto(Emoji.BULB, "pumpBoiler", "Насос котел");
	public static final ItemAbstractDto pumpElectroBoilerDto = new ItemAbstractDto(Emoji.BULB, "pumpElectroBoiler",
			"Насос електробойлер");
	public static final ItemAbstractDto pumpFloorDto = new ItemAbstractDto(Emoji.BULB, "pumpFloor", "Насос пол");

	public static final ItemAbstractDto fireringDto = new ItemAbstractDto(Emoji.FIRE, "firering", "Розжиг ");
	
	public static final ItemAbstractDto fireringDtoTrue = new ItemAbstractDto(Emoji.FIRE, "Начать розжиг", "Розжиг ВКЛ");
	public static final ItemAbstractDto fireringDtoFalse = new ItemAbstractDto(Emoji.PHONE_OFF, "Стоп розжиг", "Розжиг ВЫКЛ");

	public static final String MAX_BOILER = "maxB";
	public static final String MAX_OUT_TEMP = "maxO";

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
	private boolean floorPumpRequiredState = false;

	@Getter
	@Setter
	private boolean boilerPumpRequiredState = false;

	@Getter
	private final String defaultParameter = "{\"" + MAX_BOILER + "\":31, \"" + MAX_OUT_TEMP + "\":40}";
	
	@SuppressWarnings("unchecked")
	@Override
	public IEntityAbstractChangeBox build(IEntityAbstractProvider p, IItemAbstract item) {
		IEntity entity = (IEntity) item;
		
		IEntity heatAccu = p.getEntity(entity, boilerEntityDto);
		IEntity pumps = p.getEntity(entity, pumpsEntityDto);

		setHeatAccuEntity(heatAccu);
		setPumpEntity(pumps);

		setHeatAccuTop(p.getFieldFloat(heatAccu, heatAccuTopDto));
		setHeatAccuMiddle(p.getFieldFloat(heatAccu, heatAccuMiddleDto));
		setHeatAccuDown(p.getFieldFloat(heatAccu, heatAccuDownDto));
		setBoilerOutTemp(p.getFieldFloat(heatAccu, boilerOutTempDto));

		setPumpBoiler(p.getFieldBoolean(pumps, pumpBoilerDto));
		setPumpElectroBoiler(p.getFieldBoolean(pumps, pumpElectroBoilerDto));
		setPumpFloor(p.getFieldBoolean(pumps, pumpFloorDto));

		setFireingInProgress(p.getFieldBoolean(pumps, fireringDto, fireringDtoTrue, fireringDtoFalse));
		
		return this;
	}
	
	@Override
	public boolean check() {
		boolean result = false;
		int max = getParams().getInt(IEntityHeater.MAX_BOILER);

		boolean hasPowerInAccu = heatAccuTop.getValueAsFloat() > max || heatAccuMiddle.getValueAsFloat() > max
				|| heatAccuDown.getValueAsFloat() > max;

		boolean isHeating = boilerOutTemp.getValueAsFloat() > getParams().getInt(IEntityHeater.MAX_OUT_TEMP);

		boolean floorPumpIsOn = getValOrFalse(pumpFloor);
		boolean boilerPumpIsOn = getValOrFalse(pumpBoiler);

		boolean isFireringInProgress = getValOrFalse(fireingInProgress);

		boolean floorPumpRequiredState = isHeating || isFireringInProgress || hasPowerInAccu;
		boolean boilerPumpRequiredState = isHeating || isFireringInProgress;

		result = floorPumpIsOn != floorPumpRequiredState || boilerPumpIsOn != boilerPumpRequiredState;

		setChange(result);

		setBoilerPumpRequiredState(boilerPumpRequiredState);
		setFloorPumpRequiredState(floorPumpRequiredState);

		return result;
	}

	@Override
	protected boolean executeChange(IEntityAbstractProvider p) {
		boolean result = true;
		//TODO: move this method into box
		if (isChange()) {
			result = p.changeEntityFieldState(getPumpBoiler(), isBoilerPumpRequiredState());
			result = p.changeEntityFieldState(getPumpFloor(), isFloorPumpRequiredState()) && result;
		}

		return result;
	}

}
