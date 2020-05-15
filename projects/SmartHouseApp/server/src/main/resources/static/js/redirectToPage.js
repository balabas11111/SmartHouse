function timedRefresh(timeoutPeriod) {
   var timer = setInterval(function() {
   if (timeoutPeriod > 0) {
       timeoutPeriod -= 1;
       document.getElementById("countdown").innerHTML = "<h3>" + timeoutPeriod + " сек" + "</h3>"+ "<br />";
   } else {
       clearInterval(timer);
            window.location.href = "http://192.168.0.101/devices?v=false";
       };
   }, 1000);
};
timedRefresh(60);