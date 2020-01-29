package com.balabas.smarthouse.server.view.chart;

import java.util.List;
import java.util.Set;

import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.JoinTable;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;

import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.google.common.collect.Lists;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.Setter;

@Entity
@EqualsAndHashCode(callSuper = true, exclude = "entityFields")
@SuppressWarnings("rawtypes")
public class Metric extends ItemAbstract implements IMetrics {

	public static final List<String> LEGEND_POSITION = Lists.newArrayList("top", "right", "left");
	
	@Getter @Setter
	private Boolean entityFieldView;
	
	@Getter @Setter
	private Boolean entityView;
	
	@Getter @Setter
	private Boolean chartView;
	
	@Getter @Setter
	private String legendPosition;
	
	@Getter @Setter
	@ManyToMany(targetEntity = EntityField.class, fetch = FetchType.EAGER)
	@JoinTable(name = "entity_field_chart_view",
    joinColumns = {@JoinColumn(name = "chart_view_id")},
    inverseJoinColumns = {@JoinColumn(name = "entity_field_id")})
	protected Set<IEntityField> entityFields;
	
}
