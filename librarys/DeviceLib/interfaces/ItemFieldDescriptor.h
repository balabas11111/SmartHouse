/*
 * ItemExtraSett.h
 *
 *  Created on: 2 февр. 2019 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_DEVICELIB_ABSTRACTITEMFIELDPROVIDERItemFieldDescriptor_H_
#define LIBRARIES_DEVICELIB_ABSTRACTITEMFIELDPROVIDERItemFieldDescriptor_H_

#include "Arduino.h"
#include "interfaces/Nameable.h"

const PROGMEM char IFP_ThingSpeak[] = "IFP_ThingSpeak";
const PROGMEM uint8_t IFP_ThingSpeak_ID = 0;

const PROGMEM char IFP_MinMaxAlarm[] = "IFP_MinMaxAlarm";
const PROGMEM uint8_t IFP_MinMaxAlarm_ID = 1;

class ItemFieldDescriptor: public Nameable {
public:
	ItemFieldDescriptor(){};

	virtual ~ItemFieldDescriptor(){};

	virtual uint8_t getId()=0;

	virtual String getName()=0;

	virtual uint8_t getSize()=0;

	virtual const char* const* getKeys()=0;

	virtual const char* getDefaults()=0;
};

#endif /* LIBRARIES_DEVICELIB_ABSTRACTITEMEXTRA_ITEMEXTRASETT_H_ */
