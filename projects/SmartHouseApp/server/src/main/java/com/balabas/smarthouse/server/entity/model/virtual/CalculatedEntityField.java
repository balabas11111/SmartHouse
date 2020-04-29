package com.balabas.smarthouse.server.entity.model.virtual;

import java.util.Map;
import java.util.Set;

import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.exception.BadValueException;

import lombok.Getter;
import lombok.Setter;

@Entity
@SuppressWarnings("rawtypes")
public class CalculatedEntityField extends ItemAbstract implements ICalculatedEntityField {

	@Getter
	@Setter
	@ManyToOne(fetch = FetchType.EAGER, targetEntity = EntityField.class)
	@JoinColumn(name = "entity_id", nullable = false)
	private IEntityField targetEntityField;

	@Getter
	@Setter
	@ManyToMany(targetEntity = EntityField.class, fetch = FetchType.EAGER)
	private Set<IEntityField> sourceEntityFields;

	@Getter
	@Setter
	private String calculatorName;
	
	@Getter
	@Setter
	private String parameter;

	@Transient
	@Getter
	private ICalculatedEntityFieldCalculator calculator;

	@Override
	public void setCalculator(ICalculatedEntityFieldCalculator calculator) {
		if (calculator != null) {
			this.calculatorName = calculator.getName();
			this.calculator = calculator;
		}
	}

	@Override
	public boolean isImpacted(IEntityField field) {
		try {
			if(sourceEntityFields==null || field == null || field.getId() == null) {
				return false;
			}
			IEntityField res = sourceEntityFields.stream().filter(f -> field.getId().equals(f.getId())).findFirst().orElse(null);
			return res != null;
		}catch(Exception e) {
			e.printStackTrace();
			return false;
		}
	}

	@Override
	public void apply(IEntityField field) throws BadValueException {
		boolean applied = false;

		for (IEntityField f : sourceEntityFields) {
			if (f.getId().equals(field.getId())) {
				f.setValueStr(field.getValueStr());
				applied = true;
			}
		}

		if (applied && getCalculator() != null) {
			getCalculator().calculate(getTargetEntityField(), getSourceEntityFields(), parameter);
		}
	}
	
	@Override
	public ItemType getItemType() {
		return ItemType.ENTITY_FIELD;
	}

	@Override
	public IEntityFieldValue apply(Map<String, IEntityFieldValue> entFieldMap) {
		return getCalculator().calculateField(getTargetEntityField(), entFieldMap.values(), parameter);
	}

}
