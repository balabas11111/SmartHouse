package com.balabas.smarthouse.server.view.chart;

import java.awt.Color;
import java.util.Arrays;
import java.util.List;

import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.google.common.collect.Lists;

import lombok.Getter;
import lombok.Setter;

public class ChartDataSeries {

	public static final List<Color> colors = Arrays.asList(Color.black, Color.blue, Color.cyan, Color.darkGray, Color.gray, Color.green, Color.lightGray, Color.magenta, Color.orange, Color.pink, Color.red, Color.yellow);
	
	private static final String RGB_FORMAT = "rgb(%d, %d, %d)";
	public static final String TYPE_LINE = "line";
	
	@Getter @Setter @JsonIgnore
	private Color backgroundColorObj;
	@Getter @Setter @JsonIgnore
	private Color borderColorObj;
	
	@Getter @Setter
	private String label;
	
	@Getter @Setter
	private List<ChartDataPoint> data = Lists.newArrayList();
	@Getter @Setter
	private String type = TYPE_LINE;
	
	@Getter @Setter
	private int pointRadius = 0;
	@Getter @Setter
	private boolean fill = false;
	@Getter @Setter
	private int lineTension = 0;
	@Getter @Setter
	private int borderWidth = 2;
	
	public ChartDataSeries(String label, Color color) {
		this.label = label;
		setBackgroundColorObj(color);
		setBorderColorObj(color);
	}

	public void addDataPoint(EntityFieldValue value) {
		addDataPoint(value.getDate().getTime(), value.getValue());
	}
	
	public void addDataPoint(long time, Object value) {
		data.add(new ChartDataPoint(time, value));
	}
	
	public void setBackgroundAndBorderColor(int r, int g, int b) {
		this.backgroundColorObj = new Color(r, g, b);
		this.borderColorObj = new Color(r, g, b);
	}
	
	public String getBackgroundColor() {
		return formatColor(backgroundColorObj);
	}
	
	public String getBorderColor() {
		return formatColor(borderColorObj);
	}
	
	private String formatColor(Color color) {
		return String.format(RGB_FORMAT, color.getRed(), color.getGreen(), color.getBlue());
	}
	
	public static Color getColorByIndex(int index) {
		return colors.get(index);
	}
}
