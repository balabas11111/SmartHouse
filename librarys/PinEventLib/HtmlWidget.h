/*
 * HtmlWidget.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_PINEVENTLIB_HTMLWIDGET_H_
#define LIBRARIES_PINEVENTLIB_HTMLWIDGET_H_

#include "Arduino.h"
#include <ArduinoJson.h>

class HtmlWidget {
public:

	virtual ~HtmlWidget();

	virtual String getName()=0;
	virtual String getHtml()=0;
	virtual String getJson()=0;
};

#endif /* LIBRARIES_PINEVENTLIB_HTMLWIDGET_H_ */
