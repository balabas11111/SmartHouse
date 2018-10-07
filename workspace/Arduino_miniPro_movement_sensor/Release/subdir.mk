################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
..\Arduino_miniPro_movement_sensor.ino 

CPP_SRCS += \
..\sloeber.ino.cpp 

LINK_OBJ += \
.\sloeber.ino.cpp.o 

INO_DEPS += \
.\Arduino_miniPro_movement_sensor.ino.d 

CPP_DEPS += \
.\sloeber.ino.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
Arduino_miniPro_movement_sensor.o: ..\Arduino_miniPro_movement_sensor.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\cores\arduino" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\variants\eightanaloginputs" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

sloeber.ino.cpp.o: ..\sloeber.ino.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"D:\projects\sloeber\/arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=10802 -DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR   -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\cores\arduino" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\variants\eightanaloginputs" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM" -I"D:\projects\sloeber\arduinoPlugin\packages\arduino\hardware\avr\1.6.11\libraries\EEPROM\src" -I"D:\projects\!!!SmartHouse\librarys\TimeTrigger" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


