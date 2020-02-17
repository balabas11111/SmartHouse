package com.balabas.abstr.controller;

import java.io.ByteArrayInputStream;
import java.util.List;

import org.springframework.core.io.InputStreamResource;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.ui.Model;

import com.balabas.abstr.model.ItemAbstract;
import com.balabas.abstr.report.AbstractReporter;
import com.balabas.abstr.service.AbstractService;

public interface AbstractController<T extends ItemAbstract> {

	public static final String MODEL_NAME_LIST = "entities"; 
	public static final String MODEL_NAME_ITEM = "entity";
	
	public static final String MODEL_NAME_ENTITY_CLASS = "entityClass";
	
	public static final String MODEL_NAME_ENTITY_LABEL_TITLE = "entityLabel_title";
	public static final String MODEL_NAME_ENTITY_LABEL_HEADER = "entityLabel_header";
	public static final String MODEL_NAME_ENTITY_CLASS_ACTION = "entityClass_action";
	
	public static final String VIEW_NAME_PREFFIX_REDIRECT = "redirect:/";
	public static final String VIEW_NAME_PREFFIX_ABSTRACT = "abstract";
	
	public static final String VIEW_NAME_ROOT = "/";
	public static final String VIEW_NAME_LIST = "/list";
	public static final String VIEW_NAME_NEW = "/entity";
	public static final String VIEW_NAME_EDIT = "/entity";
	public static final String VIEW_NAME_DELETE = "/delete";
	public static final String VIEW_NAME_UPLOAD = "/upload";
	
	AbstractService<T> getService();
	
	AbstractReporter<T> getPdfReporter();
	
	default String getEntityPreffix() {
		return getService().getEntityClass();
	}
	
	default String getUploadHeader() {
		return "Upload JsonFile " + getEntityPreffix();
	}
	
	default String getListTitle() {
		return "List of " + getEntityPreffix();
	}
	
	default String getNewHeader(T entity) {
		return "New : " + getEntityPreffix();
	}
	
	default String getEditHeader(T entity) {
		return "Edit " + getEntityPreffix();
	}
	
	default String getListRedirectUrl() {
		String result = VIEW_NAME_PREFFIX_REDIRECT + getEntityPreffix() + VIEW_NAME_ROOT; 
		return result;
	}
	
	default String getAddUrl(T entity) {
		return "/" + getEntityPreffix() + "/add";
	}
	
	default String getUpdateUrl(T entity) {
		return "/" + getEntityPreffix() + "/" + entity.getId() + "/update";
	}
	
	default String getListViewName(Model model) {
		return getListViewName(model, getService().list());
	}
	
	default public String getUploadViewName(Model model) {
		model.addAttribute(MODEL_NAME_ENTITY_LABEL_TITLE, getUploadHeader());
		model.addAttribute(MODEL_NAME_ENTITY_LABEL_HEADER, getUploadHeader());
				
		addEntityPreffixToModel(model);
		
		return VIEW_NAME_PREFFIX_ABSTRACT + VIEW_NAME_UPLOAD;
	}
	
	default String getListViewName(Model model, List<T> list) {
		model.addAttribute(MODEL_NAME_ENTITY_LABEL_TITLE, getListTitle());
		model.addAttribute(MODEL_NAME_ENTITY_LABEL_HEADER, getListTitle());
		
		addEntityPreffixToModel(model);
		addListToModel(model, list);
		
		String viewName = getEntityPreffix() + VIEW_NAME_LIST; 
		
		return viewName;
	}
	
	default String getNewViewName(Model model, T entity) {
		model.addAttribute(MODEL_NAME_ENTITY_LABEL_TITLE, getNewHeader(entity));
		model.addAttribute(MODEL_NAME_ENTITY_LABEL_HEADER, getNewHeader(entity));
		model.addAttribute(MODEL_NAME_ENTITY_CLASS_ACTION, getAddUrl(entity));
		
		addEntityPreffixToModel(model);
		addItemToModel(model, entity);
		
		String viewName = getEntityPreffix() + VIEW_NAME_NEW; 
		
		return viewName;
	}
	
	default String getEditViewName(Model model, T entity) {
		model.addAttribute(MODEL_NAME_ENTITY_LABEL_TITLE, getEditHeader(entity));
		model.addAttribute(MODEL_NAME_ENTITY_LABEL_HEADER, getEditHeader(entity));
		model.addAttribute(MODEL_NAME_ENTITY_CLASS_ACTION, getUpdateUrl(entity));
		
		addEntityPreffixToModel(model);
		addItemToModel(model, entity);
		return getEntityPreffix() + VIEW_NAME_EDIT;
	}
	
	default void addEntityPreffixToModel(Model model) {
		model.addAttribute(MODEL_NAME_ENTITY_CLASS, getEntityPreffix());
	}
	
	default void addListToModel(Model model, List<T> list) {
		model.addAttribute(MODEL_NAME_LIST, list);
	}
	
	default void addItemToModel(Model model, T entity) {
		model.addAttribute(MODEL_NAME_ITEM, entity);
	}
	
	default T getEntity(Long id) {
		return getService().loadOpt(id)
            .orElseThrow(() -> new IllegalArgumentException("Invalid "+getEntityPreffix()+" Id:" + id));
	}
	
	default T getInitialized(Long id) {
		return getService().getInitialized(id);
	}
	
	default ResponseEntity<InputStreamResource> getReport(AbstractReporter<T> reporter, List<T> entities) {
		ByteArrayInputStream bis = reporter.getReport(entities);

        HttpHeaders headers = reporter.getHeaders();

        return getResponseEntity(headers, bis, reporter.getContentType());

	}
	
	default ResponseEntity<InputStreamResource> getResponseEntity(HttpHeaders headers,  ByteArrayInputStream bis, MediaType mediaType) {
		return ResponseEntity
                .ok()
                .headers(headers)
                .contentType(mediaType)
                .body(new InputStreamResource(bis));
	}
}
