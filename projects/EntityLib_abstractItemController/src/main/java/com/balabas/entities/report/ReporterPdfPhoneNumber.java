package com.balabas.entities.report;

import org.springframework.stereotype.Service;

import com.balabas.abstr.report.AbstractReporter;
import com.balabas.abstr.report.ReporterPdf;
import com.balabas.entities.model.PhoneNumber;
import com.itextpdf.text.Element;
import com.itextpdf.text.pdf.PdfPTable;

@Service
public class ReporterPdfPhoneNumber extends ReporterPdf<PhoneNumber> implements AbstractReporter<PhoneNumber> {

	@Override
	protected void headerToReportIdentifiable(PdfPTable table, PhoneNumber entity) {
		super.headerToReportIdentifiable(table, entity);
		
		addHeadCell(table, "Number" );
    	addHeadCell(table, "Company name" );
    }
	
	@Override
	protected void entityToReportIdentifiable(PdfPTable table, PhoneNumber entity) {
		super.entityToReportIdentifiable(table, entity);

		addCell(table, entity.getNumber(), Element.ALIGN_MIDDLE, Element.ALIGN_RIGHT, -1, 5);
    	addCell(table, entity.getCompany().getName(), Element.ALIGN_MIDDLE, Element.ALIGN_RIGHT, -1, 5);
    }

	@Override
	protected int[] getWidths() {
    	return new int[]{1, 3, 3, 3};
    }
}
