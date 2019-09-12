/*
 * Notifier.cpp
 *
 *  Created on: 5 сент. 2019 г.
 *      Author: Vitaliy
 */

#include <Notifiers/Notifier.h>

Notifier::Notifier(const char* name, NotificationTarget* target) {
	this->name = name;
	this->target = target;
}

