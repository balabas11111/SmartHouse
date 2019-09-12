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

	void init(long interv){
		this->interval = interv * 1000;
		/*Serial.print(FPSTR("init entity interval ="));
		Serial.print(interv);
		Serial.print(FPSTR(" result ="));
		Serial.println(getInterval());
		*/
	}

	virtual void update(bool withCheck = false) final{
		if(this->interval<1){
			Serial.println(FPSTR("No update    is expected interval<1"));
			return;
		}

		if((withCheck && !shouldUpdate())){
			Serial.println(FPSTR("No update    is expected"));
			return;
		}

		doUpdate();
		lastUpdate = millis();
	}

	virtual void doUpdate() = 0;

	long getInterval(){
		return interval;
	}

	bool isAutoupdate(){
		return interval>0;
	}

protected:
	bool shouldUpdate(){
		return (lastUpdate + interval < millis());
	}

	long interval = SETTINGS_BOX_INTERVAL;
	unsigned long lastUpdate = millis();
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYUPDATE_H_ */
