package com.balabas.abstr.service;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Spliterator;
import java.util.function.Predicate;
import java.util.stream.Collectors;
import java.util.stream.StreamSupport;

import org.hibernate.Hibernate;

import com.balabas.abstr.model.ItemAbstract;
import com.balabas.abstr.repository.AbstractRepository;

public interface AbstractService<T extends ItemAbstract> {

	AbstractRepository<T> getRepository();
	
	default Iterable<T> getAll() {
		return getRepository().findAll();
	}
	
	default Spliterator<T> getIterator() {
		return getAll().spliterator();
	}
	
	default T getInitialized(Long id) {
		T entity = get(id);
		
		if(entity!=null) {
			Hibernate.initialize(entity);
		}
		
		return entity;
	}
	
	default List<T> list() {
		return StreamSupport.stream(getIterator(), false)
                .collect(Collectors.toList());
	}
	
	default List<T> listInitialized() {
		Iterable<T> list = getAll();
		
		list.forEach(entity -> Hibernate.initialize(entity));
		
		return StreamSupport.stream(list.spliterator(), false)
                .collect(Collectors.toList());
	}
	
	T create();
	
	default Optional<T> loadOpt(Long id) {
		return getRepository().findById(id);
	}
	
	default T load(Long id) {
		return loadOpt(id).orElse(null);
	}
	
	default T loadInitialized(Long id) {
		Optional<T> opt = loadOpt(id);
		opt.ifPresent(entity -> Hibernate.initialize(entity));
		
		return opt.orElse(null);
	}
	
	default T get(Long id) {
		return load(id);
	}
	
	default List<T> get(Predicate<T> clause) {
		return list().stream().filter(clause).collect(Collectors.toList());
	}
	
	default T save(T entity) {
		return getRepository().save(entity);
	}
	
	default boolean delete(Long id) {
		try {
			getRepository().deleteById(id);
		} catch (Exception e) {
			return false;
		}
		return true;
	}
	
	default List<T> getByName(String name) {
		return getRepository().findByName(name);
	}

	String getEntityClass();
	
	default T getSimilarTo(T entity) {
		return getSimilarToByIdName(entity);
	}
	
	default T getSimilarToByIdName(T entity) {
		T result = getSimilarToById(entity); 
		
		if(result==null) {
			result = getSimilarToByName(entity);
		}
		
		if(result==null) {
			result = create();
		}
		
		return result;
	}

	default T getSimilarToById(T entity) {
		if(entity.hasId()) {
			return getRepository().findById(entity.getId()).orElse(create());
		}
		return null;
	}
	
	default T getSimilarToByName(T entity) {
		if(entity.hasName()) {
			List<T> list = getRepository().findByName(entity.getName());
			
			if(list!=null && list.size()>0) {
				return list.get(0);
			}
		}
		return null;
	}

	default T join(T fromEntity, T toEntity) {
		if(!toEntity.hasId() && fromEntity.hasId()) {
			toEntity.setId(fromEntity.getId());
		}
		toEntity.setName(fromEntity.getName());
		
		return toEntity;
	}

	default Map<Long, T> getEntitiesMapById() {
		return getEntitiesMapById(list());
	}
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	default Map<Long, T> getEntitiesMapById(List<T> list) {
		Map<Long, T> result = new LinkedHashMap();
		
		list.stream().forEach(entity -> result.put(entity.getId(), entity));
		
		return result;
	}

}
