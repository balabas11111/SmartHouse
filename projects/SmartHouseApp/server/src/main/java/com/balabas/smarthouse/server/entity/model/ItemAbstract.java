package com.balabas.smarthouse.server.entity.model;

import javax.persistence.Column;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.MappedSuperclass;

import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;

import lombok.Data;

@Data
@MappedSuperclass
public class ItemAbstract implements IItemAbstract {
	@Id
	@GeneratedValue
	protected Long id = 0L;
	@Column
	protected String name;
	@Column
	protected String description;
	@Column
	protected Emoji emoji;
}
