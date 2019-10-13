package com.balabas.smarthouse.server.entity.model;

import java.util.HashSet;
import java.util.Set;

import com.balabas.smarthouse.server.entity.watcher.IWatcherAbstract;

import lombok.Data;

@Data
@SuppressWarnings("rawtypes")
public class EntityAbstract implements IEntityAbstract {
	private Long id = 0L;
	private Long parentId = 0L ;
	private Long rootId = 0L;
	private String name;
	private String description;
	private String path;
	private EntityState state = EntityState.NONE;
	
	Set<IWatcherAbstract> watchers = new HashSet<>();
	
	public void watch() {
		watchers.forEach(w -> w.watch(this));
	}
}
