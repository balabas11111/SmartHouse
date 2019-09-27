package com.balabas.smarthouse.telegram.bot.message;

import java.io.InputStream;
import java.io.StringReader;
import java.io.StringWriter;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.json.XML;
import org.springframework.stereotype.Component;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Group;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Component
public class GroupViewBuilder {

	public static final String ROOT_PATH = "root";
	public static final String TEMPLATE_PATH = "templates/bot/xslt/groupView/";
	public static final String DEFAULT_TEMPLATE_FIRMWARE = "default/";
	public static final String DEFAULT_TEMPLATE_GROUP = "default";
	public static final String TEMPLATES_EXT = ".xslt";
	public static final String NEXT_ROW = "\n";
	public static final String EMPTY_STR = "";
	
	public static final String TRANSFORMATION_ERROR = "Transformation error";
	
	public String build(Device device, Group group) throws TransformerException{
		
		String templatePath = buildTemplatePath(device.getDeviceFirmware(), group.getName());
		String xmlStr = XML.toString(group.getData(), ROOT_PATH);
		
		String text = applyTransform(templatePath, xmlStr);
		return cleanResult(text);
	}
	
	private String cleanResult(String text){
		
		text = text
				.replace("<?xml version=\"1.0\" encoding=\"UTF-8\"?>", EMPTY_STR)
				.replace("<root></root>", TRANSFORMATION_ERROR)
				.replace("<root>", TRANSFORMATION_ERROR)
				.replace("</root>", TRANSFORMATION_ERROR)
				.replaceAll("<br></br>", EMPTY_STR)
				.replaceAll("<br>", EMPTY_STR)
				.replaceAll("</br>", NEXT_ROW)
				.replaceAll("<br/>", NEXT_ROW);
		
		return text;
	}
	
	private String buildTemplatePath(String firmware, String groupName){
		String templatePath = TEMPLATE_PATH;
		
		if(!resourceExists(templatePath + firmware)){
			templatePath += DEFAULT_TEMPLATE_FIRMWARE;
		}else{
			templatePath += firmware;
		}
		
		if(!resourceExists(templatePath + groupName + TEMPLATES_EXT)){
			templatePath += DEFAULT_TEMPLATE_GROUP;
		}else{
			templatePath += groupName;
		}
		
		templatePath += TEMPLATES_EXT;
		
		log.info(templatePath," exists "+resourceExists(templatePath));
		//templates/bot/xslt/groupView/default/default.xslt
		return templatePath;
	}
	
	private String applyTransform(String templatePath, String xmlStr)
			throws  TransformerException {

		TransformerFactory transFactory = TransformerFactory.newInstance();
		
		InputStream xsl = this.getClass().getClassLoader().getResourceAsStream(templatePath);
		
	    StreamSource xmlInSource = new StreamSource(new StringReader(xmlStr));

		Transformer transformer = transFactory.newTransformer(new StreamSource(xsl));

		StringWriter writer = new StringWriter();

		transformer.transform(xmlInSource, new StreamResult(writer));
		
		return writer.toString();
	}
	
	private boolean resourceExists(String path){
		InputStream stream = this.getClass().getClassLoader().getResourceAsStream(path);
		
		return stream!=null;
	}

}
