package com.balabas.smarthouse.server.widget;

import javax.persistence.Entity;

import lombok.Getter;
import lombok.Setter;

@Entity
public class ViewWidgetString extends ViewWidget {

	@Getter
	private String renderer = "String";
	
	@Getter @Setter
	private String target;
}
