package com.balabas.entities.model;

import javax.persistence.Entity;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;

import com.balabas.abstr.model.ItemAbstract;

import lombok.Getter;
import lombok.Setter;

@Entity
public class PhoneNumber extends ItemAbstract {

	public static final String PROP_NUMBER = "number";
	public static final String PROP_COMPANY = "company";
	
	@Getter @Setter
	private String number;
	
	@Getter @Setter
	@ManyToOne
	@JoinColumn(name = "company_id")
	private PhoneCompany company;
}
