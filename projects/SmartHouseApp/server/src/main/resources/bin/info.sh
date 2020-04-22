process=`ps -ef | grep -v awk | awk -e '/sudo.*java.*server-2.2.0.M6.jar/ { print $2 }'`


if [ -z "$process" ]
then
      echo "\smartHouseServer has not started as sudo"
else
      echo "\sudo pid = $process"
      ps auwx|egrep "MEM|$process"|grep -v grep  
fi

process2=`ps -ef | grep -v awk | awk -e '/java.*server-2.2.0.M6.jar/ { print $2 }'`

if [ -z "$process2" ]
then
      echo "\smartHouseServer has not started "
else
      echo "\ pid = $process2"
      ps auwx|egrep "MEM|$process2"|grep -v grep
fi