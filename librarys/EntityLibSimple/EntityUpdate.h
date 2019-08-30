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
	EntityUpdate();
	virtual ~EntityUpdate(){};

	void init(int16_t interval){
		this->interval = interval * 1000;
	}

	virtual void update(bool withCheck = false) final{
		if(this->interval<1 || (withCheck && !shouldUpdate())){
			return;
		}

		doUpdate();
		lastUpdate = millis();
	}

	virtual void doUpdate() = 0;

	int16_t getInterval(){
		return interval;
	}

	bool isAutoupdate(){
		return interval>0;
	}

protected:
	bool shouldUpdate(){
		return (lastUpdate + interval < millis());
	}

	int16_t interval = SETTINGS_BOX_INTERVAL;
	unsigned long lastUpdate = millis();
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATE_H_ */
