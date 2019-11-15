package com.balabas.smarthouse.server.entity.alarm;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Target({ElementType.TYPE})
@Retention(RetentionPolicy.RUNTIME)
public @interface EntityFieldAlarmMarker {
	
	Class<?> target() default Number.class;

}
