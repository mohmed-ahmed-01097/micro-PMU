################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Libraries/Device/STM32F4xx/src/startup_stm32f40xx.s 

C_SRCS += \
../Libraries/Device/STM32F4xx/src/stm32f4xx_it.c \
../Libraries/Device/STM32F4xx/src/system_stm32f4xx.c 

OBJS += \
./Libraries/Device/STM32F4xx/src/startup_stm32f40xx.o \
./Libraries/Device/STM32F4xx/src/stm32f4xx_it.o \
./Libraries/Device/STM32F4xx/src/system_stm32f4xx.o 

C_DEPS += \
./Libraries/Device/STM32F4xx/src/stm32f4xx_it.d \
./Libraries/Device/STM32F4xx/src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/Device/STM32F4xx/src/%.o: ../Libraries/Device/STM32F4xx/src/%.s
	@echo assembling $< & arm-atollic-eabi-gcc -c -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -I../src -I../Libraries/CMSIS/Include -I../Libraries/Device/STM32F4xx/Include -I../Libraries/STM32F4xx_StdPeriph_Driver/inc -g -Wa,--warn -x assembler-with-cpp -specs=nano.specs -o "$@" "$<"
Libraries/Device/STM32F4xx/src/%.o: ../Libraries/Device/STM32F4xx/src/%.c
	@echo compiling  $< & arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -DARM_MATH_CM4 -D__FPU_PRESENT=1 -D__FPU_USED=1 -I"G:\STM\TrueSTUDIO for STM32 9.3.0\ARMTools\arm-atollic-eabi\include" -I"G:\STM\TrueSTUDIO for STM32 9.3.0\ARMTools\lib\gcc\arm-atollic-eabi\6.3.1\include" -I"G:\STM\TrueSTUDIO for STM32 9.3.0\ARMTools\lib\gcc\arm-atollic-eabi\6.3.1\include-fixed" -I../Libraries/CMSIS/Include -I../Libraries/Device/STM32F4xx/Include -I../src -I../src/inc -Ofast -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

