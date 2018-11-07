//------------------------------DS18D20--------------------------------
function processDS18D20Json(data){
	var items=data.items;
	var parentId=data.name;
	var childCount=data.childCount;
	
	var countCompId="Count_"+parentId;
	var countComponent=document.getElementById(countCompId);
	
	if(countComponent!=undefined){
		countComponent.innerHTML=childCount;
	}
		
	for(var i in items){
		var inputId=items[i].name;
		var labelVal=items[i].descr;
		var inputVal=items[i].val;
		
		addDS18D20Component(parentId,inputId,labelVal,inputVal);
	}
}

function addDS18D20Component(parentId,inputId,labelVal,inputVal){
		
	var newLabelId="Label_"+inputId;
	var newInputId="Input_"+inputId;
	var newLabelHtml="<b>"+labelVal+"</b>"
	//<label style="max-width: 240px;" for="Temperature"><b id="temp1"></b></label>		
	//<input id="Temperature"	class="" ="" type="text" value="��������" disabled>	
	var inputComponent=document.getElementById(newInputId);
	var labelComponent=document.getElementById(newLabelId);
	
	if(inputComponent!=undefined && labelComponent!=undefined){
		inputComponent.value=inputVal;
		labelComponent.innerHTML=newLabelHtml;
		return;
	}
	
	var container=document.getElementById(parentId);
	
	if(container!=undefined){
		
		var newLabel = document.createElement("Label");
		newLabel.setAttribute("id",newLabelId);
		newLabel.setAttribute("for",newInputId);
		newLabel.setAttribute("style","max-width: 240px;");
		newLabel.innerHTML = newLabelHtml ;
		
		var newInput=document.createElement("Input");
		newInput.setAttribute("id",newInputId);
		newInput.setAttribute("class","w3-input w3-border");
		newInput.setAttribute("style","width:30%");
		newInput.setAttribute("disabled","disabled");
		newInput.setAttribute("value",inputVal);
		
		container.appendChild(newLabel);
		container.appendChild(newInput);
	}
	
}