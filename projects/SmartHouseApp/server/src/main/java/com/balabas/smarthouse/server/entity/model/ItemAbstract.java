package com.balabas.smarthouse.server.entity.model;

import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.MappedSuperclass;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

import lombok.Data;

@Data
@MappedSuperclass
public class ItemAbstract implements IItemAbstract {
	@Id
	@GeneratedValue(strategy = GenerationType.AUTO)
	protected Long id = 0L;
	protected String name;
	protected String description;
	@Enumerated(EnumType.STRING)
	protected Emoji emoji;
}
