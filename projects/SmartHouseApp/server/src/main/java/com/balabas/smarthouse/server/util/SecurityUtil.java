package com.balabas.smarthouse.server.util;

import java.nio.charset.StandardCharsets;
import java.util.Date;
import java.util.UUID;

import com.google.common.hash.Hashing;

import lombok.experimental.UtilityClass;

@UtilityClass
public class SecurityUtil {
	
	public static String generateTempServerKey(String value) {
		String salted = UUID.randomUUID().toString()+":"+value+":"+Long.toString((new Date()).getTime());
		return Hashing.sha1().hashString(salted, StandardCharsets.UTF_8).toString();
	}
	
	public static String split(String...strings) {
		StringBuilder buf = new StringBuilder();
		for(String str:strings) {
			buf.append(str);
		}
		return buf.toString();
	}
	
	public static String getHashed(String value) {
		return Hashing.sha1().hashString(value, StandardCharsets.UTF_8).toString();
	}
	
}
