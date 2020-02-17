package com.balabas.abstr.model;

public interface IIdentifiable {

	String PROP_ID = "id";
	String PROP_NAME = "name";

	Long getId();
	void setId(Long id);
	
	public static boolean existsAndIsNotNew(IIdentifiable item) {
		return item != null && !isNew(item);
	}

	public static boolean isNew(IIdentifiable item) {
		return item.getId() == null || item.getId().equals(0L);
	}
}
