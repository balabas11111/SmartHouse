/*
 * EntityNrfManager.h
 *
 *  Created on: 8 ���. 2019 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_ENTITYLIBSIMPLE_ENTITYNRFMANAGER_H_
#define LIBRARIES_ENTITYLIBSIMPLE_ENTITYNRFMANAGER_H_

#include <EntityNrfHolder.h>
#ifdef RF24_ENABLED
#include "RF24.h"
#endif

class EntityNrfManager {
public:
	EntityNrfManager(EntityNrfHolder** nrfs, uint8_t nrfCount) {
		this->nrfs = nrfs;
		this->nrfCount = nrfCount;
	}
	;

	virtual void init() {
#ifdef RF24_ENABLED
		this->radio = new RF24(12, 14, 26, 25, 27);
		radio->begin();
		radio->setChannel(2);
		radio->setPayloadSize(17);
		radio->setDataRate(RF24_250KBPS);
		radio->openReadingPipe(1,pipeReceive);
		radio->startListening();

		// radio->printDetails();
#endif
	}

	virtual void loop() {
		delay(1);
#ifdef RF24_ENABLED
		if (radio->available()) {
			Serial.println(FPSTR("loop"));

			radio->read(msg, 16);
			Serial.println(FPSTR("radio="));
			Serial.println(msg);

			for(uint8_t i =0; i< nrfCount; i++) {
				EntityNrfHolder* nrf = nrfs[i];
				if(nrf->isTarget(msg)) {
					nrf->set(msg);
				}
			}
			//delay(10);

		}
		else {
			Serial.println("No radio available");
		}
#endif
	}

	virtual ~EntityNrfManager() {
	}
	;

private:
	EntityNrfHolder** nrfs;
	uint8_t nrfCount;

	char msg[16];
#ifdef RF24_ENABLED
	RF24* radio = nullptr;
#endif
	const uint64_t pipeReceive = 0xE8E8F0F0E1LL;

	void initNrf() {

	}
};

#endif /* LIBRARIES_ENTITYLIBSIMPLE_ENTITYNRFMANAGER_H_ */
