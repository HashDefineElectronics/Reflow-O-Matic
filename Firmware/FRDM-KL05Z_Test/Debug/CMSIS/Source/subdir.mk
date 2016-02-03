################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/Source/system_MKL05Z4.c 

S_UPPER_SRCS += \
../CMSIS/Source/startup_MKL05Z4.S 

OBJS += \
./CMSIS/Source/startup_MKL05Z4.o \
./CMSIS/Source/system_MKL05Z4.o 

C_DEPS += \
./CMSIS/Source/system_MKL05Z4.d 

S_UPPER_DEPS += \
./CMSIS/Source/startup_MKL05Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/Source/%.o: ../CMSIS/Source/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -x assembler-with-cpp -I"/home/vagrant/Documents/Reflow-O-Matic/Firmware/FRDM-KL05Z_Test" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CMSIS/Source/%.o: ../CMSIS/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -I"/home/vagrant/Documents/Reflow-O-Matic/Firmware/FRDM-KL05Z_Test" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


