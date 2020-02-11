package com.balabas.smarthouse.server.util;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

import lombok.experimental.UtilityClass;

@UtilityClass
public class DateTimeUtil {

	public static final String DATE_TIME_PATTERN = "hh:mm:ss dd-L-yyyy";
	
	public static String getDateTimeStr() {
		return getDateTimeStr(LocalDateTime.now(), DATE_TIME_PATTERN);
	}
	
	public static String getDateTimeStr(LocalDateTime date, String pattern) {
		DateTimeFormatter formatter = DateTimeFormatter.ofPattern(pattern);
		return date.format(formatter);
	}
}
