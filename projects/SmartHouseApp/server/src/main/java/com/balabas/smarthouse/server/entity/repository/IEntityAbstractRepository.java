package com.balabas.smarthouse.server.entity.repository;

import java.io.IOException;
import java.util.Optional;
import java.util.Set;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.EntityState;
import com.balabas.smarthouse.server.entity.model.IEntityAbstract;

@SuppressWarnings("rawtypes")
public interface IEntityAbstractRepository<T extends IEntityAbstract> {

	T create(JSONObject json);
	
	Predicate<? super T> getPrototypeComparePredicate(T entity);
	
	Predicate<? super T> getUniqueComparePredicate(T entity);
	
	Class getClazz();
	
	void load() throws IOException;
	void save() throws IOException;
	
	default Optional<T> getById(Long id){
		return getList().stream().filter(e -> e.getId().equals(id)).findFirst();
	}
	
	Set<T> getList();
	
	default void save(T entity) throws IOException {
		boolean notFound = getByPredicate( getUniqueComparePredicate(entity)).isEmpty();
		
		if(notFound) {
			this.getList().add(entity);
			save();
		}
	}
	
	default Set<T> getByStates(Set<EntityState> states){
		return getByPredicate(e-> {
			for(EntityState state: states) {
				if(e.getState()!=null && e.getState().equals(state)) {
					return true;
				}
			}
			return false;
			});
	}
	
	default Set<T> getByState(EntityState state){
		return getByPredicate(e-> e.getState()!=null && e.getState().equals(state));
	}
	
	default Set<T> getByPrototype(T entity){
		return getByPredicate(getPrototypeComparePredicate(entity));
	}
	
	default Set<T> getByParentId(Long id){
		return getByPredicate(e-> e.getParentId()!=null && e.getParentId().equals(id));
	}
	default Set<T> getByRootId(Long id){
		return getByPredicate(e-> e.getRootId()!=null && e.getRootId().equals(id));
	}
	default Set<T> getByName(String name){
		return getByPredicate(e-> e.getName()!=null && e.getName().equals(name));
	}
	default Set<T> getByDescription(String description){
		return getByPredicate(e-> e.getDescription()!=null && e.getDescription().equals(description));
	}
	default Set<T> getByPath(String path){
		return getByPredicate(e->e.getPath()!=null && e.getPath().equals(path));
	}
	default Set<T> getByPredicate(Predicate<? super T> pr){
		return getList().stream().filter(pr).collect(Collectors.toSet());
	}
}
