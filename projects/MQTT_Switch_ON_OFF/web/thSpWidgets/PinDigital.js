<script type='text/javascript'>

  // set your channel params here
  var channel_id = 623698;
  var write_key = 'N9EQ8RTYQ7ZXYR8T';
  var read_key = 'XQ4QSQQKEKMRJ4DK';
  var user_key = 'YV1SA7H87PS8RFMU';
  
  var fieldId=1;
  //-----------------------------------------------------
  //------------All other vals are auto generated--------
  var setValUrl='https://api.thingspeak.com/update.json?api_key='+write_key+'&field'+fieldId+'=';
  var getValUrl='https://api.thingspeak.com/channels/'+channel_id+'/fields/'+fieldId+'/last.json?api_key='+read_key+'&status=true';
  var getChDetUrl='https://api.thingspeak.com/channels/'+channel_id+'/fields/'+fieldId+'.json?api_key='+read_key+'&results=0';
  
  //https://api.thingspeak.com/update.json?api_key=N9EQ8RTYQ7ZXYR8T&field2=0
  //https://api.thingspeak.com/channels/623698/fields/2/last.json?api_key=XQ4QSQQKEKMRJ4DK&status=true
  //https://api.thingspeak.com/channels/623698/fields/1.json?api_key=XQ4QSQQKEKMRJ4DK&results=0
  
  var currentVal=0;
  var currentFieldName="Loading...";
  
  
  init();
  
  function init(){
     getCurrentVal();
     getChannelDetails();
     setTimeout(function(){init()}, 60000); 
  }
  
  function getChannelDetails(){
     var data=httpGet(getChDetUrl);
     var field='field'+fieldId;
     
     var json = JSON.parse(data);
     var channel = json.channel;
     var val= channel[field];
    
     currentFieldName=val; 
    
  }
  
  function getCurrentVal(){
    var data=httpGet(getValUrl);
       
    currentVal=parseInt(parseJsonResponse(data));
       
    displayCurrentVal();
  }
  
  function sendCurrentVal() {
    var val='0';
    
    if(currentVal=='0'){
      val='1';
    }
    
    var url=setValUrl+val;
    var result=httpGet(url);
    
    console.log('val='+val);
    console.log('url='+url);
    console.log('result='+result);
    
    if(result!='0'){
        currentVal=parseJsonResponse(result);
        displayCurrentVal();
    }else{
        setTimeout(function(){sendCurrentVal()}, 10000);
    }
  }
  
  function displayCurrentVal(){
    var comp=document.getElementById('currentValImg');
        
    console.log('comp='+comp);
    
    if(comp!=undefined && comp!=null){
      var imageName='imgQ';
      
      comp.value=currentVal;
      if(currentVal=='1' || currentVal=='0'){
          imageName='img'+currentVal;
      }
      
      var imgs=document.getElementsByClassName('digitalPinImage');
      var imgsLength=imgs.length;
      
      for(var i=0;i<imgsLength;i++){
        if(imgs[i]!=undefined && imgs[i]!=null && i!='length'){
          if(imgs[i].id==imageName){
             imgs[i].setAttribute('style','display: block'); 
          }else{
             imgs[i].setAttribute('style','display: none');
          }
        }
      }
      
      var lbl=document.getElementById('currentFieldNameLbl');
      lbl.value=currentFieldName;
      lbl.innerHTML=currentFieldName;
      
    }else{
      setTimeout(function(){displayCurrentVal()}, 1000);
    }
  }
  
  function parseJsonResponse(data){
     var field='field'+fieldId;
     var json = JSON.parse(data);
    
     var val=json[field]; 
    
    console.log('field='+field);
    console.log('json='+JSON.stringify(json));
    console.log('val='+val);
    
    return val;
  }
     
  function httpGet(theUrl){
    console.log('url='+theUrl);
    
    var xmlHttp = new XMLHttpRequest();
    xmlHttp.open( "GET", theUrl, false ); // false for synchronous request
    xmlHttp.send( null );
    return xmlHttp.responseText;
  }  
 
</script>

