/*
 * HtmlWidgetAction.h
 *
 *  Created on: 21 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINEVENTLIB_HTMLWIDGETACTION_H_
#define LIBRARIES_PINEVENTLIB_HTMLWIDGETACTION_H_

#include "Arduino.h"

class HtmlWidgetAction {

	public:
		String actionName;
		String widgetName;
		String className;
		String childClassName;
};

#endif /* LIBRARIES_PINEVENTLIB_HTMLWIDGETACTION_H_ */
