################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../PID/pid.c 

OBJS += \
./PID/pid.o 

C_DEPS += \
./PID/pid.d 


# Each subdirectory must supply rules for building sources it contributes
PID/%.o: ../PID/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -I"/home/vagrant/Documents/Reflow/Firmware/Reflow-O-Matic" -I"/home/vagrant/Documents/Reflow/Firmware/Reflow-O-Matic/CMSIS/Header" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

