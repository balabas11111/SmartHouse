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

//base controls
const char HTML_B_OPEN[] PROGMEM ="<b>";

const char HTML_H2_OPEN[] PROGMEM ="<h2>";
const char HTML_H2_CLOSE[] PROGMEM ="</h2>";

//base attributes
const PROGMEM char HTML_ATTR_ID_OPEN[]                = " id=\"";
const PROGMEM char HTML_ATTR_NAME_OPEN[]              = " name=\"";
const PROGMEM char HTML_ATTR_TYPE_OPEN[]              = " type=\"";
const PROGMEM char HTML_ATTR_VALUE_OPEN[]             = " value=\"";
const PROGMEM char HTML_ATTR_CLASS_OPEN[]             = " class=\"";
const PROGMEM char HTML_ATTR_STYLE_OPEN[]             = " style=\"";
const PROGMEM char HTML_ATTR_CLOSE[]                  = "\"";
const PROGMEM char HTML_ATTR_SPACE[]                  = " ";

//class names W3
const PROGMEM char HTML_CLASS_W3card4[]               = "w3-card-4";
const PROGMEM char HTML_CLASS_W3container[]           = "w3-container";
const PROGMEM char HTML_CLASS_W3teal[]                = "w3-teal";
const PROGMEM char HTML_CLASS_W3border[]              = "w3-border";
const PROGMEM char HTML_CLASS_W3input[]               = "w3-input";

const PROGMEM char HTML_CLASS_W3containerTeal[]       = "w3-container w3-teal";

//styles
const PROGMEM char HTML_STYLE_width25_perc[]          = "width:25%;";
const PROGMEM char HTML_STYLE_float_left[]            = "float:left;";

//DIV section
const PROGMEM char HTML_DIV_OPEN[]                    = "<div>";
const PROGMEM char HTML_DIV_OPEN_NO_END_QUOTE[]       = "<div";
const PROGMEM char HTML_DIV_CLOSE[] PROGMEM           = "</div>";

//form
const PROGMEM char HTML_FORM_OPEN[]                   = "<form>";
const PROGMEM char HTML_FORM_OPEN_NO_END_QUOTE[]      = "<form";
const PROGMEM char HTML_FORM_CLOSE[]                  = "</form>";


//label
const PROGMEM char HTML_LABEL_OPEN[]                  = "<label>";
const PROGMEM char HTML_LABEL_OPEN_NO_END_QUOTE[]     = "<label";
const PROGMEM char HTML_LABEL_CLOSE[]                 = "</label>";


//input
const PROGMEM char HTML_INPUT_OPEN[]                  = "<input>";
const PROGMEM char HTML_INPUT_OPEN_NO_END_QUOTE[]     = "<input";
const PROGMEM char HTML_INPUT_CLOSE[]                 = "</input>";



class HtmlWidget {
public:

	virtual ~HtmlWidget();

	virtual String getName()=0;
	virtual String getHtml()=0;
	virtual String getJson()=0;
};

#endif /* LIBRARIES_PINEVENTLIB_HTMLWIDGET_H_ */
