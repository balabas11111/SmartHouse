SET IP=192.168.0.107
SET FOLDER=css
SET PREFFIX=web
SET DELIM=/


curl -i -X POST -H "Expect:" -F "uploadfile=@index.htm.gz;filename=web/index.htm.gz;" -F submit=upload http://%ip%/edit


pause
