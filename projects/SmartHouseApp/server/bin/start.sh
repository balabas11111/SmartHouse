process=`ps -ef | grep -v awk | awk -e '/sudo.*java.*server-2.2.0.M6.jar/ { print $2 }'`

if [ -z "$process" ]
then
      echo "\smartHouseServer has not started as sudo"
      nohup sudo java -jar /home/pi/apps/SmartHouseApp/server-2.2.0.M6.jar
else
      echo "\pid = $process smartHouseServer already runs"
fi