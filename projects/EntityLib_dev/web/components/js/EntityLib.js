
var dataModel={};

const dataModelRootUrl = '/info';
var source = new EventSource('/events');

var currentView = 'sensors';

/*--------------------------Menu functions-----------------------------*/
function w3_open(){
	document.getElementById("leftSidebar").style.display = "block";
};
					
function w3_close(){
	document.getElementById("leftSidebar").style.display = "none";
};
/*---------------------------AJAX functions------------------------------------*/
function isHttpStatusOk(status){
	/*Standard http 200 Ok*/
	if(status==200){	return true;	}
	/*local files only*/
	if(localTest && status==0){	return true; }
	
	return false;
}

function onLoadPageComplete(){
	sendRequest("GET", dataModelRootUrl, handleRootReceive, "exec","root", 0, 2000);
	initEventSourceChannel();
}

function handleRootReceive(json){
	processTemplates(currentView);
}

function initEventSourceChannel(){
	if (!!window.EventSource) {
	
	  source.addEventListener('open', function(e) {
	    console.log("Events Connected");
	  }, false);
	
	  source.addEventListener('error', function(e) {
	    if (e.target.readyState != EventSource.OPEN) {
	      console.log("Events Disconnected");
	    }
	  }, false);
	
	  source.addEventListener('message', function(e) {
	    console.log("message", e.data);
	  }, false);
	
	  source.addEventListener('myevent', function(e) {
	    console.log("myevent", e.data);
	  }, false);
	}
}

function processTemplates(group){
	var childNodes = dataModel.data[group];
	
	document.getElementById("mainContent").innerHTML='';
	
	if(childNodes!=undefined){
		for (var i in childNodes) {
			console.log("child= ", childNodes[i]);
			console.log("template =",getTemplateId(childNodes[i]));
			
			//sensors.bme280.templ
			
		}
	}
}

function getTemplateId(item){
	var result=item.group+'.'+item.name+'.templ';
	return result;
}

function sendRequest(requestmethod, url, handler, val,sensor, timeout, errorTimeOut){
	console.log("XHR send request "+val+"="+sensor);
	
	var request = new XMLHttpRequest();
	
	var formData = new FormData();
	if(sensor!=undefined){
		formData.append(val, sensor);
	}
	
	request.open(requestmethod, url, true);
	request.timeout=6000;
	request.ontimeout = function(){
		console.log("XHR request timeout "+request.status);
		handleHttpRequestStatus(false,'Ошибка: Превышен интервал ожидания');
	};
	request.onerror = function(){
		console.log("XHR request error "+request.status);
		handleHttpRequestStatus(false,'Ошибка запроса устройства');
	};
	
	request.onreadystatechange  = 
		function() {
			if(this.readyState == 4){
								
				if (isHttpStatusOk(this.status)){
					var ok=true;
					
					try{
						var json = JSON.parse(this.responseText);
						dataModel = json;
						if(handler!=undefined){
							handler(json);
						}
						console.log("XHR result = "+this.responseText);
					}catch(err){
						ok=false;
						console.log("ERROR = "+err);
					}
									
					if(timeout>0){
						var reloadTime=timeout;
						addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut,reloadTime);
					}
					
					console.log("XHR complete = ");
				}else{
					if(errorTimeOut>0){
						var reloadTime=errorTimeOut;
						addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut, reloadTime);
					}else{
						console.log('Ошибка запроса устройства');
					}
				}
			}else{
				if(this.status!=200){
					console.log('Ошибка: статус='+this.status);
				}
			};
		};
	request.send(formData);
};

function addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut, reloadTime){
	if(reloadTime!='' && reloadTime!=0 && reloadTime>=2000){
		setTimeout(function(){sendRequest(requestmethod, url, handler, val,sensor, timeout, errorTimeOut);}, reloadTime);
	}
};