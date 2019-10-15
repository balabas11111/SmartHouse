package com.balabas.smarthouse.server.model;

public enum EntityClass{
		
		DEFAULT(null),
		TOGGLE_BUTTON("tb"),
		PUSH_BUTTON("pb");
		
		String itemClass;
		
		EntityClass(String itemClass){
			this.itemClass = itemClass;
		}
		
		public String getItemClass() {
			return itemClass;
		}
		
		public static EntityClass getByKey(String key) {
			if(key == null || key.isEmpty()) {
				return DEFAULT;
			}
			for(EntityClass r:EntityClass.values()) {
				if(key.equals(r.getItemClass())) {
					return r;
				}
			}
			return DEFAULT;
		}
}
