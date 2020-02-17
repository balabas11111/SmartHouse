package com.balabas.abstr.report;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.util.List;

import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;

import com.balabas.abstr.model.IIdentifiable;
import com.balabas.abstr.model.ItemAbstract;
import com.itextpdf.text.Document;
import com.itextpdf.text.DocumentException;
import com.itextpdf.text.Element;
import com.itextpdf.text.Font;
import com.itextpdf.text.FontFactory;
import com.itextpdf.text.Phrase;
import com.itextpdf.text.pdf.PdfPCell;
import com.itextpdf.text.pdf.PdfPTable;
import com.itextpdf.text.pdf.PdfWriter;

import lombok.extern.log4j.Log4j2;

@Log4j2
public abstract class ReporterPdf<T extends ItemAbstract> implements AbstractReporter<T> {

	@Override
	public MediaType getContentType() {
		return MediaType.APPLICATION_PDF;
	}
	
	@Override
	public HttpHeaders getHeaders() {
		HttpHeaders headers = new HttpHeaders();
		headers.add("Content-Disposition", "inline; filename=" + getFileName());
		
		return headers;
	}
	
	@Override
    public ByteArrayInputStream getReport(List<T> entities) {

        Document document = new Document();
        ByteArrayOutputStream out = new ByteArrayOutputStream();

        try {

            PdfPTable table = new PdfPTable(getColumnCount());
            table.setWidthPercentage(60);
            table.setWidths(getWidths());

            headerToReport(table);
            
            entities.stream().forEach(entity ->	entityToReport(table, entity));

            PdfWriter.getInstance(document, out);
            document.open();
            document.add(table);

            document.close();

        } catch (DocumentException ex) {

            log.error("Error occurred: {0}", ex);
        }

        return new ByteArrayInputStream(out.toByteArray());
    }
    
    public void headerToReport(PdfPTable table) {
    	headerToReportIdentifiable(table, null);
    }
    
    public void entityToReport(PdfPTable table, T entity) {
    	entityToReportIdentifiable(table, entity);
    }
    
    protected String getFileName() {
    	return "entitiesReport.pdf";
    }
    
    protected void headerToReportIdentifiable(PdfPTable table, T entity) {
    	addHeadCell(table, IIdentifiable.PROP_ID );
    	addHeadCell(table, IIdentifiable.PROP_NAME );
    }
    
    protected void entityToReportIdentifiable(PdfPTable table, T entity) {
    	addCell(table, entity.getId().toString(), Element.ALIGN_MIDDLE, Element.ALIGN_CENTER, -1, -1);
    	addCell(table, entity.getName().toString(), Element.ALIGN_MIDDLE, Element.ALIGN_LEFT, 5, -1);
    }
    
    protected void addHeadCell(PdfPTable table, String text) {
        Font headFont = FontFactory.getFont(FontFactory.HELVETICA_BOLD);

        PdfPCell hcell = new PdfPCell(new Phrase(text, headFont));
        hcell.setHorizontalAlignment(Element.ALIGN_CENTER);
        table.addCell(hcell);
    }
    
    protected void addCell(PdfPTable table, String text, int vertAlign, int horAlign, int paddingLeft, int paddingRight) {
    	PdfPCell cell = new PdfPCell(new Phrase(text));
    	if(paddingLeft > 0) {
    		cell.setPaddingLeft(paddingLeft);
    	}
    	if(vertAlign > -1) {
    		cell.setVerticalAlignment(vertAlign);
    	}
    	if(horAlign > -1) {
    		cell.setHorizontalAlignment(horAlign);
    	}
        table.addCell(cell);
    }
    
    protected int getColumnCount() {
    	return getWidths().length;
    }
    
    protected abstract int[] getWidths();

}
