################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bridge.c \
../Core/Src/callbacks.c \
../Core/Src/led1202.c \
../Core/Src/led1202_reg.c \
../Core/Src/led12a1.c \
../Core/Src/main.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/system_stm32g0xx.c 

OBJS += \
./Core/Src/bridge.o \
./Core/Src/callbacks.o \
./Core/Src/led1202.o \
./Core/Src/led1202_reg.o \
./Core/Src/led12a1.o \
./Core/Src/main.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/system_stm32g0xx.o 

C_DEPS += \
./Core/Src/bridge.d \
./Core/Src/callbacks.d \
./Core/Src/led1202.d \
./Core/Src/led1202_reg.d \
./Core/Src/led12a1.d \
./Core/Src/main.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/system_stm32g0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B1xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/bridge.cyclo ./Core/Src/bridge.d ./Core/Src/bridge.o ./Core/Src/bridge.su ./Core/Src/callbacks.cyclo ./Core/Src/callbacks.d ./Core/Src/callbacks.o ./Core/Src/callbacks.su ./Core/Src/led1202.cyclo ./Core/Src/led1202.d ./Core/Src/led1202.o ./Core/Src/led1202.su ./Core/Src/led1202_reg.cyclo ./Core/Src/led1202_reg.d ./Core/Src/led1202_reg.o ./Core/Src/led1202_reg.su ./Core/Src/led12a1.cyclo ./Core/Src/led12a1.d ./Core/Src/led12a1.o ./Core/Src/led12a1.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32g0xx_hal_msp.cyclo ./Core/Src/stm32g0xx_hal_msp.d ./Core/Src/stm32g0xx_hal_msp.o ./Core/Src/stm32g0xx_hal_msp.su ./Core/Src/stm32g0xx_it.cyclo ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/stm32g0xx_it.su ./Core/Src/system_stm32g0xx.cyclo ./Core/Src/system_stm32g0xx.d ./Core/Src/system_stm32g0xx.o ./Core/Src/system_stm32g0xx.su

.PHONY: clean-Core-2f-Src

