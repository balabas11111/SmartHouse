package com.balabas.smarthouse.server.widget;

import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;

import com.balabas.smarthouse.server.entity.model.Entity;

import lombok.Getter;
import lombok.Setter;

@javax.persistence.Entity
public class ViewWidgetEntity extends ViewWidget {

	@Getter
	private String renderer = "Entity";
	
	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="entity_id", nullable=false)
	@Getter @Setter
	private Entity target;
}
