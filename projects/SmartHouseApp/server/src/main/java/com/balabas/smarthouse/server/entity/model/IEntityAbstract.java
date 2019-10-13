package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.watcher.IWatcherAbstract;

public interface IEntityAbstract {

	Long getId();
	void setId(Long id);
	
	Long getParentId();
	void setParentId(Long id);
	
	Long getRootId();
	void setRootId(Long id);
	
	String getName();
	void setName(String name);
	
	String getDescription();
	void setDescription(String description);
	
	String getPath();
	void setPath(String value);
	
	EntityState getState();
	void setState(EntityState state);
	
	Set<IWatcherAbstract> getWatchers();
	void setWatchers(Set<IWatcherAbstract> watchers);
	
	void watch();
}
