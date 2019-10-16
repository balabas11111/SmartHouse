package com.balabas.smarthouse.server.entity.model;

import lombok.Data;

@Data
public class ItemAbstract implements IItemAbstract {
	private Long id = 0L;
	private String name;
	private String description;
	private Emoji emoji;
}
