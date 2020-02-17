package com.balabas.abstr.model;

public interface IItemAbstract extends IIdentifiable, INameable{
	
	String getItemClassId();
	
	boolean isVirtualized();
	void setVirtualized(Boolean value);
	
	default boolean equalsAsValue(Object val1, Object val2) {
		boolean result = false;
		
		if(val1 == null && val2 == null) {
			return true;
		}
		if(val1!=null) {
			return val1.equals(val2);
		}
		if(val2!=null) {
			return val2.equals(val1);
		}
		
		return result;
	}
	
}
