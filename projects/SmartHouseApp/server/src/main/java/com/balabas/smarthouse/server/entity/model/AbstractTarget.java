package com.balabas.smarthouse.server.entity.model;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

@AllArgsConstructor
public class AbstractTarget {

	@Getter @Setter
	private Long id;
	
	@Getter @Setter
	private String name;
	
	@Getter @Setter
	private String description;
	
	public boolean isIdEqual(Long otherId) {
		return id.equals(otherId);
	}
}
