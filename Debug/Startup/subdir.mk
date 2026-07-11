################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f411ceux.s 

OBJS += \
./Startup/startup_stm32f411ceux.o 

S_DEPS += \
./Startup/startup_stm32f411ceux.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/madhav-gaur/Programming/Embedded /Embedded-C/My_workspace/STM32F411CEU6/4DRIVER/Drivers/Inc" -I"/home/madhav-gaur/Programming/Embedded /Embedded-C/My_workspace/STM32F411CEU6/4DRIVER/Drivers/Src" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Startup

clean-Startup:
	-$(RM) ./Startup/startup_stm32f411ceux.d ./Startup/startup_stm32f411ceux.o

.PHONY: clean-Startup

