package com.balabas.smarthouse.server.entity.alarmV2;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;

@Target({ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
public @interface AlarmMarker {
	
	Class<?> target() default EntityField.class;
	
	Class<?> detail() default Number.class;
}
