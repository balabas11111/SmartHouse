package com.balabas.smarthouse.server.util;

import lombok.experimental.UtilityClass;

@UtilityClass
public class StringUtil {

	public static String setSize(String str) {
		return setSize(str, 16);
	}

	public static String setSize(String str, int size) {
		if (str == null) {
			str = "";
		}

		int strSize = str.length();
		int spacesCount = -1; 
		
		if (strSize > size) {
			str = str.substring(1, size);
		}
		if (strSize < size) {
			spacesCount = size - strSize;
			for(int i=0;i<spacesCount;i++) {
				str = str + " ";
			}
		}
		
		return str;
	}
}
