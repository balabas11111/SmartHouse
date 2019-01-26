/*----------------------------------------------------------------------*/
var currentTab='';
var currentHeaderName='';

var currentForm=undefined;

var validateValuesHandler=undefined;

var getValuesUrl='';
var submitValuesUrl='';

function openTabGui(tabName,headerName){
	customOnGetMessage=undefined;
	openTab(tabName,headerName);
}

function openTab(tabName,headerName) {
	restartFunctionHandler=reloadCurrentSettingsTab;
	var loadMessagePreffix="Загружаю";
	
	if(currentTab==tabName){
		loadMessagePreffix="Обновляю";
	}
	
	currentTab=tabName;
	currentHeaderName=headerName;
	
    hideComponentsByClassName("settingsTab");
    document.getElementById(tabName).style.display = "block"; 

	var containerComponent=document.getElementById(tabName);
	currentForm=getComponentById(tabName+'_form');
	
	setStatusMessageComp(document.getElementById(tabName+'_msg'));
	
	document.title='Настройки устройства - '+currentHeaderName;
	
	if(containerComponent.classList.contains('settingsTab')){
		
		showStatusMessage(loadMessagePreffix+' настройки...','w3-yellow');
		validateValuesHandler=undefined;
		
		var ok=false;
		
		if(tabName=='device'){
			validateValuesHandler=validateDeviceSettingsForm;
			submitValuesUrl='/processJson?name=espSettingsBox&page=device';
			getValuesUrl='/getJson?name=espSettingsBox&page=device';
			ok=true;
		}
		
		if(tabName=='net'){
			validateValuesHandler=validateFormFunctionDefault;
			submitValuesUrl='/processJson?name=espSettingsBox&page=net';
			getValuesUrl='/getJson?name=espSettingsBox&page=net';
			ok=true;
		}
		
		if(tabName=='publish'){
			validateValuesHandler=validateFormFunctionDefault;
			submitValuesUrl='/processJson?name=espSettingsBox&page=publish';
			getValuesUrl='/getJson?name=espSettingsBox&page=publish';
			ok=true;
		}
		if(tabName=='manage'){
			ok=true;
		}
		if(ok){
			
			if(currentForm!=undefined){
				currentForm.addEventListener('submit',function(evt){evt.preventDefault();},false);
			}
			
			getValuesUrl=updateUrlEnvironment(getValuesUrl);
			submitValuesUrl=updateUrlEnvironment(submitValuesUrl);
			
			updateComponentsByAjaxCall('GET', getValuesUrl, processDeviceSettingsGet,"",undefined, 0,2000);
		}else{
			alert('Error on page!');
		}
	}
}
/*-----------------to be moved on higher level-------------------*/
function updateTemplateValue(template,clazz,field,value,suffix,disabled){
	var comp=template.querySelector('.'+clazz+'.'+field);
	if(comp!=undefined){
		comp.id=clazz+'_'+field+suffix;
		comp.disabled=disabled;
		setComponentValue(comp,value);
	
		var lbl=template.querySelector('label.htmlFor.'+field);
		if(lbl!=undefined){lbl.htmlFor=comp.id;}
	}
}

function hideComponentsByClassName(clazz){
	var x = document.getElementsByClassName(clazz);
    for (var i = 0; i < x.length; i++) {
       x[i].style.display = "none";  
    }
}

function preventFormSubmit(formName){
	var form=document.getElementById(formName);
	if(form!=undefined){
		form.addEventListener('submit', 
			function(evt){
				evt.preventDefault();
			},false);
	}else{
		console.log('form not found '+formName);
	}
}
/*------------------to be moved on high level-----------------------*/
function updateUrlEnvironment(url){
	if(localTest && url.substring(0,1)=='/'){
		url=url.substring(1,url.length)+'.json';
		url=url.replace('?', '');
	}
	return url;
}

function reloadCurrentSettingsTab(){
	openTab(currentTab,currentHeaderName);
}

/*---------------------------------device settings tab----------------------*/
function processDeviceSettingsGet(data){
	processSimpleJsonResponse(data,'');
	showStatusMessage('Загружено '+currentHeaderName,'w3-green');
}

function processDeviceSettingsSave(data){
	processSimpleJsonResponse(data.data,'');
	showStatusMessage('Сохранено и загружено '+currentHeaderName,'w3-green');
	openCommandRestartPopup();
}

function validateDeviceSettingsForm(){
	var errorMessage=validateFormFunctionDefault(currentForm);
	
	var pass=document.getElementById('espSettingsBox_accessPass');
	var conf=document.getElementById('espSettingsBox_accessPassConfirm');
	
	errorMessage+=processPassConfirmPath(pass,conf);
	
	pass=document.getElementById('espSettingsBox_settingsPass');
	conf=document.getElementById('espSettingsBox_settingsPass');
	
	errorMessage+=processPassConfirmPath(pass,conf);
	
	if(!pass || pass.value==undefined || 0===pass.length){
		errorMessage+='Доступ на страницу пароля возможен только по паролю!';
	}
			
	return errorMessage;
}

function processPassConfirmPath(pass,conf){
	var message="";
	
	markComponentValidity(pass,true);
	markComponentValidity(conf,true);
	
	if(pass.value!=conf.value){
		markComponentValidity(pass,false);
		markComponentValidity(conf,false);
		
		message="Пароль и подьверждение пароля не равны";
	}
	
	return message;
}

function ValidateIPaddress(ipaddress) {  
  if (/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/.test(ipaddress)) {  
    return (true);  
  }  
  alert("You have entered an invalid IP address!");  
  return (false)  
}

var customOnGetMessage=undefined;
/*-------------------form submit-----------*/
function submitCurrentSettingsForm(){
	showStatusMessage('Сохраняю настройки ...','w3-yellow');
	postForm(currentForm,submitValuesUrl,validateValuesHandler,constructFormDataAsJson,undefined,processDeviceSettingsSave,getStatusMessageComp());
}
