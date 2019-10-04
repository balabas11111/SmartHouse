package com.balabas.smarthouse.server.util;

import java.util.Date;
import java.util.UUID;

import static com.balabas.smarthouse.server.DeviceConstants.FIELD_SPLITTER;
import static java.nio.charset.StandardCharsets.UTF_8;

import static com.google.common.hash.Hashing.sha1;

import lombok.experimental.UtilityClass;

@UtilityClass
public class SecurityUtil {

	public static String generateTempServerKey(String value) {
		String salted = UUID.randomUUID().toString() + FIELD_SPLITTER + value + FIELD_SPLITTER
				+ Long.toString((new Date()).getTime());
		
		return sha1().hashString(salted, UTF_8).toString();
	}

	public static String split(String... strings) {
		StringBuilder buf = new StringBuilder();
		for (String str : strings) {
			buf.append(str);
		}
		return buf.toString();
	}

	public static String getHashed(String value) {
		return sha1().hashString(value, UTF_8).toString();
	}

}
