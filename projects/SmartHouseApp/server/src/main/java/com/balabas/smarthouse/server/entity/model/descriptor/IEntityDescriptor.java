package com.balabas.smarthouse.server.entity.model.descriptor;

import java.util.Map;

import com.balabas.smarthouse.server.entity.model.IEntityAbstract;

public interface IEntityDescriptor extends IEntityAbstract {

	EntityClassType getEntityClassType();
	EntityClassView getEntityClassView();
	Long getTimeToLive();
	boolean isReadOnly();
	Map<String, String> getEnabledValues();
	/*
	EntityClassEditors getEntityClassEditor();
	EntityClassSubstance getEntityClassSubstance();
	EntityClassLevel getEntityClassLevel();
	EntityClassPersistor getEntityClassPersistor();
	*/
}
