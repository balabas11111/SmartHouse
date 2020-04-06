package com.balabas.smarthouse.server.entity.processors;

import java.util.function.Supplier;

import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.entity.model.IEntity;

import lombok.Getter;

@SuppressWarnings("unchecked")
@Component
public class BoilerPumpsAccumulator extends EntityAbstractChangeBoxProcessor<IEntityHeater, IEntity> {

	@Getter
	Supplier<IEntityHeater> supplier = IEntityHeater::new;

}
