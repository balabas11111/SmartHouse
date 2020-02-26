package com.balabas.smarthouse.server.util;

import java.math.BigDecimal;
import java.math.RoundingMode;

import org.springframework.beans.factory.annotation.Value;

import lombok.experimental.UtilityClass;

@UtilityClass
public class MathUtil {

	private static Integer precision = 2;

	@Value("${smarthouse.server.entityfield.float.precision:#{null}}")
	public void setPresision(Integer pr) {
		precision = pr;
	}

	public static Float precise(Float value) {
		if (value == null) {
			return value;
		}
		return (new BigDecimal(value).setScale(precision, RoundingMode.HALF_UP)).floatValue();
	}
	
	public static boolean isValidFloat(String value) {
		try {
			getAsFloat(value);
		} catch(Exception e) {
			return false;
		}
		return true;
	}
	

	public static Float getAsFloat(String parameter) {
		return Float.valueOf(parameter);
	}
}
