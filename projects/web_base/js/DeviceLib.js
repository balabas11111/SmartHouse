
const MSG_SUFFIX="msg";
const FORM_SUFFIX="form";
const VAL_SUFFIX="";
const ITEM_COUNT_SUFFIX="itemCount";

const DESCR_SUFFIX="descr";
const MIN_VAL_SUFFIX="minVal";
const MAX_VAL_SUFFIX="maxVal";
const FIELD_ID_SUFFIX="fieldId";
const QUEUE_SUFFIX="queue";

const CLASS_AUTO_CREATED_CHILD="autoCreatedChild";
const LABEL_PREFFIX="Label";

/*--------------------------Menu functions-----------------------------*/
function w3_open(){
	document.getElementById("leftSidebar").style.display = "block";
};
					
function w3_close(){
	document.getElementById("leftSidebar").style.display = "none";
};
/*---------------------------------------------------------------------*/
function hideComponent(componentId){
	var comp=document.getElementById(componentId);
	if(comp!=undefined){
		comp.style.display = "none";
	}
};

function showComponent(componentId){
	var comp=document.getElementById(componentId);
	if(comp!=undefined){
		comp.style.display = "block";
	}
};

function markComponentAs_Valid(comp){
	if(comp!=undefined){
		if(comp.classList!=undefined){
			comp.classList.remove('w3-border-red');
		}
	}
}
	
function markComponentAs_InValid(comp){
	if(comp!=undefined){
		if(comp.classList!=undefined){
			comp.classList.add('w3-border-red');
		}
	}
}

function showMessage(msgComp,message,className){
	if(msgComp!=undefined){
		if(className!=undefined && className!=''){
			msgComp.setAttribute('class',className);
		}
		
		if(message!=undefined && message!=''){
			msgComp.style.display = "block";
			msgComp.innerHTML = message;
		}else{
			msgComp.style.display = "none"; 
		}
	}
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

function arrayToCheckBoxList(component,namePreffix,valArray,nameArray,clazz,style){
	if(component!=undefined){
	
		component.innerHTML = '';
		var cbx=null;
		var lbl=null;
		
		var itemCount=valArray.length;
		
		if(nameArray.length>itemCount){
			itemCount=nameArray.length;
		}
		
		for(i = 0; i<itemCount; i++){ 
			
			var cbxname='checkbox_'+namePreffix+'_'+i;
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

function checkBoxListToArray(component){
	var result = [];	
	
	var inputs=getComponentChildrenByTag(component,'input');
	var j=0;
	
	 for (var i = 0; i < inputs.length; ++i) {
		 var val=getComponentValue(inputs[i]);
		 
		 if(val=='1' || val=='0'){
			 result[j]=val;
			 j++;
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

/*--------------------jsonHandlers----------------*/
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

function processSettingsJson(data){
	
	processSimpleJson(data);
	
	var devId=data.DeviceId;
	var devLocation=data.DeviceLocation;
	
	if(devId!=undefined && devId!='' && devLocation!=undefined && devLocation!=''){
		pageHeader='Устройство !!!SmartHouse - '+devId+' '+devLocation;
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
function updateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut){
	
	var request = new XMLHttpRequest();
	
	var formData = new FormData();
	formData.append('val', val);
	if(sensor!=undefined){
		formData.append('sensor', sensor);
	}
	
	request.open(requestmethod, url, true);
	request.onreadystatechange  = 
		function() {
			if(this.readyState == 4){
								
				if (this.status == 200){
					
					var json = JSON.parse(this.responseText);
							
					handler(json);
									
					if(timeout>0){
						var reloadTime=timeout;
						addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut,reloadTime);
					}
					
				}else{
					if(errorTimeOut>0){
						var reloadTime=errorTimeOut;
						addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val,sensor, timeout, errorTimeOut, reloadTime);
					}
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
function postForm(form,url,validateHandler,resultProcessHandler){
	postFormWithPreprocess(form,url,validateHandler,resultProcessHandler,processFormDataAsFormItems);
}

function postFormAsJsonVal(form,url,validateHandler,resultProcessHandler){
	postFormWithPreprocess(form,url,validateHandler,resultProcessHandler,processFormDataAsJsonVal);
}

function postFormWithPreprocess(form,url,validateHandler,resultProcessHandler,formProcessFunction){
	console.log('submitting form');
	
	var isValidForm = form.checkValidity();
	
	var errorMessage='';
	
	if(isValidForm){
		if(validateHandler!=undefined){
			errorMessage=validateHandler();
		}
		
		if(!(0===errorMessage.length)){
			isValidForm=false;
		}
	}
	
	if(isValidForm){
		
		var formData = new FormData(form);
		
		var childNodes = getComponentChildrenByClass(form,'w3-check');
		
		formData=formProcessFunction(form,childNodes);
		
		showMessage(currentMessageComp,'Сохраняю данные...','w3-yellow');
		
		var request = new XMLHttpRequest();
		request.open("POST", url, true);
		request.onreadystatechange  = 
			function(){
				if(this.readyState == 4){
					if (this.status == 200){
						
						var json = JSON.parse(this.responseText);
						resultProcessHandler(json);
						
						showMessage(currentMessageComp,'Данные сохранены!','w3-green');
						
					} else {
						showMessage(currentMessageComp,'Ошибка на стророне сервера!','w3-red');
					};
				};
			};
		request.send(formData);
	}else{
		errorMessage='Некоторые значения неверны:  '+errorMessage;
		showMessage(currentMessageComp,errorMessage,'w3-red');
	}
}

function processFormDataAsFormItems(form,childNodes){
	
	var formData = new FormData(form);
	
	for(var i=0;i<childNodes.length;i++){
		var child=childNodes[i];
		if(child!=undefined && child.tagName!=undefined){
			var tagName = child.tagName.toLowerCase();
			
			if (tagName == 'input'){
				if(child.type!=undefined){
					var type=child.type.toLowerCase();
					
					if(type=='checkbox'){
						var chbVal=child.checked;
						
						if(chbVal!=true){
							formData.append(child.id, false);
						}
					}
				};
			}
		}
	}
	
	return formData;
}

function processFormDataAsJsonVal(form,childNodes){
	
	var formData = new FormData(form);
	
	/*const data = formToJSON(form.elements);
	formData.append("val", data);
	*/
	return formData;
}
/*
const formToJSON = elements => [].reduce.call(elements, (data, element) => {
	  
	  data[element.name] = element.value;
	  return data;

	}, {});
	*/



