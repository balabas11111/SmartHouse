package com.balabas.smarthouse.server.entity.model;

import lombok.Data;

@Data
public class EntityAbstract implements IEntityAbstract {
	private Long id = 0L;
	private Long parentId = 0L ;
	private Long rootId = 0L;
	private String name;
	private String description;
	private String path;
	private EntityState state = EntityState.NONE;
}
