package com.balabas.smarthouse.server.entity.model.entityfields;

import java.net.InetAddress;
import java.net.UnknownHostException;

import javax.persistence.Entity;

import com.balabas.smarthouse.server.exception.BadValueException;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Entity
public class EntityFieldIp extends EntityField<InetAddress> implements IEntityField<InetAddress> {

	@Override
	public void validateValue(InetAddress value) throws BadValueException {
		super.validateValue(value);
	}
	
	@Override
	public void setValue(InetAddress value) throws BadValueException {
		this.value = value;
	}
	
	void setValue(String value) throws BadValueException{
		setValueStr(value);
	}
	
	@Override
	public void setValueWithNoCheck(InetAddress value) throws BadValueException {
		this.value = value;
	}
	
	@Override
	public void setValueStr(String value) throws BadValueException {
		try {
			InetAddress addr = InetAddress.getByName(value);
			validateValue(addr);
			this.value = addr;
		} catch (UnknownHostException e) {
			log.error(e);
			throw new BadValueException(e.getMessage());
		}
	}

}
