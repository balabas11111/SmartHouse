SET IP=192.168.0.107
SET FOLDER=css
SET PREFFIX=web
SET DELIM=/


curl -i -X POST -H "Expect:" -F "uploadfile=@css/w3.css;filename=web/css/w3.css;" -F submit=upload http://%ip%/edit


pause

