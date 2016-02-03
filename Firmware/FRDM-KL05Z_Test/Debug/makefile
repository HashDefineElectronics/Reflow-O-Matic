################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Terminal/subdir.mk
-include PID/subdir.mk
-include Driver/UART/subdir.mk
-include Driver/Tick/subdir.mk
-include Driver/LED/subdir.mk
-include CMSIS/Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
FRDM-KL05Z_Test.hex \

SECONDARY_SIZE += \
FRDM-KL05Z_Test.siz \


# All Target
all: FRDM-KL05Z_Test.elf secondary-outputs

# Tool invocations
FRDM-KL05Z_Test.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -T "/home/vagrant/Documents/Reflow-O-Matic/Firmware/FRDM-KL05Z_Test/CMSIS/Source/MKL05Z32xxx4_flash.ld" -Xlinker --gc-sections -Wl,-Map,"FRDM-KL05Z_Test.map" --specs=nano.specs --specs=nosys.specs -o "FRDM-KL05Z_Test.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

FRDM-KL05Z_Test.hex: FRDM-KL05Z_Test.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "FRDM-KL05Z_Test.elf"  "FRDM-KL05Z_Test.hex"
	@echo 'Finished building: $@'
	@echo ' '

FRDM-KL05Z_Test.siz: FRDM-KL05Z_Test.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "FRDM-KL05Z_Test.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(SECONDARY_FLASH)$(S_UPPER_DEPS) FRDM-KL05Z_Test.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
