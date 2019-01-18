function createDivComponent(id,clazz,style,innerHtml){
	return createHtmlComponent('div',id,'',clazz,style,innerHtml);
}

function createHeaderComponent(clazz){
	return createHtmlComponent('header',undefined,undefined,clazz,undefined,undefined);
}

function createSpanComponent(id,clazz,onclick,innerHtml){
	var comp= createHtmlComponent('span',id,undefined,clazz,undefined,innerHtml);
	comp.setAttribute('onclick',onclick);
	return comp;
}

function createButtonComponent(tag,id,clazz,style,innerHtml,onclick){
	var btn=createHtmlComponent('button',id,undefined,clazz,style,innerHtml);
	btn.setAttribute('onclick',onclick);
	return btn;
}

function createHiddenInput(id,name,value){
	var inputsDiv=getConfirmInputsDiv();
	var input=createInputSimple(id,name,'','',value,true);
	input.setAttribute('type','hidden')
	return input;
}

function createLabelSimple(id,forVal,clazz,style,value){
	var lbl=document.createElement("label");
	
	lbl.setAttribute("id",id);
	lbl.setAttribute("for",forVal);
	if(clazz!=undefined){ lbl.setAttribute("class",clazz);}
	if(style!=undefined){ lbl.setAttribute("style",style);}
	lbl.innerHTML=value;
	
	return lbl; 
}

function createInputSimple(id,name,clazz,style,value,editable){
	var input1=document.createElement("Input");
	
	input1.setAttribute("id",id);
	input1.setAttribute("name",name);
	if(clazz==undefined){ clazz="w3-input w3-border";}
	input1.setAttribute("class",clazz);
	if(style!=undefined){ input1.setAttribute("style",style);}
	input1.setAttribute("value",value);
	
	if(editable==undefined || !editable)
		input1.setAttribute("disabled","disabled");
	
return input1; 
}

function createHtmlComponent(tag,id,name,clazz,style,innerHtml){
	var comp=document.createElement(tag);
	
	if(id!=undefined && id!=''){comp.setAttribute('id',id);}
	if(name!=undefined && name!=''){	comp.setAttribute('name',name); }
	if(clazz!=undefined && clazz!=''){	comp.setAttribute('class',clazz); }
	if(style!=undefined && style!=''){	comp.setAttribute('style',style); }
	if(innerHtml!=undefined && innerHtml!=''){	comp.innerHTML=innerHtml; }
		
	return comp;
}
/*----------------------------------------------------------------------------*/
function hideComponent(componentId){
	var comp=document.getElementById(componentId);
	setVisible(comp,false);
};

function showComponent(componentId){
	var comp=document.getElementById(componentId);
	setVisible(comp,true);
};

function isVisible(comp){
	return (comp!=undefined && comp.style!=undefined && comp.style.display=='block')
}

function setVisible(comp,visible){
	if(comp!=undefined){
		if(visible){
			comp.style.display = "block";
		}else{
			comp.style.display = "none";
		}
	}
}