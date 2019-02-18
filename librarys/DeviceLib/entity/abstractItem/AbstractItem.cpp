/*
 * AbstractItem.cpp
 *
 *  Created on: 16 февр. 2019 г.
 *      Author: Vitaliy
 */

#include <entity/abstractItem/AbstractItem.h>

String AbstractItem::getFieldValue(String key) {
	return "";
}

int AbstractItem::begin() {
}

int AbstractItem::setFieldValue(String key, String value) {
}

boolean AbstractItem::validateKey(String key) {
}

int AbstractItem::loadDefault() {
}

int AbstractItem::putJson(JsonObject& obj) {
}

String AbstractItem::getJson(String root) {
}

boolean AbstractItem::isCached() {
}

int AbstractItem::saveToCache() {
}

int AbstractItem::initializeItem(JsonObject* parent, JsonObject* object) {
}
