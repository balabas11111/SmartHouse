package com.balabas.abstr.repository;

import java.util.List;

import org.springframework.data.repository.CrudRepository;

import com.balabas.abstr.model.ItemAbstract;

public interface AbstractRepository<T extends ItemAbstract>  extends CrudRepository<T, Long> {
	List<T> findByName(String name);
}
