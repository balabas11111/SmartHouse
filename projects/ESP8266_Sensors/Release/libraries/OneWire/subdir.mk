################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
D:\projects\sloeber\arduinoPlugin\libraries\OneWire\2.3.3\OneWire.cpp 

LINK_OBJ += \
.\libraries\OneWire\OneWire.cpp.o 

CPP_DEPS += \
.\libraries\OneWire\OneWire.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\OneWire\OneWire.cpp.o: D:\projects\sloeber\arduinoPlugin\libraries\OneWire\2.3.3\OneWire.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:\projects\!!!SmartHouse\projects\ESP8266_Sensors/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\libraries\DallasTemperature\3.7.6" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\OneWire\2.3.3" -I"D:\projects\sloeber\arduinoPlugin\libraries\Rtc_by_Makuna\2.0.2" -I"D:\projects\sloeber\arduinoPlugin\libraries\Rtc_by_Makuna\2.0.2\src" -I"D:\projects\!!!SmartHouse\librarys\SensorsDS18_helper" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\Wire" -I"D:\projects\!!!SmartHouse\librarys\OLED" -I"D:\projects\sloeber\arduinoPlugin\libraries\DHT_sensor_library\1.2.2" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\EEPROM" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266HTTPClient\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\PubSubClient\2.6.0\src" -I"D:\projects\!!!SmartHouse\librarys\BeeperB" -I"D:\projects\!!!SmartHouse\librarys\ButtonB" -I"D:\projects\!!!SmartHouse\librarys\EspSettingsBox" -I"D:\projects\!!!SmartHouse\librarys\LedB" -I"D:\projects\!!!SmartHouse\librarys\Loopable" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


