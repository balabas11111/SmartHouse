package com.balabas.smarthouse.server.util;

import lombok.experimental.UtilityClass;
import lombok.extern.log4j.Log4j2;

@Log4j2
@UtilityClass
public class ObjectUtil {

	
	public static final String BOOLEAN_TRUE = "True";
	public static final String BOOLEAN_FALSE = "false";
	
	public static Boolean getBooleanValue(String val) {
		if(BOOLEAN_TRUE.equalsIgnoreCase(val)) {
			return true;
		}
		if(BOOLEAN_FALSE.equalsIgnoreCase(val)) {
			return false;
		}
		try {
			Integer i = Integer.getInteger(val);
			
			return i!=0;
		}catch(Exception e) {
			log.error(e);
			return null;
			//throw new IllegalArgumentException("Not boolean" + val);
		}
	}
}
