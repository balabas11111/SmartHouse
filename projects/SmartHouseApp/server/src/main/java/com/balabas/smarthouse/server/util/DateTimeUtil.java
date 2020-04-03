package com.balabas.smarthouse.server.util;

import java.text.SimpleDateFormat;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Date;

import lombok.experimental.UtilityClass;

@UtilityClass
public class DateTimeUtil {

	public static final String DATE_TIME_PATTERN = "HH:mm:ss dd-L-yyyy";
	
	public static String getDateTimeStr() {
		return getDateTimeStr(LocalDateTime.now(), DATE_TIME_PATTERN);
	}
	
	public static String getDateTimeStr(LocalDateTime date, String pattern) {
		DateTimeFormatter formatter = DateTimeFormatter.ofPattern(pattern);
		return date.format(formatter);
	}
	
	public static String getDateTimeStr(long lastUpdated) {
		Date dt = new Date(lastUpdated);
		
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat(DATE_TIME_PATTERN);
		return simpleDateFormat.format(dt);
	}
	
	public static long now() {
		return (new Date()).getTime();
	}
}