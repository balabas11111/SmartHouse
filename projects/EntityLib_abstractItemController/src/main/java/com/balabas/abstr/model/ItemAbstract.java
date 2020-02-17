package com.balabas.abstr.model;

import javax.persistence.Column;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.MappedSuperclass;

import org.springframework.util.StringUtils;

import lombok.Getter;
import lombok.Setter;

@MappedSuperclass
public class ItemAbstract implements IItemAbstract {

	public static final String PROP_ID = "id";
	public static final String PROP_NAME = "name";

	@Id
	@GeneratedValue(strategy = GenerationType.IDENTITY)
	@Getter
	@Setter
	protected Long id = 0L;
	@Getter
	@Setter
	protected String name;
	@Getter
	@Setter
	@Column(columnDefinition = "boolean default false")
	protected Boolean virtualized;

	@Override
	public String getItemClassId() {
		StringBuilder builder = new StringBuilder();

		builder.append(this.getClass().getSimpleName());

		return builder.toString();
	}

	@Override
	public boolean isVirtualized() {
		return Boolean.TRUE.equals(virtualized);
	}
	
	public static int compareByName(IItemAbstract ia1, IItemAbstract ia2) {
		return ia1.getName().compareTo(ia2.getName());
	}

	public boolean hasId() {
		return id!=null && id!=0L;
	}

	public boolean hasName() {
		return !StringUtils.isEmpty(getName());
	}

}
