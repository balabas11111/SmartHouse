package com.balabas.smarthouse.server.entity.model;

public interface IIdentifiable {

	Long getId();
	void setId(Long id);
	
	default boolean isNew() {
		return isNew(this);
	}
	
	public static boolean existsAndIsNotNew(IIdentifiable item) {
		return item != null && !isNew(item);
	}

	public static boolean isNew(IIdentifiable item) {
		return item.getId() == null || item.getId().equals(0L);
	}
}
