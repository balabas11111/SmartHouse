SET ip=192.168.0.107

curl -i -X DELETE -H "Expect:" -F "filename=/web/css/w3.css" http://%ip%/edit

pause
