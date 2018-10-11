################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266\spiffs\spiffs_cache.c \
D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266\spiffs\spiffs_check.c \
D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266\spiffs\spiffs_gc.c \
D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266\spiffs\spiffs_hydrogen.c \
D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266\spiffs\spiffs_nucleus.c 

C_DEPS += \
.\core\core\spiffs\spiffs_cache.c.d \
.\core\core\spiffs\spiffs_check.c.d \
.\core\core\spiffs\spiffs_gc.c.d \
.\core\core\spiffs\spiffs_hydrogen.c.d \
.\core\core\spiffs\spiffs_nucleus.c.d 

AR_OBJ += \
.\core\core\spiffs\spiffs_cache.c.o \
.\core\core\spiffs\spiffs_check.c.o \
.\core\core\spiffs\spiffs_gc.c.o \
.\core\core\spiffs\spiffs_hydrogen.c.o \
.\core\core\spiffs\spiffs_nucleus.c.o 


# Each subdirectory must supply rules for building sources it contributes
core\core\spiffs\spiffs_cache.c.o: D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266\spiffs\spiffs_cache.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-gcc" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:/projects/!!!SmartHouse/workspace/SHT21_sensor/Release/core" -c -Wall -Wextra -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -std=gnu99 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\Wire" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\core\spiffs\spiffs_check.c.o: D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266\spiffs\spiffs_check.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-gcc" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:/projects/!!!SmartHouse/workspace/SHT21_sensor/Release/core" -c -Wall -Wextra -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -std=gnu99 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\Wire" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\core\spiffs\spiffs_gc.c.o: D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266\spiffs\spiffs_gc.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-gcc" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:/projects/!!!SmartHouse/workspace/SHT21_sensor/Release/core" -c -Wall -Wextra -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -std=gnu99 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\Wire" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\core\spiffs\spiffs_hydrogen.c.o: D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266\spiffs\spiffs_hydrogen.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-gcc" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:/projects/!!!SmartHouse/workspace/SHT21_sensor/Release/core" -c -Wall -Wextra -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -std=gnu99 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\Wire" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

core\core\spiffs\spiffs_nucleus.c.o: D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266\spiffs\spiffs_nucleus.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2/bin/xtensa-lx106-elf-gcc" -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/lwip/include" "-ID:\projects\sloeber\/arduinoPlugin/packages/esp8266/hardware/esp8266/2.4.0-rc2/tools/sdk/libc/xtensa-lx106-elf/include" "-ID:/projects/!!!SmartHouse/workspace/SHT21_sensor/Release/core" -c -Wall -Wextra -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -std=gnu99 -ffunction-sections -fdata-sections -DF_CPU=80000000L -DLWIP_OPEN_SRC  -DARDUINO=10802 -DARDUINO_ESP8266_NODEMCU -DARDUINO_ARCH_ESP8266 "-DARDUINO_BOARD=\"ESP8266_NODEMCU\"" -DESP8266  -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\cores\esp8266" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\variants\nodemcu" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0" -I"D:\projects\sloeber\arduinoPlugin\libraries\ArduinoJson\5.12.0\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\ESP8266WiFi\src" -I"D:\projects\sloeber\arduinoPlugin\packages\esp8266\hardware\esp8266\2.4.0-rc2\libraries\Wire" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


