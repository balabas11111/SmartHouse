package com.balabas.smarthouse.server.entity.model;

public interface IEntityAbsttractChangeListener<T extends IEntityAbstract> {

	void dispatch(T entity);
}
