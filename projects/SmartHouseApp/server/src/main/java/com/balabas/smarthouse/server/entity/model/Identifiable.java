package com.balabas.smarthouse.server.entity.model;

import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.MappedSuperclass;

import lombok.Getter;
import lombok.Setter;

@MappedSuperclass
public class Identifiable implements IIdentifiable {

	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Getter
	@Setter
	private Long id;
}
