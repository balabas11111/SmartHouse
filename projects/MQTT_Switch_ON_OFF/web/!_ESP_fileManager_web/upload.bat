SET ip=192.168.0.100

curl -X POST -F "data=@index.htm"     http://%ip%/edit
curl -X POST -F "data=@graphs.js.gz"  http://%ip%/edit
curl -X POST -F "data=@favicon.ico"   http://%ip%/edit
curl -X POST -F "data=@edit.htm.gz"   http://%ip%/edit

pause