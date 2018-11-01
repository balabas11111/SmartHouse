//----------------------Web sockets--------------------------
var socket=undefined;

function ping() {
		if(countDown>0){
			return;
		}
        socket.send('__ping__');
        tm = setTimeout(function () {
           /// ---connection closed ///
		onConnectionClosed();
    }, 5000);
}

function pong() {
    clearTimeout(tm);
}

var countDown=0;
var textPreffix="";

function onConnectionClosed(){
	if(countDown>0){
		return;
	}
	onConnectionChanged(false);
	printMainStatus("Соединение потеряно...",STATUS_ERROR_CLASS); 
	socket.close();
	
	var widgetId="websocket_status";
	var component=document.getElementById(widgetId);
	
	textPreffix=component.innerHTML;
	countDown=60;
	tm2=setTimeout(tickCountDown, 1000);
}

function tickCountDown(){
		
	if(document.getElementById("connectingImage").style.display == "none"){
		document.getElementById("connectingImage").style.display = "block";
	}else{
		document.getElementById("connectingImage").style.display = "none";	
	}
	
	var txt=textPreffix+" перезагрузка страницы через "+countDown+" сек"
	
	printMainStatus(txt,STATUS_ERROR_CLASS);
	
	if(countDown<=0){
		location.reload(true);
	}else{
		tm2=setTimeout(tickCountDown, 1000);
	}
	
	countDown=countDown-1;
}


 function WebSocketInit() {
            
            if ("WebSocket" in window) {
               printMainStatus("Соединение WebSocket поддерживается", STATUS_OK_CLASS);
               
               // Let us open a web socket
               socket=new WebSocket("ws://"+window.location.hostname+":8081");
			   
			   var status=socket.readyState;
			   
			   if(status==3){
               		printMainStatus("В соединении отказано",STATUS_ERROR_CLASS);
               }
				
               socket.onopen = function() {
                  // Web Socket is connected, send data using send()
				  setInterval(ping, 120000);
                  socket.send("Message to send");
                  printMainStatus("Подключено", STATUS_OK_CLASS);
				  onConnectionChanged(true);
               };
				
               socket.onmessage = function(event){
				    var incomingMessage=event.data;
				   					
					if (incomingMessage == '__pong__') {
						pong();
						return;
					}
				   					
					receiveWsMessage(incomingMessage);
               };
				
               socket.onclose = function() { 
                  // websocket is closed.
                  printMainStatus("Соединение закрыто...", STATUS_ERROR_CLASS); 
				  onConnectionClosed();
               };
			   
			   socket.onerror = function(){
				   printMainStatus("Ошибка сокета...", STATUS_ERROR_CLASS); 
				   onConnectionClosed();
			   };
            } else {
               onConnectionChanged(false);
               // The browser doesn't support WebSocket
               printMainStatus("Веб сокет не поддерживается вашим Браузером", STATUS_ERROR_CLASS);
            }
}

function onConnectionChanged(connected){
	
	if(connected){
		document.getElementById("connectingImage").style.display = "none";
		document.getElementById("connectedImage").style.display = "block";
	}else{
		document.getElementById("connectingImage").style.display = "block";
		document.getElementById("connectedImage").style.display = "none";
	}
}

function receiveWsMessage(message){
	if(message.startsWith('{"wsId":')){
		processComponentUpdate(message);
	}
}

function sendWsMessage(message){
	socket.send(message);
	printMainStatus("Отправлено "+message, STATUS_OK_CLASS);
}

function processComponentUpdate(message){
// {"wsId":"luxMeasurer","wsClass":"wsParent","items":"{"light":"5"}"}
// {"wsId":"bmeMeasurer","wsClass":"wsParent","items":"{"Temperature":"23.57","Pressure":"99943.66","Humidity":"50.04","Altitude":"115.65"}"}
// {"wsId":"lampLeft","wsClass":"wsItem","wsValue":"./img/OffLamp.png"}
	
	var json = JSON.parse(message);

	var wsId=json.wsId;
	var wsClass=json.wsClass;
	
	if(wsClass==WS_TAG_PARENT){
		var items=json.items;
		var components=getComponentsByWsId(wsId,wsClass);
		
		if(components!=undefined){
			for (i = 0; i < components.length; i++){
				var component=components[i];
				if(component!=undefined){
					var children = component.getElementsByClassName(WS_TAG_CHILD);
					
					for (index = 0; index < children.length; ++index) {
						childComponent=children[index];
						
						var wsId=childComponent.getAttribute(WS_TAG_ID);
						var wsValueTagValue=childComponent.getAttribute(WS_TAG_VALUE);
																
						if(items.hasOwnProperty(wsId)){
							receivedValue=items[wsId];
							
							childComponent.setAttribute(wsValueTagValue,receivedValue);
						}
					}
				}
			}
		}
	}
	
	if(wsClass==WS_TAG_ITEM){
		var components=getComponentsByWsId(wsId,wsClass);
		
		if(components!=undefined){
			for (i = 0; i < components.length; i++){
				var component=components[i];
				var wsValue=component.getAttribute(WS_TAG_VALUE);
				var value=json.wsValue;
				
				if(wsValue!=undefined){
					component.setAttribute(wsValue,value);
				}else{
					component.innerHTML=value;
				}
			}
		}
	}
}

function getComponentsByWsId(wsId,wsClass){
	widgets=document.getElementsByClassName(wsClass);
	
	var result=[];
	var index=0;
	
	for (i = 0; i < widgets.length; i++) {
		var component=widgets[i];
		
		if(component.hasAttribute(WS_TAG_ID) 
			&& component.getAttribute(WS_TAG_ID)==wsId){
				result.push(component);
				index++;
			}
	}
	
	return result;
}

function printMainStatus(message,compClass){
	var widgetId="websocket_status";
	var component=document.getElementById(widgetId);
		
	updateStatusComponentInnerHtml(component,"block",compClass,message);
}