package com.balabas.smarthouse.server.entity.model.collectors;

import java.util.Set;

import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Transient;

import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.exception.BadValueException;

import lombok.Getter;
import lombok.Setter;

@Entity
@SuppressWarnings("rawtypes")
public class DataCollector extends ItemAbstract implements IDataCollector {

	@Getter
	@Setter
	@ManyToOne(fetch = FetchType.EAGER, targetEntity = EntityField.class)
    @JoinColumn(name="entity_id", nullable=false)
	private IEntityField entityField;

	@Getter
	@Setter
	@OneToMany(targetEntity = EntityField.class, fetch = FetchType.EAGER)
	private Set<IEntityField> entityFields;

	@Getter
	@Setter
	private String transformerName;
	
	@Transient
	@Getter
	@Setter
	private IDataTransformer transformer;

	@Override
	public boolean isImpacted(IEntityField field) {
		IEntityField res = entityFields.stream().filter(f -> field.getId().equals(f.getId())).findFirst().orElse(null);
		return res!=null;
	}

	@Override
	public void apply(IEntityField field) throws BadValueException {
		boolean applied = false;
		
		for(IEntityField f : entityFields) {
			if(f.getId().equals(field.getId())) {
				f.setValueStr(field.getValueStr());
				applied = true;
			}
		}
		
		if(applied && getTransformer()!=null) {
			getTransformer().transform(getEntityField(), getEntityFields());
		}
	}

}
