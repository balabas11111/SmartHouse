process=`ps -ef | grep -v awk | awk -e '/sudo.*java.*server-2.2.0.M6.jar/ { print $2 }'`


if [ -z "$process" ]
then
      echo "\smartHouseServer has not started as sudo"
else
      echo "\sudo will be killed pid = $process"
      sudo kill -9 $process      
fi

process2=`ps -ef | grep -v awk | awk -e '/java.*server-2.2.0.M6.jar/ { print $2 }'`

if [ -z "$process2" ]
then
      echo "\smartHouseServer has not started "
else
      echo "\will be killed pid = $process2"
      sudo kill -9 $process2
fi