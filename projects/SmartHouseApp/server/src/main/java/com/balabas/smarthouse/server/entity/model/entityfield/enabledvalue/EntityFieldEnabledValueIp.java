package com.balabas.smarthouse.server.entity.model.entityfield.enabledvalue;

import java.net.InetAddress;
import java.net.UnknownHostException;

import com.balabas.smarthouse.server.exception.BadValueException;

import lombok.extern.log4j.Log4j2;

@Log4j2
@javax.persistence.Entity
public class EntityFieldEnabledValueIp extends EntityFieldEnabledValue<InetAddress> implements IEntityFieldEnabledValue<InetAddress> {

	@Override
	public void setValueStr(String value) {
		try {
			InetAddress addr = InetAddress.getByName(value);
			validateValue(addr);
			this.value = addr;
		} catch (UnknownHostException | BadValueException e) {
			log.error(e);
			throw new IllegalArgumentException(e.getMessage());
		}
	}
	
	
	public void validateValue(InetAddress value) throws BadValueException {
		
	}
}
