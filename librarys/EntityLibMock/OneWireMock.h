/*
 * OneWireMock.h
 *
 *  Created on: Mar 26, 2019
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_ENTITYLIBMOCK_ONEWIREMOCK_H_
#define LIBRARIES_ENTITYLIBMOCK_ONEWIREMOCK_H_

class OneWireMock {
public:
	OneWireMock(int pin){
		this->pin = pin;
	}

	virtual ~OneWireMock() { }
	int getPin(){ return pin; }

protected:
	int pin;
};

#endif /* LIBRARIES_ENTITYLIBMOCK_ONEWIREMOCK_H_ */
