package com.balabas.telegram.bot.service;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.StringReader;
import java.io.StringWriter;
import java.nio.charset.Charset;

import org.w3c.dom.Document;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.XML;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.stereotype.Service;
import org.xml.sax.SAXException;

import com.balabas.telegram.bot.util.DeviceJsonUtil;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class TransformationServiceImpl implements InitializingBean, TransformationService {
	
	private static final String ROOT_PATH = "root";
	private static final String TEMPLATES_PATH = "xslt/";
	private static final String TEMPLATES_EXT = ".xslt";
	
	private DocumentBuilderFactory factor;
	private DocumentBuilder builder;
	private TransformerFactory transformerFactory;
	
	@Override
	public String transformToHtml(JSONObject device)
			throws JSONException, ParserConfigurationException, SAXException, IOException, TransformerException {

		String firmware = DeviceJsonUtil.getDeviceFirmware(device);
		String xml = XML.toString(device, ROOT_PATH);

		log.info("formware = " + firmware);

		String result = applyTransform(firmware, xml);

		log.info("result = " + result);

		return result;
	}

	private String applyTransform(String firmware, String xmlStr)
			throws SAXException, IOException, TransformerException {

		if (firmware == null) {
			return xmlStr;
		}
		
		String resourcePath = TEMPLATES_PATH + firmware + TEMPLATES_EXT;

		InputStream xml = new ByteArrayInputStream(xmlStr.getBytes(Charset.forName("UTF-8")));
		InputStream xsl = this.getClass().getClassLoader().getResourceAsStream(resourcePath);
		
		Document document = builder.parse(xml);
		
	    StreamSource xmlInSource = new StreamSource(new StringReader(xmlStr));

		Transformer transformer = transformerFactory.newTransformer(new StreamSource(xsl));

		StringWriter writer = new StringWriter();

		DOMSource source = new DOMSource(document);
		
		transformer.transform(xmlInSource, new StreamResult(writer));
		
		return writer.toString().replace("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", "");
	}

	@Override
	public void afterPropertiesSet() throws Exception {
		factor = DocumentBuilderFactory.newInstance();
		builder = factor.newDocumentBuilder();
		transformerFactory = TransformerFactory.newInstance();
	}

}
