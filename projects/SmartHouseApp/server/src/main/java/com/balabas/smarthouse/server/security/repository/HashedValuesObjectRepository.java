package com.balabas.smarthouse.server.security.repository;

import java.util.Map;

import javax.persistence.EntityNotFoundException;

public interface HashedValuesObjectRepository<T> extends HashedValuesRepository{

	Map<String,T> getHashObjectMap();
	
	default boolean containsObjectByValue(String value) {
		if(!containsValue(value)) {
			return false;
		}
		
		String hash = getValueHashMap().get(value);
		
		return containsObjectByHash(hash);
	}
	
	default boolean containsObjectByHash(String hash) {
		return getHashObjectMap().containsKey(hash);
	}
	
	default String putValueObjectGetHash(String value, T object) {
		String hash = putValue(value);
		getHashObjectMap().put(hash, object);
		return hash;
	}
	
	default T getObjectByValue(String value) throws EntityNotFoundException{
		if(!containsObjectByValue(value)) {
			throw new EntityNotFoundException(value +" value");
		}
		String hash = getHashByValue(value);
		return getObjectByHash(hash);
	}
	
	default T getObjectByHash(String hash) throws EntityNotFoundException{
		if(containsObjectByHash(hash)) {
			throw new EntityNotFoundException(hash +" hash");
		}
		return getHashObjectMap().get(hash);
	}
	
	default void removeObjectByValue(String value) {
		if(containsObjectByValue(value)) {
			removeObjectByHash(getHashByValue(value));
		}
	}
	
	default void removeObjectByHash(String hash) {
		if(containsObjectByHash(hash)) {
			getHashObjectMap().remove(hash);
		}
	}
	
}
