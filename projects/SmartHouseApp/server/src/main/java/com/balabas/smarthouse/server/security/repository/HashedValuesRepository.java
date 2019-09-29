package com.balabas.smarthouse.server.security.repository;

import java.util.List;
import java.util.Map;

import javax.persistence.EntityNotFoundException;

import org.springframework.beans.factory.InitializingBean;

public interface HashedValuesRepository extends InitializingBean{

	Map<String, String> getValueHashMap();
	
	Map<String, String> getHashValueMap();
	
	default boolean containsValue(String value) {
		return getValueHashMap().containsKey(value);
	}
	
	default boolean containsHash(String hash) {
		return getHashValueMap().containsKey(hash);
	}
	
	default String getHashByValue(String value) throws EntityNotFoundException{
		if(!containsValue(value)) {
			throw new EntityNotFoundException(value + " not found");
		}
		return getValueHashMap().get(value);
	}
	
	default String getValueByHash(String hash)  throws EntityNotFoundException{
		if(!containsHash(hash)) {
			throw new EntityNotFoundException(hash + " not found");
		}
		return getHashValueMap().get(hash);
	}
	
	default String putValue(String value) {
		String hash = hashValue(value);
		
		getValueHashMap().put(value, hash);
		getHashValueMap().put(hash, value);
		
		return hash;
	}
	
	default void putNoHash(String value, String hash) {
		getValueHashMap().put(value, hash);
		getHashValueMap().put(hash, value);
	}
	
	default String putValueAddSaltGetHash(String value) {
		String valueSalted = saltValue(value);
		String hash = hashSaltedValue(valueSalted);
		
		putNoHash(value, hash);
		
		return hash;
	}
	
	default String getSaltedValueHash(String value) {
		return hashSaltedValue(saltValue(value));
	}
	
	default void putValues(List<String> values) {
		values.stream().forEach(this::putValue);
	}
	
	default void removeValue(String value) {
		if(containsValue(value)) {
			String hash =getValueHashMap().get(value);
			
			getValueHashMap().remove(value);
			
			if(containsHash(hash)) {
				getHashValueMap().remove(hash);
			}
		}
	}
	
	default void removeHash(String hash) {
		if(containsHash(hash)) {
			String value =getHashValueMap().get(hash);
			
			getHashValueMap().remove(hash);
			
			if(containsValue(value)) {
				getValueHashMap().remove(value);
			}
		}
	}
	
	default public void removeValues(List<String> values)  throws EntityNotFoundException{
		values.stream().forEach(this::removeValue);
	}
	
	default String getSplitter() {
		return ":";
	}
	
	String hashValue(String value);
	
	default String hashSaltedValue(String salted) {
		return hashValue(salted);
	}
	
	default String saltValue(String value) {
		return value;
	}
	
	default void saveAll() {};
	
	default String split(String...strings) {
		int size = strings.length;
		String result = "";
		
		for(int i= 0; i<size; i++) {
			result+=strings[i];
			if(i!=size-1) {
				result+=getSplitter();
			}
		}
		
		return result;
	}
	
}
