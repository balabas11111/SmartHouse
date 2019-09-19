package com.balabas.telegram.bot.service;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;

import org.json.JSONException;
import org.json.JSONObject;
import org.xml.sax.SAXException;

public interface TransformationService {

	String transformToHtml(JSONObject device) throws JSONException, ParserConfigurationException, SAXException, IOException, TransformerException;
}
