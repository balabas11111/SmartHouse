package com.balabas.smarthouse.server.util;

import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;

import lombok.experimental.UtilityClass;

@UtilityClass
public class ReflectionUtil {

	@SuppressWarnings("rawtypes")
	public static Class getGenericClass(Object obj, int paramIndex) {
			Type genericSuperClass = obj.getClass().getGenericSuperclass();

			ParameterizedType parametrizedType = null;
			while (parametrizedType == null) {
				if ((genericSuperClass instanceof ParameterizedType)) {
					parametrizedType = (ParameterizedType) genericSuperClass;
				} else {
					if(genericSuperClass ==null) {
						return null;
					}
					genericSuperClass = ((Class<?>) genericSuperClass).getGenericSuperclass();
				}
			}

			return  (Class) parametrizedType.getActualTypeArguments()[paramIndex];
	}
}
