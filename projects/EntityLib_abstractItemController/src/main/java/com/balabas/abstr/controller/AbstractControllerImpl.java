package com.balabas.abstr.controller;

import java.io.IOException;
import java.util.List;

import javax.validation.Valid;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.io.InputStreamResource;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.multipart.MultipartFile;
import org.springframework.web.servlet.mvc.support.RedirectAttributes;

import com.balabas.abstr.model.ItemAbstract;
import com.balabas.abstr.report.AbstractReporter;
import com.balabas.abstr.service.AbstractService;
import com.balabas.abstr.transformer.AbstractTransformer;

import lombok.Getter;

public class AbstractControllerImpl<T extends ItemAbstract> implements AbstractController<T> {

	@Autowired
	@Getter
	AbstractService<T> service;
	
	@Autowired
	@Getter
	AbstractReporter<T> pdfReporter;
	
	@Autowired
	@Getter
	AbstractTransformer<T> jsonTransformer;
	
	@GetMapping
    public String list(Model model) {
        return getListViewName(model);
    }
	
	@GetMapping("/create")
    public String create(Model model) {
        return getNewViewName(model, getService().create());
    }
	
    @PostMapping(value = "/add", consumes = MediaType.APPLICATION_FORM_URLENCODED_VALUE)
    public String add(@Valid @ModelAttribute T entity, BindingResult result, Model model) {
        if (result.hasErrors()) {
            return getNewViewName(model, entity);
        }
        getService().save(entity);

        return getListRedirectUrl();
    }
    
    @GetMapping("/{id}")
    public String get(@PathVariable Long id, Model model) {
        return getEditViewName(model, getInitialized(id));
    }

    @PostMapping("/{id}/update")
    public String update(@PathVariable Long id, @Valid @ModelAttribute T entity, BindingResult result, Model model) {
        if (result.hasErrors()) {
            return getEditViewName(model, entity);
        }
        
        getEntity(id);
        service.save(entity);
        
        return getListRedirectUrl();
    }

    @GetMapping("/{id}/delete")
    public String deleteBook(@PathVariable Long id, Model model) {
    	getEntity(id);
        service.delete(id);
        
        return getListRedirectUrl();
    }
    
    /*TODO: add new method which accepts only request with header Accept=application/pdf*/
	@GetMapping(value = "/pdf", produces = MediaType.APPLICATION_PDF_VALUE)
    public ResponseEntity<InputStreamResource> pdf() {
        return getReport(pdfReporter, getService().list());
    }
	
	@GetMapping("/upload")
	public String upload(Model model) throws IOException {
		return getUploadViewName(model);
	}
	
	@PostMapping("/upload")
	public String handleFileUpload(@RequestParam("uploadingFiles") MultipartFile[] uploadingFiles,
			RedirectAttributes redirectAttributes) throws IOException {

		MultipartFile file = uploadingFiles[0];
		List<T> list = jsonTransformer.transform(file);
		
		list.stream().forEach(entity -> getService().save(entity));
		
		redirectAttributes.addFlashAttribute("message",
				"You successfully uploaded " + file.getOriginalFilename() + "!");

		return getListRedirectUrl();
	}
	
}
