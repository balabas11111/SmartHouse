package com.balabas.smarthouse.server.util;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

import lombok.experimental.UtilityClass;

@UtilityClass
@SuppressWarnings("rawtypes")
public class SortingUtility {

	static final Class<?>[] classOrder = {String.class, Float.class, Long.class,  Boolean.class};
	
	public static final Comparator<? super IItemAbstract> DEFAULT_COMPARARTOR = ItemAbstract::compareByDescriptionField;
	
	public static Map<String, List<IEntityField>> sort(Map<String, List<IEntityField>> targetMap) {
		return sort(targetMap, DEFAULT_COMPARARTOR);
	}
	
	public static List<IEntityField> sort(Collection<IEntityField> toSort) {
		return sort(toSort, DEFAULT_COMPARARTOR);
	}
	
	public static Map<String, List<IEntityField>> sort(Map<String, List<IEntityField>> targetMap, Comparator<? super IItemAbstract> comparator) {
		for(String key: targetMap.keySet()) {
			List<IEntityField> sortedList = sort(targetMap.get(key), comparator);
			targetMap.put(key, sortedList);
		}
		return targetMap;
	}
	
	public static List<IEntityField> sort(Collection<IEntityField> toSort, Comparator<? super IItemAbstract> comparator) {
		List<IEntityField> result = new ArrayList<>();
		
		for(Class<?> clazz : classOrder) {
			result.addAll(getByFieldClassSorted(toSort, clazz, comparator));
		}
		
		return result;
	}

	static List<IEntityField> getByFieldClassSorted(Collection<IEntityField> toSort, Class<?> clazz, Comparator<? super IItemAbstract> comparator) {
		return toSort.stream().filter(ef -> ef.getClazz().equals(clazz)).sorted(comparator).collect(Collectors.toList());
	}
}
