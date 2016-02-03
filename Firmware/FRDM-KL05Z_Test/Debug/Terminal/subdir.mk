################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Terminal/DebugInterface.c \
../Terminal/Debug_MainMenu.c 

OBJS += \
./Terminal/DebugInterface.o \
./Terminal/Debug_MainMenu.o 

C_DEPS += \
./Terminal/DebugInterface.d \
./Terminal/Debug_MainMenu.d 


# Each subdirectory must supply rules for building sources it contributes
Terminal/%.o: ../Terminal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -I"/home/vagrant/Documents/Reflow-O-Matic/Firmware/FRDM-KL05Z_Test" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


