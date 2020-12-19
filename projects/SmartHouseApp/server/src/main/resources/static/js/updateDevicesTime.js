function timedTimeTickerRefresh() {

   var timer = setInterval(function() {
   	var elements = document.getElementsByClassName("time-ticker");
   	
   	for (let i = 0; i < elements.length; i++) {
  		var element = elements[i];
  		var text = parseInt(element.innerHTML) + 1;
  		
  		element.innerHTML = text;
  		
	}

   }, 1000);
   
};
timedTimeTickerRefresh();
