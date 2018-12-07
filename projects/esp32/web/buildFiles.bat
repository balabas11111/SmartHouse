SET IP=192.168.0.107
SET FOLDER=css
SET PREFFIX=web
SET DELIM=/

@ECHO on

mkdir temp

copy ..\..\MQTT_Switch_ON_OFF\web\css\w3.css temp
copy ..\..\MQTT_Switch_ON_OFF\web\js\DeviceLib.js temp

powershell -Command "(gc index.htm) -replace '<link rel=\"stylesheet\" href=\"./css/w3.css\">', '12345' | Out-File temp\index.htm"


pause

