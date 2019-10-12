package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.entity.model.IEntityAbstract;
import com.balabas.smarthouse.server.entity.repository.IEntityAbstractRepository;

public interface IEntityAbstractService<T extends IEntityAbstract, S extends IEntityAbstractRepository<T>> {

}
