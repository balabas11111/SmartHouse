package com.balabas.smarthouse.server.entity.model.descriptor;

import com.balabas.smarthouse.server.entity.model.IEntityAbstract;

public interface IEntityDescriptor extends IEntityAbstract {

	EntityClassType getEntityClassType();
	EntityClassEditors getEntityClassEditor();
	EntityClassSubstance getEntityClassSubstance();
	EntityClassLevel getEntityClassLevel();
	EntityClassPersistor getEntityClassPersistor();
	EntityClassView getEntityClassView();
	Long getTimeToLive();
	boolean isPushFromServer();
}
