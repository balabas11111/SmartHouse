/*-----------------------Sensors values processing--------------------------------*/
var innactiveIndex=-1;
var multidailyIndex=-1;
var periodicIndex=-1;
var intervalType=[];
var intervalState=[];
var dayOfWeekShort=[];
var dayOfWeek=[];

function processIntervalsJsonGet(data){
	fillParameters(data);
	processItemsJsonGet(data);
}

function fillParameters(data){
	if(intervalType.length==0){
		var arr=data.intervalType;
		for(var i=0;i<arr.length;i++){intervalType[i]=arr[i];}
	}
	if(intervalState.length==0){
		var arr=data.intervalState;
		for(var i=0;i<arr.length;i++){intervalState[i]=arr[i];}
	}
	if(dayOfWeekShort.length==0){
		var arr=data.dayOfWeekShort;
		for(var i=0;i<arr.length;i++){dayOfWeekShort[i]=arr[i];}
	}
	if(dayOfWeek.length==0){
		var arr=data.dayOfWeek;
		for(var i=0;i<arr.length;i++){dayOfWeek[i]=arr[i];}
	}
	multidailyIndex=data.multidailyIndex;
	periodicIndex=data.periodicIndex;
	innactiveIndex=data.innactiveIndex;
}

function putIntervalHeaderToContainer(container,items){
	
}

function putIntervalContentToContainer(container,interval,noId,editable){
	var id=interval.id;
	var name=interval.name;
	var typeInt=interval.typeInt;
	var stateInt=interval.stateInt;
	var startTime=interval.startTime;
	var endTime=interval.endTime;
	var time=interval.time;
	var days=interval.days.split(',');
	var param=interval.param;
	
	var hr = document.createElement('hr');
	container.appendChild(hr);
	container.appendChild(hr);
	var divh=document.createElement('div');
	divh.setAttribute('class','w3-row w3-teal');
	divh.setAttribute('style','margin-top: 15px; height: 50px;');
	
	var div1h=createDivComponent('w3-quarter','Интервал');
	var div2h=createDivComponent('w3-quarter','Тип/Состояние.');
	var div3h=createDivComponent('w3-quarter','Старт/Стоп');
	var div4h=createDivComponent('w3-quarter','Параметры');
	
	divh.appendChild(div1h);
	divh.appendChild(div2h);
	divh.appendChild(div3h);
	divh.appendChild(div4h);
	
	container.appendChild(divh);
	var divhv=document.createElement('div');
	divhv.setAttribute('class','w3-row');
	divhv.setAttribute('style','margin-top: 1px; height: 130px;');
	
	var col1h=createDivComponent('w3-quarter','');
	var col2h=createDivComponent('w3-quarter','');
	var col3h=createDivComponent('w3-quarter','');
	var col4h=createDivComponent('w3-quarter','');
	/* column 1*/
	
	var suf='';
	
	if(editable==undefined || !editable){
		suf='_'+id;
	}
	
	var cid='id'+suf;
	var cname='name'+suf;
	var ctypeInt='typeInt'+suf;
	var cstateInt='stateInt'+suf;
	var cstartTime='startTime'+suf;
	var cendTime='endTime'+suf;
	var ctime='time'+suf;
	var cdays='days'+suf;
	var cparam='param'+suf;
	
	if(editable==undefined || !editable){
		var cellEditButton=createDivComponent('w3-cell','');
		var cellDeleteButton=createDivComponent('w3-cell','');
		var editButton=createItemEditButton('id',id,name,undefined);
		var deleteButton=createItemDeleteButton('id',id,name);
		
		cellEditButton.appendChild(editButton);
		cellDeleteButton.appendChild(deleteButton);
		
		col1h.appendChild(cellEditButton);
		col1h.appendChild(cellDeleteButton);
	}
	
	var inputId=createInputSimple(cid,'id',undefined,'',id,false);
	var cellDivId=createDivComponent('w3-cell w3-container','');
	cellDivId.appendChild(inputId);
	setVisible(cellDivId,false);

	var lblName=createLabelSimple('lbl'+cname,cname,undefined,undefined,'Название');
	var inputName=createInputSimple(cname,'name',undefined,'width: 80%;',name,editable);
	var cellDivName=createDivComponent('','');
	cellDivName.appendChild(lblName);
	cellDivName.appendChild(inputName);
		
	col1h.appendChild(cellDivId);
	col1h.appendChild(cellDivName);
	/* column 2*/
	
	var lblType=createLabelSimple('lbl'+ctypeInt,ctypeInt,undefined,undefined,'Тип интервала');
	var typeSelect=document.createElement("select");
	var cellDivType=createDivComponent('','');
	
	typeSelect.id=ctypeInt;
	typeSelect.name='typeInt';
	typeSelect.setAttribute('onChange',"handlePeriodTypeChange(this.selectedIndex);");
	typeSelect.setAttribute('class','w3-select');
	typeSelect.setAttribute('style','width: 80%;');
	
	if(editable==undefined || !editable){typeSelect.setAttribute("disabled","disabled");}
	putItemsToComboCox(typeSelect,intervalType,typeInt);
	
	cellDivType.appendChild(lblType);
	cellDivType.appendChild(typeSelect);
	/*--*/
	var lblState=createLabelSimple('lbl'+cstateInt,cstateInt,undefined,undefined,'Состояние');
	var stateSelect=document.createElement("select");
	var cellDivState=createDivComponent('','');
	
	stateSelect.id=cstateInt;
	stateSelect.name='stateInt';
	stateSelect.setAttribute('class','w3-select');
	stateSelect.setAttribute('style','width: 80%;');
	
	if(editable==undefined || !editable){stateSelect.setAttribute("disabled","disabled");}
	putItemsToComboCox(stateSelect,intervalState,stateInt);
	
	cellDivState.appendChild(lblState);
	cellDivState.appendChild(stateSelect);
	
	col2h.appendChild(cellDivType);
	col2h.appendChild(cellDivState);
	
	/* column 3*/
	var lblStartTime=createLabelSimple('lbl'+cstartTime,cstartTime,undefined,undefined,'Старт');
	var inputStartTime=createInputSimple(cstartTime,'startTime',undefined,'width: 90%;','',editable);
	var cellDivStartTime=createDivComponent('','');
	
	inputStartTime.setAttribute('type','datetime-local');
	setComponentValue(inputStartTime,startTime);
	
	cellDivStartTime.appendChild(lblStartTime);
	cellDivStartTime.appendChild(inputStartTime);
	/*--*/
	var lblEndTime=createLabelSimple('lbl'+cendTime,cendTime,undefined,undefined,'Стоп ');
	var inputEndTime=createInputSimple(cendTime,'endTime',undefined,'width: 90%;','',editable);
	var cellDivendTime=createDivComponent('','');
	
	inputEndTime.setAttribute('type','datetime-local');
	setComponentValue(inputEndTime,endTime);
	
	cellDivendTime.appendChild(lblEndTime);
	cellDivendTime.appendChild(inputEndTime);
	
	col3h.appendChild(cellDivStartTime);
	col3h.appendChild(cellDivendTime);
	
	/* column 4*/
		var lblDays=createLabelSimple('lbl'+cdays,undefined,undefined,undefined,'Дни выполнения');
		var daysDiv=createDivComponent('','');
		daysDiv.id=cdays+'Div';
		daysDiv.name='days';
		
		daysDiv.appendChild(lblDays);
		arrayToCheckBoxList(daysDiv,cdays,days,dayOfWeekShort);
		/*--*/
		var lblTime=createLabelSimple('lbl'+ctime,ctime,undefined,undefined,'Период (сек) ');
		var inputTime=createInputSimple(ctime,'time',undefined,'width: 50%;',time,editable);
		var timeDiv=createDivComponent('','');
		timeDiv.id=ctime+'Div';
		
		inputTime.setAttribute('type','number');
		inputTime.setAttribute('min','60');
		inputTime.setAttribute('step','5');
		
		timeDiv.appendChild(lblTime);
		timeDiv.appendChild(inputTime);
		/*--*/
		var lblParam=createLabelSimple('lbl'+cparam,cparam,undefined,undefined,'Параметр');
		var inputParam=createInputSimple(cparam,'param',undefined,'width: 60%;',param,editable);
		var paramDiv=createDivComponent('','');
		
		paramDiv.id=cparam+'Div';
		
		paramDiv.appendChild(lblParam);
		paramDiv.appendChild(inputParam);
		/*--*/
		applyIntervalTypeValueToComp(daysDiv,timeDiv,paramDiv,typeInt);
		
	col4h.appendChild(daysDiv);
	col4h.appendChild(timeDiv);
	col4h.appendChild(paramDiv);
					
	divhv.appendChild(col1h);
	divhv.appendChild(col2h);
	divhv.appendChild(col3h);
	divhv.appendChild(col4h);
	
	container.appendChild(divhv);
	
	var divb=document.createElement('div');
	divb.setAttribute('class','w3-row w3-black');
	divb.setAttribute('style','margin-top: 1px; margin-bottom: 5px; height: 5px;');
			
	container.appendChild(divb);
	container.appendChild(hr);
}

function handlePeriodTypeChange(selectedIndex){
	var daysDiv=getComponentById('daysDiv');
	var timeDiv=getComponentById('timeDiv');
	var paramDiv=getComponentById('paramDiv');
	
	applyIntervalTypeValueToComp(daysDiv,timeDiv,paramDiv,selectedIndex)
}

/*show or hide days interval component based on type*/
function applyIntervalTypeValueToComp(daysDiv,timeDiv,paramDiv,typeInt){
	var isMultidaily=(typeInt==multidailyIndex);
	var isPeriodic=(typeInt==periodicIndex);
	var isParam=true;
	
	setVisible(daysDiv,isMultidaily);
	setVisible(timeDiv,isPeriodic);
	setVisible(paramDiv,isParam);
}

function validateCurrentIntervalForm(){
	var errorMessage='';
		
	
	return errorMessage;
}

