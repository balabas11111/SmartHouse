package com.balabas.smarthouse.server.entity.model.enabledvalue;

import java.net.InetAddress;
import java.net.UnknownHostException;

import javax.persistence.Column;

import com.balabas.smarthouse.server.exception.BadValueException;

import lombok.Getter;
import lombok.Setter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@javax.persistence.Entity
public class EntityFieldEnabledValueIp extends EntityFieldEnabledValue<InetAddress> implements IEntityFieldEnabledValue<InetAddress> {

	@Getter @Setter
	@Column(name = "valueIp")
	protected InetAddress value;
	
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
	
	@Override
	public String getValueStr() {
		return this.value.toString();
	}
}
