/*
 * EntityUpdate.h
 *
 *  Created on: Aug 27, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATE_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATE_H_

#define NO_UPDATE -1
#define SETTINGS_BOX_INTERVAL 0

#include <Arduino.h>

class EntityUpdate {
public:
	EntityUpdate(){};
	virtual ~EntityUpdate(){};

	void init(long interv,unsigned long initTime);

	virtual void loop(){}

	virtual bool update(unsigned long time,bool force = false) final;
	virtual void doUpdate() = 0;

	long getInterval();
	bool isAutoupdate();

protected:
	virtual bool shouldUpdate(unsigned long time);

	long interval = SETTINGS_BOX_INTERVAL;
	unsigned long lastUpdate = millis();
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATE_H_ */
