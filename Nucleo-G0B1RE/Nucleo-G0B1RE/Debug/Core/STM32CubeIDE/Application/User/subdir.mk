################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/STM32CubeIDE/Application/User/syscalls.c \
../Core/STM32CubeIDE/Application/User/sysmem.c 

OBJS += \
./Core/STM32CubeIDE/Application/User/syscalls.o \
./Core/STM32CubeIDE/Application/User/sysmem.o 

C_DEPS += \
./Core/STM32CubeIDE/Application/User/syscalls.d \
./Core/STM32CubeIDE/Application/User/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Core/STM32CubeIDE/Application/User/%.o Core/STM32CubeIDE/Application/User/%.su Core/STM32CubeIDE/Application/User/%.cyclo: ../Core/STM32CubeIDE/Application/User/%.c Core/STM32CubeIDE/Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-STM32CubeIDE-2f-Application-2f-User

clean-Core-2f-STM32CubeIDE-2f-Application-2f-User:
	-$(RM) ./Core/STM32CubeIDE/Application/User/syscalls.cyclo ./Core/STM32CubeIDE/Application/User/syscalls.d ./Core/STM32CubeIDE/Application/User/syscalls.o ./Core/STM32CubeIDE/Application/User/syscalls.su ./Core/STM32CubeIDE/Application/User/sysmem.cyclo ./Core/STM32CubeIDE/Application/User/sysmem.d ./Core/STM32CubeIDE/Application/User/sysmem.o ./Core/STM32CubeIDE/Application/User/sysmem.su

.PHONY: clean-Core-2f-STM32CubeIDE-2f-Application-2f-User

