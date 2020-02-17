package com.balabas.entities.model;

import java.util.List;

import javax.persistence.Entity;
import javax.persistence.ManyToMany;

import com.balabas.abstr.model.ItemAbstract;

import lombok.Getter;
import lombok.Setter;

@Entity
public class PhoneUser extends ItemAbstract {

	public static final String PROP_NUMBERS = "numbers";
	
	@Getter @Setter
	@ManyToMany
	private List<PhoneNumber> numbers;

}
