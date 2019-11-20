package com.balabas.smarthouse.server.view.chart;

import java.util.Set;

import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.JoinTable;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;

import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.Setter;

@Entity
@EqualsAndHashCode(callSuper = true, exclude = "entityFields")
@SuppressWarnings("rawtypes")
public class ViewChartEntityFields extends ItemAbstract {

	@Getter @Setter
	@ManyToMany(targetEntity = EntityField.class, fetch = FetchType.EAGER)
	@JoinTable(name = "entity_field_chart_view",
    joinColumns = {@JoinColumn(name = "chart_view_id")},
    inverseJoinColumns = {@JoinColumn(name = "entity_field_id")})
	protected Set<IEntityField> entityFields;
	
}
