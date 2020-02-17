package com.balabas.abstr.service;

import org.springframework.beans.factory.annotation.Autowired;

import com.balabas.abstr.model.ItemAbstract;
import com.balabas.abstr.repository.AbstractRepository;

import lombok.Getter;

public abstract class AbstractServiceImpl<T extends ItemAbstract> implements AbstractService<T>{

	private String entityClassName = null;
	
	@Autowired
	@Getter
	AbstractRepository<T> repository;

	@Override
	public String getEntityClass() {
		if(entityClassName == null) {
			entityClassName = create().getItemClassId();
		}
		return entityClassName;
	}

}
