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
	
	public static Date getDate() {
		return new Date();
	}
	
	public static Long getDiffSecs(Date date1) {
		return getDiffSecs(getDate(), date1);
		
	}
	
	public static Long getDiffSecs(Date date1, Date date2) {
		return getDiffMs(date1, date2)/1000;
		
	}
	
	public static long getDiffMs(Date date1, Date date2) {
		return Math.abs(date2.getTime()  - date1.getTime());
	}
}
