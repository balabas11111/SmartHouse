/*
 * Measurable.h
 *
 *  Created on: 3 нояб. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_HELPERS_MEASURABLE_H_
#define LIBRARIES_HELPERS_MEASURABLE_H_

class Measurable {
public:
	virtual void initSensor();
	virtual void update();
	virtual void print();
};

#endif /* LIBRARIES_HELPERS_MEASURABLE_H_ */
