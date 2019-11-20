package com.balabas.smarthouse.server.view.chart;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class ChartDataPoint {

	private long t;
	private Object y;
}
