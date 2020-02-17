package com.balabas.entities.report;

import org.springframework.stereotype.Service;

import com.balabas.abstr.report.AbstractReporter;
import com.balabas.abstr.report.ReporterPdf;
import com.balabas.entities.model.PhoneCompany;

@Service
public class ReporterPdfPhoneCompany extends ReporterPdf<PhoneCompany> implements AbstractReporter<PhoneCompany> {

    protected int[] getWidths() {
    	return new int[]{1, 3};
    }
}
