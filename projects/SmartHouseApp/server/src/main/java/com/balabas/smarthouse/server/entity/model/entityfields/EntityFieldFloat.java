package com.balabas.smarthouse.server.entity.model.entityfields;

import javax.persistence.Entity;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.util.MathUtil;
import com.fasterxml.jackson.annotation.JsonAlias;

@Entity
public class EntityFieldFloat extends EntityField<Float> implements IEntityField<Float> {

	@Override
	public Class<Float> getClazz() {
		return Float.class;
	}
	
	@Transient
	@JsonAlias("value")
	private Float valueF;

	@Override
	protected Float fromString(String value) {
		try {
		return MathUtil.precise(Float.valueOf(value));
		}catch(Exception e) {
			System.out.println(e.getMessage());
			throw e;
		}
	}

	@Override
	public Float getValue() {
		return valueF;
	}

	@Override
	public boolean setValue(Float value) {
		boolean equals = equalsAsValue(this.valueF, value);

		if(!equals) {
			this.valueF = MathUtil.precise(value);
		}
		
		return !equals;
	}

}
