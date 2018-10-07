################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\BeeperB.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\ButtonB.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\LedB.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\RequestParam.cpp \
D:\projects\!!!SmartHouse\librarys\TimeTrigger\TimeTrigger.cpp 

LINK_OBJ += \
.\libraries\TimeTrigger\BeeperB.cpp.o \
.\libraries\TimeTrigger\ButtonB.cpp.o \
.\libraries\TimeTrigger\LedB.cpp.o \
.\libraries\TimeTrigger\RequestParam.cpp.o \
.\libraries\TimeTrigger\TimeTrigger.cpp.o 

CPP_DEPS += \
.\libraries\TimeTrigger\BeeperB.cpp.d \
.\libraries\TimeTrigger\ButtonB.cpp.d \
.\libraries\TimeTrigger\LedB.cpp.d \
.\libraries\TimeTrigger\RequestParam.cpp.d \
.\libraries\TimeTrigger\TimeTrigger.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries\TimeTrigger\BeeperB.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\BeeperB.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\cores\arduino" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\variants\eightanaloginputs" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\ButtonB.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\ButtonB.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\cores\arduino" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\variants\eightanaloginputs" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\LedB.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\LedB.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\cores\arduino" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\variants\eightanaloginputs" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\RequestParam.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\RequestParam.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\cores\arduino" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\variants\eightanaloginputs" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries\TimeTrigger\TimeTrigger.cpp.o: D:\projects\!!!SmartHouse\librarys\TimeTrigger\TimeTrigger.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\cores\arduino" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\variants\eightanaloginputs" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


