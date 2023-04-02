################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/STM32CubeIDE/Application/Startup/startup_stm32l073rztx.s 

OBJS += \
./Core/STM32CubeIDE/Application/Startup/startup_stm32l073rztx.o 

S_DEPS += \
./Core/STM32CubeIDE/Application/Startup/startup_stm32l073rztx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/STM32CubeIDE/Application/Startup/%.o: ../Core/STM32CubeIDE/Application/Startup/%.s Core/STM32CubeIDE/Application/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Core-2f-STM32CubeIDE-2f-Application-2f-Startup

clean-Core-2f-STM32CubeIDE-2f-Application-2f-Startup:
	-$(RM) ./Core/STM32CubeIDE/Application/Startup/startup_stm32l073rztx.d ./Core/STM32CubeIDE/Application/Startup/startup_stm32l073rztx.o

.PHONY: clean-Core-2f-STM32CubeIDE-2f-Application-2f-Startup

