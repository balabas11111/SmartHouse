SET FILE=%1 
SET FILENAME=%2
SET IP=%3
SET RESULT_FILE=%4

curl -s -i -X POST -H "Expect:" -F "uploadfile=%FILE%;filename=%FILENAME%;" -F submit=upload http://%IP%/files 