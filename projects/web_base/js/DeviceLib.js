/*-----Locale constants---*/
const LBL_MSG_LOC_DEVICE_SMART_HOUSE='Устройство !!!SmartHouse - ';
const LBL_MSG_LOC_SAVING_DATA='Сохраняю данные...';
const LBL_MSG_LOC_DATA_SAVED='Данные сохранены!';
const LBL_MSG_LOC_SERVER_SIDE_ERROR='Ошибка на стороне сервера!';
const LBL_MSG_LOC_PARSE_ERROR='Ошибка обработки ответа сервера!';
const LBL_MSG_LOC_VALIDATION_ERROR='Ошибка валидации:';
const LBL_MSG_LOC_HTTP_STATUS_EQ=' HTTP статус=';

/*-----serv constants---*/
const FORM_SUFFIX="form";
const VAL_SUFFIX="";
const ITEM_COUNT_SUFFIX="itemCount";

const ID_SUFFIX="id";
const NAME_SUFFIX="name";
const TYPE_SUFFIX="type";
const STATE_SUFFIX="state";
const DESCR_SUFFIX="descr";
const MIN_VAL_SUFFIX="minVal";
const MAX_VAL_SUFFIX="maxVal";
const FIELD_ID_SUFFIX="fieldId";
const QUEUE_SUFFIX="queue";

const CLASS_AUTO_CREATED_CHILD="autoCreatedChild";
const LABEL_PREFFIX="Label";

const localTest=false;

/*--------------------------Menu functions-----------------------------*/
function w3_open(){
	document.getElementById("leftSidebar").style.display = "block";
};
					
function w3_close(){
	document.getElementById("leftSidebar").style.display = "none";
};

/*---------------------------------------------------------------------*/

function markComponentsArrayValidity(compons,valid){
	if(compons!=undefined){
		for (var i in compons) {
			markComponentValidity(compons[i],valid);
		}
	}
}

function markComponentValidityById(id,valid){
	var comp=getComponentById(id);
	
	markComponentValidity(comp,valid);
}

function markComponentValidity(comp,valid){
	if(comp!=undefined){
		if(comp.classList!=undefined){
			if(valid){
				comp.classList.remove('w3-border-red');
			}else{
				comp.classList.add('w3-border-red');
			}
		}
	}
}

function setComponentValidityMessage(comp,message){
	if(comp!=undefined && comp.type!=undefined){
		var type=comp.type.toLowerCase();
		if(type=='input'){
			comp.setCustomValidity(message);
		}
	}
}

function markComponentValidityWithMessage(comp,valid,msg){
	markComponentValidity(comp,valid);
	setComponentValidityMessage(comp,msg);
	
	return msg;
}

function markFormAsValid(form,msgComp,valid,cleanValMsg){
	var childNodes=getComponentChildrenByTag(form,'input');
	
	if(childNodes!=undefined){
		for (var i in childNodes) {
			markComponentValidity(childNodes[i],valid);
			if(cleanValMsg){
				setComponentValidityMessage(childNodes[i],'');
			}
		}
	}
	
	showMessage(msgComp,'','w3-green');
}

/*-----------------------------------item name functions----------------*/
function getComponentIdWithPreffix(preffix,id){
	if(preffix!=undefined && preffix!='' && 0!=preffix.length){
		id=preffix+"_"+id;
	}
	return id;
}

function getComponentIdWithSuffix(id,suffix){
	if(suffix!=undefined && suffix!='' && 0!=suffix.length){
		id=id+"_"+suffix;
	}
	return id;
}

function getComponentChildrenByClass(comp,clazz){
	var childNodes = comp.getElementsByClassName(clazz);
	
	return childNodes;
}

function getComponentChildrenByTag(comp,tag){
	var childNodes = comp.getElementsByTagName(tag);
	
	return childNodes;
}

function getLabelsHtmlByForTag(labels,forTagValue){
	for(var i in labels){
		var label=labels[i];
		
		if(label!=undefined && label['htmlFor']==forTagValue){
			return label.innerHTML;
		}
	}
	return "";
}

function getFirstComponentInnerHtmlByTagValue(items,tagName,tagValue){
	for(var i in items){
		var item=items[i];
		
		if(item.hasAttribute(tagName) && item[tagName]==tagValue){
			return item.innerHTML;
		}
	}
	return "";
}

function getComponentById(id){
	return document.getElementById(id);
}

function getComponent(preffix,id,suffix){
	id=getComponentIdWithPreffix(preffix,id);
	id=getComponentIdWithSuffix(id,suffix);
	
	var comp=getComponentById(id);
	
	return comp;
}

function isChildInItems(childId,parentId,items){

	for(var j in items){
		var itemName=items[j].name;
		var candidateName=getComponentIdWithPreffix(parentId,itemName);
		
		if(candidateName==childId){
			return true;
		}
	}
	
	return false;
}

function addChildComponentIfNotExists(comp,item){
	
	var parentId=comp.id;
	var itemName=item.name;
	var itemVal=item.val;
	var itemDescr=item.descr;
	
	var newLabelId=getComponentIdWithPreffix(LABEL_PREFFIX,itemName);
	var newInputId=getComponentIdWithPreffix(parentId,itemName);
	var newLabelHtml="<b>"+itemDescr+"</b>";

	var labelComponent=getComponentById(newLabelId);
	var inputComponent=getComponentById(newInputId);
		
	if(inputComponent!=undefined && labelComponent!=undefined){
		setComponentValue(inputComponent,itemVal);
		setComponentValue(labelComponent,newLabelHtml);
		return;
	}
	
	if(comp!=undefined){
		
		var newLabel = document.createElement("Label");
		newLabel.setAttribute("id",newLabelId);
		newLabel.setAttribute("for",newInputId);
		newLabel.setAttribute("style","max-width: 240px;");
		newLabel.innerHTML = newLabelHtml ;
		
		var newInput=document.createElement("Input");
		newInput.setAttribute("id",newInputId);
		newInput.setAttribute("class","w3-input w3-border "+CLASS_AUTO_CREATED_CHILD);
		newInput.setAttribute("style","width:30%");
		newInput.setAttribute("disabled","disabled");
		newInput.setAttribute("value",itemVal);
		
		comp.appendChild(newLabel);
		comp.appendChild(newInput);
	}
	
}
/*----------------------------component values gettters/setters---------------------------*/
function getComponentValueById(id){
	return getComponentValue(getComponentById(id));
}

function setComponentValueById(id,val){
	setComponentValue(getComponentById(id),val);
}

function getComponentValue(component){
	if(component!=undefined){
		var tagName = component.tagName.toLowerCase();
		
		if (tagName == 'input'){
			if(component.type!=undefined){
				var type=component.type.toLowerCase();
				
				if(type=='checkbox'){
					if(component.hasAttribute("checked") || component.checked){
						return '1';
					}
					return '0';
				}else if(type=='date'){
					var dt=new Date(component.value);
					var tmp = Math.round(dt.getTime()/1000-dt.getTimezoneOffset()*60);
					return tmp;
				}else if(type=='time'){
					var dt=new Date(component.value);
					var tmp = Math.round(dt.getTime()/1000-dt.getTimezoneOffset()*60);
					return tmp;
				}else if(type=='datetime-local'){
					var dt=new Date(component.value);
					var tmp = Math.round(dt.getTime()/1000-dt.getTimezoneOffset()*60);
					return tmp;
				}				
				else{
					return component.value;
				}
			}else{
				return component.value;
			}
		};
		
		if(tagName=='select'){
			return component.selectedIndex;
		}
		
		if (tagName == 'a'){
			return component.href;
		};
		
		if (tagName == 'h1' || tagName == 'h2' || tagName == 'h3' || tagName == 'h4' || tagName == 'h5'
			|| tagName == 'b'){
			return component.innerHTML;
		};
	}
	
	return undefined;
}


function setComponentValue(component,val){
	if(component!=undefined){
		var tagName = component.tagName.toLowerCase();
		
		if (tagName =='label' || tagName=='div' || tagName == 'h1' || tagName == 'h2' || tagName == 'h3' || tagName == 'h4' || tagName == 'h5' || tagName == 'h6'
			|| tagName == 'b'){
			component.innerHTML=val;
			return;
		};
		
		if (tagName == 'input'){
			if(component.type!=undefined){
				var type=component.type.toLowerCase();
				
				if(type=='checkbox'){
					var chbVal=false;
					if(val==1 || val=='on' || val=='1' || val=='True' || val=='true'){
						chbVal=true;
						component.checked=chbVal;
					}
					
				}else if(type=='date'){
					const tmp=new Date(val*1000).toISOString().substring(0, 10);
					component.value=tmp;
				}else if(type=='time'){
					const tmp=new Date(val*1000).toISOString().substring(11, 19);
					component.value=tmp;
				}else if(type=='datetime-local'){
					const tmp=new Date(val*1000).toISOString().substring(0, 19);
					component.value=tmp;
				}				
				else{
					component.value=val;
				}
			}else{
				component.value=val;
			}
		};
		
		if(tagName == 'select'){
			component.selectedIndex=val;
		}
		
		if (tagName == 'a'){
			component.href=val;
			showComponent(component.id);
		};
	}
}

function updateComponentValue(preffix,id,suffix,value){
	
	var comp=getComponent(preffix,id,suffix);
	setComponentValue(comp,value);
}
/*--------------------process all json -------------------------------*/
var jsonHandlers={};
var validationChains={};

function registerDefaultHandlers(){
	console.log('Register default handlers');
	registerJsonHandler('processSimpleJson',processSimpleJson);
	registerJsonHandler('processSimpleJsonRecreateChildren',processSimpleJsonRecreateChildren);
	registerJsonHandler('processJsonOnImageComponent',processJsonOnImageComponent);
	registerJsonHandler('noJsonProcess',noJsonProcess);
}

function registerJsonHandler(name,handler){
	console.log('Register handler ',name);
	jsonHandlers[name]=handler;
}

function getJsonHandlerByName(name){
	if(name==undefined){
		return undefined;
	}

	return jsonHandlers[name];
}

/*--------------------json response processors----------------*/
function noJsonProcess(data){}

function processSimpleJson(data){
	processSimpleJsonResponse(data,'');	
}

function processSimpleJsonResponse(data,preffix){
	var name=data.name;
	var itemCount=data.itemCount;
	var descr=data.descr;
	
	console.log('Process name='+name+' itemCOunt='+itemCount+' descr='+descr);
	
	updateComponentValue(preffix,name,DESCR_SUFFIX,descr);
	updateComponentValue(preffix,name,ITEM_COUNT_SUFFIX,itemCount);
		
	/*process children*/
	var items=data.items;
	for(var i in items){
		var itemName=name+"_"+items[i].name;
		var itemVal=items[i].val;
		var itemDescr=items[i].descr;
				
		updateComponentValue(preffix,itemName,VAL_SUFFIX,itemVal);
		updateComponentValue(preffix,itemName,DESCR_SUFFIX,itemDescr);
	}
}

function processAllJson(allSensorData){
	var sensors=allSensorData.sensors;
	
	for(var i in sensors){
		var data=sensors[i];
		
		if(data!=undefined && data!=null){
			var sensorName=data.name;
			
			var dataHandler=getJsonHandlerByName(sensorName);
			
			if(dataHandler!=undefined && dataHandler!=null){
				dataHandler(data);
			}else{
				console.log('Json error. handler not found for :');
				console.log(data);
				console.log('noJsonProcess will be used as handler');
				registerJsonHandler(sensorName,noJsonProcess);
			}
			
		}else{
			console.log('sensor value not found. Json error');
		}
	}
}

function processSettingsJson(data){
	
	processSimpleJson(data);
	
	var devId=data.DeviceId;
	var devLocation=data.DeviceLocation;
	
	if(devId!=undefined && devId!='' && devLocation!=undefined && devLocation!=''){
		pageHeader=LBL_MSG_LOC_DEVICE_SMART_HOUSE+devId+' '+devLocation;
		document.title=pageHeader;
	}
}

function processSimpleJsonRecreateChildren(data){
	var items=data.items;
	var id=data.name;
	var itemCount=data.itemCount;
	var descr=data.descr;
	
	console.log('Process name='+name+' itemCOunt='+itemCount+' descr='+descr);
	
	var comp=getComponentById(id);
	
	updateComponentValue("",id,DESCR_SUFFIX,descr);
	updateComponentValue("",id,ITEM_COUNT_SUFFIX,itemCount);
	
	var childNodes=getComponentChildrenByClass(comp,CLASS_AUTO_CREATED_CHILD);
	
	for(var n in childNodes){
		/*check if unknown element is displayed by no data received from device. Just remove it*/
		var child=childNodes[n];
		
		if(!isChildInItems(child.id,id,items)){
			if(child!=undefined && child.id!=undefined){
				console.log('child removed '+child.id);
							
				var labelId=getComponentIdWithPreffix(LABEL_PREFFIX,child.id);
				var label=getComponentById(labelId);
				
				if(label!=undefined && child!=undefined){
					child.remove();
					label.remove();
				}
			}
		}
	}
		
	for(var i in items){
		addChildComponentIfNotExists(comp,items[i]);
	}
}

function processJsonOnImageComponent(data){
	
	var id=data.name;
	var descr=data.descr;
	var itemCount=data.itemCount;
	
	var comp=document.getElementById(id);
	var lookUpClassName=data.name+"_Image";
	
	updateComponentValue("",id,DESCR_SUFFIX,descr);
	updateComponentValue("",id,ITEM_COUNT_SUFFIX,itemCount);
	
	if(comp!=undefined){
		var childNodes = comp.getElementsByClassName(lookUpClassName);
		
		for(i in childNodes){
			hideComponent(childNodes[i].id);
		}
		var items=data.items;
		
		for(var i in items){
			var val=items[i].val;
			
			var ind=val.indexOf('.');
			
			if(ind!=-1){
				val=val.substr(0,ind);
			}
			
			var imageName=data.name+"_"+val;
			showComponent(imageName);
		}
	}
}
/*---------------------------AJAX functions------------------------------------*/
function isHttpStatusOk(status){
	/*Standard http 200 Ok*/
	if(status==200){	return true;	}
	/*local files only*/
	if(localTest && status==0){	return true; }
	
	return false;
}

var lastOk=true;

function handleHttpRequestStatus(ok,tooltip){
	if(document.getElementById('httpStatus_div')==undefined){return;}
	
	var statImg=document.getElementById('httpStatusImg');
	var progrImg=document.getElementById('httpProgressImg');
	var errImg=document.getElementById('httpErrorImg');
	
	var tooltipDiv=document.getElementById('httpStatusTooltip');
	
	if(!(ok==undefined && tooltip==undefined)){
	
		if(ok && (tooltip==undefined || tooltip=='')){tooltip='Подключено ОК';}
		if(!ok && tooltip==undefined){tooltip='Ошибка подключения';}
		var imageClass='http-'+(ok?'ok':'error')+'-icon48';
		
		statImg.setAttribute('class',imageClass);
		tooltipDiv.innerHTML=tooltip;
		
		statImg.style.display = "block";
		progrImg.style.display = "none";
		errImg.style.display = "none";
		
		lastOk=ok;
	}else{
			statImg.style.display = "none";
		if(lastOk){
			tooltipDiv.innerHTML='Соединяюсь с устройством';
			progrImg.style.display = "block";
			errImg.style.display = "none";
		}else{
			tooltipDiv.innerHTML='Переподключение после ошибки';
			progrImg.style.display = "none";
			errImg.style.display = "block";
		}
	}
}

function updateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut){
	handleHttpRequestStatus();
	var request = new XMLHttpRequest();
	
	var formData = new FormData();
	formData.append('val', val);
	if(sensor!=undefined){
		formData.append('sensor', sensor);
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
						handler(json);
					}catch(err){
						ok=false;
						handleHttpRequestStatus(false,'Ошибка: '+err);
					}
									
					if(timeout>0){
						var reloadTime=timeout;
						addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut,reloadTime);
					}
					
					handleHttpRequestStatus(true);
				}else{
					if(errorTimeOut>0){
						var reloadTime=errorTimeOut;
						addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut, reloadTime);
					}else{
						handleHttpRequestStatus(false,'Ошибка запроса устройства');
					}
				}
			}else{
				if(this.status!=200){
					handleHttpRequestStatus(false,'Ошибка: статус='+this.status);
				}
			};
		};
	request.send(formData);
};

function addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut, reloadTime){
	if(reloadTime!='' && reloadTime!=0 && reloadTime>=2000){
		setTimeout(function(){updateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut);}, reloadTime);
	}
};
/*-------------------------------form submission---------------------*/
function postForm(form,url,validateFormFunction,constructFormDataFunction,responseValidateFunc,resultProcessHandler,msgComp){
	var errorMessage='';
	var isValidForm=true;
	
	if(validateFormFunction!=undefined){
		errorMessage=validateFormFunction(form);
		isValidForm=(errorMessage=='');
	}
	
	if(isValidForm){
		
		if(constructFormDataFunction==undefined){
			constructFormDataFunction=constructFormDataDefault;
		}
		
		var formData = constructFormDataFunction(form);
		
		showMessage(msgComp,LBL_MSG_LOC_SAVING_DATA,'w3-yellow');
		
		var request = new XMLHttpRequest();
		request.open("POST", url, true);
		request.onreadystatechange  = 
			function(){
				if(this.readyState == 4){
					
					var errMsg='';
					
					if (isHttpStatusOk(this.status)){
						if(responseValidateFunc!=undefined){
							errMsg=responseValidateFunc(this.responseText);
						}
					}else{
						errMsg=LBL_MSG_LOC_HTTP_STATUS_EQ+this.status+' <br>';
					}
					
					if(errMsg==undefined || errMsg.length==0){
						try{
							var json = JSON.parse(this.responseText);
							var resultMsg=resultProcessHandler(json);
							
							if(resultMsg==undefined || resultMsg==''){
								resultMsg=LBL_MSG_LOC_DATA_SAVED;
							}
							
							showMessage(msgComp,resultMsg,'w3-green');
						}catch(err){
							showMessage(msgComp,LBL_MSG_LOC_PARSE_ERROR+' '+err,'w3-red');
						}
					} else {
						showMessage(msgComp,/*LBL_MSG_LOC_SERVER_SIDE_ERROR+' '+*/errMsg,'w3-red');
					};
				};
			};
		request.send(formData);
	}else{
		errorMessage='<strong>'+LBL_MSG_LOC_VALIDATION_ERROR+'</strong>  <br><br>'+errorMessage;
		showMessage(msgComp,errorMessage,'w3-red');
	}
}
/*------------------Validators----------------------------------*/
function isStatusMessageResponseOk(httpStatus){
	return (httpStatus>=200 && httpStatus<300);
}

function validateStatusMessageDefault(statusMessage){
	try {
		console.log('formPost response'+statusMessage);
		var json=JSON.parse(statusMessage);
		if(json.statusInt!=0){
			return json.status;
		}
    } catch (e) {
        return statusMessage+'<br>';
    }
}

function validateFormFunctionDefault(form){
	var validateMessage='';
	
	var isValidForm = form.checkValidity();
	var childNodes=getComponentChildrenByTag(form,'input');
	
	if(!isValidForm){
		var labels=getComponentChildrenByTag(form,'label');
		
		for(var i=0;i<childNodes.length;i++){
			var child=childNodes[i];
			var valid=child.validity.valid;
			
			markComponentValidity(child, valid);
			
			if(!valid){
				var lbl=getLabelsHtmlByForTag(labels,child.id);
				if(lbl!=''){
					validateMessage+=lbl+': ';
				}
				validateMessage+=child.validationMessage+' <br>'
			}
		}
	}else{
		for(var i=0;i<childNodes.length;i++){
			markComponentValidity(childNodes[i], true);
		}
	}
	
	return validateMessage;
}
/*-------------------------Construct forms----------------*/
function constructFormDataDefault(form){
	var formData = new FormData(form);
	
	var formInputs = getComponentChildrenByTag(form,'input');
	
	for(var i=0;i<formInputs.length;i++){
		var child=formInputs[i];
		if(child!=undefined && child.tagName!=undefined && child.type!=undefined){
			
			var type=child.type.toLowerCase();
			
			if(type=='checkbox'){
				var chbVal=child.checked;
				
				if(chbVal!=true){
					formData.append(child.name, false);
				}else{
					formData.set(child.name,true);
				}
			}
		}
	}
	
	return formData;
}

function constructFormData_JSONprocessor(target,page,json){
	/*--construct standard form to process in device JSONprocessors*/
	if(page==undefined){page=''};
	
	const REMOTE_TARGET='remote_target';
	const REMOTE_PAGE='remote_page';
	const VAL_JSON='val_json';
	
	var formData = new FormData();
	formData.append(REMOTE_TARGET,target);
	formData.append(REMOTE_PAGE,page);
	formData.append(VAL_JSON,json);	
	
	console.log('JSONprocessor name='+target+' json='+json);
	
	return formData;
}

function constructFormDataAsJson(form){
	
	const PAGE_SUF='_page';
	const TARGET_SUF='_target';
	const REMOTE_TARGET='remote_target';
	const VAL_JSON='val_json';
	
	var formData = new FormData();
	
	var formInputs = getComponentChildrenByTag(form,'input');
	var str='{';
	var pageName='';
	
	for(var i=0;i<formInputs.length;i++){
		var child=formInputs[i];
		if(child!=undefined && child.tagName!=undefined && child.type!=undefined
				&& child.name!=undefined && child.id!=undefined){

				inputName=child.name;
				
				if(inputName!=undefined && inputName!=''){
					inputValue=getComponentValue(child);
					str=str+'"'+inputName+'": "'+inputValue+'",';
				}
		}
	}
	
	if(str.substring(str.length - 1)==','){
		str=str.substring(0,str.length-1);
	}
	
	str+='}';
	
	var target=getComponentValueById(form.id+TARGET_SUF);
	var page=getComponentValueById(form.id+PAGE_SUF);
	
	return constructFormData_JSONprocessor(target,page,str);
}

/*-------------------CheckBox list---------------*/

function arrayToCheckBoxList(component,namePreffix,valArray,nameArray,editable,clazz,style){
	if(component!=undefined){
	
		component.innerHTML = '';
		var cbx=null;
		var lbl=null;
		
		var itemCount=valArray.length;
		
		if(nameArray.length>itemCount){
			itemCount=nameArray.length;
		}
		
		for(i = 0; i<itemCount; i++){ 
			
			var cbxname='cb_'+namePreffix+'_'+i;
			cbx = document.createElement('input');
			cbx.setAttribute('id',cbxname);
			cbx.setAttribute('name',cbxname);
			cbx.setAttribute('type','checkbox');
			if(clazz!=undefined && clazz.length>0){
				cbx.setAttribute('class',clazz);
			}
			if(style!=undefined && style.length>0){
				cbx.setAttribute('style',style);
			}
			setComponentValue(cbx,valArray[i]);
			if(editable==undefined || !editable){
				cbx.setAttribute("disabled","disabled");
			}
			/*cbx.setAttribute('class',className);*/
			
			lbl = document.createElement('label');
			lbl.setAttribute('id','lbl_'+cbxname);
			lbl.setAttribute('for',cbxname);
			setComponentValue(lbl,nameArray[i]);
		    
		    component.appendChild(cbx);
		    component.appendChild(lbl);
		}
	}
}

function checkBoxListToString(componentId){
	var component=getComponentById(componentId);
	var result = '';	
	
	var inputs=getComponentChildrenByTag(component,'input');
	
	 for (var i = 0; i < inputs.length; ++i) {
		 if(inputs[i]!=undefined && inputs[i].type=='checkbox'){
			 var val=getComponentValue(inputs[i]);
			 
			 if(val=='1' || val=='0'){
				 result+=val;
				 result+=',';
			 }
		 }
	 }
	 
	if(result.substring(result.length - 1)==','){
		 result=result.substring(0,result.length-1);
	}
	 
	 return result;
}

function checkBoxListToArray(component){
	var result = [];	
	
	var inputs=getComponentChildrenByTag(component,'input');
	var j=0;
	
	 for (var i = 0; i < inputs.length; ++i) {
		 if(inputs[i]!=undefined && inputs[i].type=='checkbox'){
			 var val=getComponentValue(inputs[i]);
			 
			 if(val=='1' || val=='0'){
				 result[j]=val;
				 j++;
			 }
		 }
	 }
	 
	 return result;
}

function putItemsToComboCox(component,items,selectedIndex){
	if(component==undefined || items==undefined || items.length==undefined){
		return;
	}
	component.innerHTML = '';
	var opt = null;
	
	for(i = 0; i<items.length; i++){ 
	    opt = document.createElement('option');
	    opt.value = i;
	    opt.innerHTML = items[i];
	    component.appendChild(opt);
	}
	
	if(selectedIndex==undefined || selectedIndex>items.length-1){
		selectedIndex=-1;
	}
	component.selectedIndex=selectedIndex;
}

