package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.Map;

import com.google.common.collect.Maps;

public enum Severity {
	INFO(Emoji.CHECK_MARK, "INFO", "Информация"),
	WARN(Emoji.WARNING, "WARN", "Предупреждение"),
	ERROR(Emoji.ERROR, "ERROR", "Ошибка");

	private Emoji emoji;
	private String code;
	private String description;
	
  private static final Map<String, Severity> codeMap =
    		Maps.newHashMapWithExpectedSize(Severity.values().length);
    static {
        for (Severity severity : Severity.values()) {
        	codeMap.put(severity.code, severity);
        }
    }
	
	private Severity(Emoji emoji, String code, String description) {
		this.emoji = emoji;
		this.code = code;
		this.description = description;
	}
	
	public Emoji getEmoji() {
		return this.emoji;
	}
	
	public String getCode() {
		return code;
	}
	
	public String getDescription() {
		return description;
	}
	
	public static Severity getSeveritybyCode(String code) {
		return codeMap.getOrDefault(code, INFO);
	}
}
