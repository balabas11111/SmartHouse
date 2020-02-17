package com.balabas.entities.report;

import org.springframework.stereotype.Service;

import com.balabas.abstr.report.AbstractReporter;
import com.balabas.abstr.report.ReporterPdf;
import com.balabas.entities.model.PhoneNumber;
import com.balabas.entities.model.PhoneUser;
import com.itextpdf.text.Element;
import com.itextpdf.text.pdf.PdfPTable;

@Service
public class ReporterPdfPhoneUser extends ReporterPdf<PhoneUser> implements AbstractReporter<PhoneUser> {

	@Override
	protected void headerToReportIdentifiable(PdfPTable table, PhoneUser entity) {
		
		super.headerToReportIdentifiable(table, entity);
		
		addHeadCell(table, "Phone Name" );
		addHeadCell(table, "Phone" );
    	addHeadCell(table, "Company name" );
    }
	
	@Override
	protected void entityToReportIdentifiable(PdfPTable table, PhoneUser entity) {
		
		String lastUserName = "";
		for(PhoneNumber number: entity.getNumbers()) {
			if(!lastUserName.equals(entity.getName())) {
				lastUserName = entity.getName();
				super.entityToReportIdentifiable(table, entity);
			} else {
				addCell(table, "", Element.ALIGN_MIDDLE, Element.ALIGN_CENTER, -1, -1);
		    	addCell(table, "", Element.ALIGN_MIDDLE, Element.ALIGN_LEFT, 5, -1);
			}
			
			addCell(table, number.getName(), Element.ALIGN_MIDDLE, Element.ALIGN_RIGHT, -1, 5);
			addCell(table, number.getNumber(), Element.ALIGN_MIDDLE, Element.ALIGN_RIGHT, -1, 5);
			addCell(table, number.getCompany().getName(), Element.ALIGN_MIDDLE, Element.ALIGN_RIGHT, -1, 5);
		}
    }
	
	@Override
	protected int[] getWidths() {
    	return new int[]{1, 3, 3, 3, 3};
    }
}
