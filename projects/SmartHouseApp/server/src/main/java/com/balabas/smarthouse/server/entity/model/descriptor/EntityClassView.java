package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.stream.Stream;

public enum EntityClassView {

	EDC_CLASS_VIEW_DEFAULT("def"),
	EDC_CLASS_VIEW_NONE("non"),
	EDC_CLASS_VIEW_LIST("lst"),
	EDC_CLASS_VIEW_TABLE("tbl"),
	EDC_CLASS_VIEW_MAP("map"),
	EDC_CLASS_VIEW_LABEL("txt"),
	EDC_CLASS_VIEW_INPUT("inp"),
	EDC_CLASS_VIEW_PASSWORD_INPUT("pwd"),
	EDC_CLASS_VIEW_IP_INPUT("ipi"),
	EDC_CLASS_VIEW_COMBOBOX("cmb"),
	EDC_CLASS_VIEW_DATE_TIME("dtm"),
	EDC_CLASS_VIEW_OPTIONS("opt"),
	EDC_CLASS_VIEW_BUTTON("btn"),
	EDC_CLASS_VIEW_BUTTON_COMMAND("btc"),
	EDC_CLASS_VIEW_BUTTON_BOOLEAN("btb"),
	EDC_CLASS_VIEW_BUTTON_OPTIONS("bto");
	
	private String key;
	
	EntityClassView(String key) {
		this.key = key;
	}
	
	public String getKey() {
		return this.key;
	}
	
	public static EntityClassView from(String key){
		return Stream.of(EntityClassView.values()).filter( i -> i.getKey().equalsIgnoreCase(key)).findFirst().orElse(EDC_CLASS_VIEW_DEFAULT);
	}
}
