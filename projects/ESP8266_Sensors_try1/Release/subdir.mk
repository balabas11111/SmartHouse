################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
..\ESP8266_Sensors.ino 

CPP_SRCS += \
..\sloeber.ino.cpp 

LINK_OBJ += \
.\sloeber.ino.cpp.o 

INO_DEPS += \
.\ESP8266_Sensors.ino.d 

CPP_DEPS += \
.\sloeber.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
ESP8266_Sensors.o: ..\ESP8266_Sensors.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-IC:\Users\Vitaliy\Documents\sloeber-workspace\ESP8266_Sensors/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266mDNS" -I"C:\Users\Vitaliy\Documents\Arduino\libraries\OLED" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\Wire" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\libraries\Rtc_by_Makuna\2.0.2" -I"D:\projects\sloeber\arduinoPlugin\libraries\Rtc_by_Makuna\2.0.2\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\OneWire\2.3.3" -I"C:\Users\Vitaliy\Documents\Arduino\libraries\DallasTemperature" -I"D:\projects\!!!SmartHouse\librarys\SensorsDS18_helper" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

sloeber.ino.cpp.o: ..\sloeber.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-g++" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-IC:\Users\Vitaliy\Documents\sloeber-workspace\ESP8266_Sensors/Release/core" -c -Wall -Wextra -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WebServer\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266mDNS" -I"C:\Users\Vitaliy\Documents\Arduino\libraries\OLED" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\Wire" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\libraries\Rtc_by_Makuna\2.0.2" -I"D:\projects\sloeber\arduinoPlugin\libraries\Rtc_by_Makuna\2.0.2\src" -I"D:\projects\sloeber\arduinoPlugin\libraries\OneWire\2.3.3" -I"C:\Users\Vitaliy\Documents\Arduino\libraries\DallasTemperature" -I"D:\projects\!!!SmartHouse\librarys\SensorsDS18_helper" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


