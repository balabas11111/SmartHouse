package com.balabas.smarthouse.server.entity.watcher;

import com.balabas.smarthouse.server.entity.model.IEntityAbstract;

public interface IWatcherAbstract<T extends IEntityAbstract> {

	void watch(T entity);
}
