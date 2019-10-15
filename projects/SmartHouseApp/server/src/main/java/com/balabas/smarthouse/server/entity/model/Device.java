package com.balabas.smarthouse.server.entity.model;

import lombok.Data;
import lombok.EqualsAndHashCode;

@Data
@EqualsAndHashCode(callSuper = true)
public class Device extends ItemContainer<IGroup> implements IDevice {

	private String firmware;
}
