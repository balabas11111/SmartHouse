function submitForm(vievName,formName,updateName){
	var f = document.forms[formName];
	var isValidForm = f.checkValidity();
	
	if(isValidForm){
		f.addEventListener('submit', function(evt){evt.preventDefault();},false);
		var formData = new FormData(f);
		formData.append("form", formName);
		formData.append("vievName", vievName);
		document.getElementById(updateName).innerHTML = 'Обновляю данные';
		var request = new XMLHttpRequest();
		request.open("POST", "setup", true);
		request.onreadystatechange  = 
			function(){
				if(this.readyState == 4){
					if (this.status == 200){
						document.getElementById(updateName).innerHTML = this.responseText;
						document.getElementById(formName).addEventListener('submit',function(e){e.preventDefault();},false);
						if(postUpdateForm){ postUpdateForm();}
					} else {
						document.getElementById(updateName).innerHTML = this.responseText;
						document.getElementById(formName).addEventListener('submit',function(e){e.preventDefault();},false);
						if(postUpdateForm) postUpdateForm();
					};
				};
			};
		request.send(formData);
	}else{
		document.getElementById('msgContent').innerHTML ='Некоторые значения неверны';
		}
};

function loadForm(viewName,updateName){
	updateHtmlComponentByAjax("vievName",viewName,updateName,"POST","setup","");
};

function updateHtmlComponentByAjax(paramName,paramValue,updateName,requestmethod,url,reloadIfCompClass){
	document.getElementById(updateName).innerHTML = 'Обновляю данные';
	var formData = new FormData();
	formData.append(paramName, paramValue);
	var request = new XMLHttpRequest();
	
	request.open(requestmethod, url, true);
	request.onreadystatechange  = 
		function() {
			if(this.readyState == 4){
				if (this.status == 200){
					document.getElementById(updateName).innerHTML = this.responseText;
					if(reloadIfCompClass!=""){
						if(document.getElementById(updateName).hasAttribute("reloadInterval")==true){
							var reloadInterval = document.getElementById(updateName).getAttribute("reloadInterval");
							addPostponedUpdate(updateName,reloadIfCompClass,reloadInterval);
						}
					}
				}else{
					document.getElementById(updateName).innerHTML = this.responseText;
				}
			};
		};
	request.send(formData);
};

function reloadSettingsWidgets(){
	reloadAllWidgetsByClassname("SettingsWidgetESP");
};

function reloadMeasurerWidgets(){
	reloadAllWidgetsByClassname("MeasurerWidgetESP");
};

function reloadAllWidgetsByClassname(widgetClass){
	var widgets,i;
	
	widgets=document.getElementsByClassName(widgetClass);
	
	for (i = 0; i < widgets.length; i++) {
	    updateWidgetByAjax(widgets[i].id,widgetClass);
	}
	
};

function updateWidgetByAjax(widgetId,widgetClass){
	updateHtmlComponentByAjax("widgetName",widgetId,widgetId,"POST",widgetClass,widgetClass);
}

function addPostponedUpdate(widgetId,widgetClass,timeout){
	setTimeout(function(){updateWidgetByAjax(widgetId,widgetClass);}, timeout);
}

