################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/src/dma.c \
../src/src/estimation.c \
../src/src/exti.c \
../src/src/gpio.c \
../src/src/led.c \
../src/src/nvic.c \
../src/src/process.c \
../src/src/systick.c \
../src/src/usart2.c 

OBJS += \
./src/src/dma.o \
./src/src/estimation.o \
./src/src/exti.o \
./src/src/gpio.o \
./src/src/led.o \
./src/src/nvic.o \
./src/src/process.o \
./src/src/systick.o \
./src/src/usart2.o 

C_DEPS += \
./src/src/dma.d \
./src/src/estimation.d \
./src/src/exti.d \
./src/src/gpio.d \
./src/src/led.d \
./src/src/nvic.d \
./src/src/process.d \
./src/src/systick.d \
./src/src/usart2.d 


# Each subdirectory must supply rules for building sources it contributes
src/src/%.o: ../src/src/%.c
	@echo compiling  $< & arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -DARM_MATH_CM4 -D__FPU_PRESENT=1 -D__FPU_USED=1 -I"G:\STM\TrueSTUDIO for STM32 9.3.0\ARMTools\arm-atollic-eabi\include" -I"G:\STM\TrueSTUDIO for STM32 9.3.0\ARMTools\lib\gcc\arm-atollic-eabi\6.3.1\include" -I"G:\STM\TrueSTUDIO for STM32 9.3.0\ARMTools\lib\gcc\arm-atollic-eabi\6.3.1\include-fixed" -I../Libraries/CMSIS/Include -I../Libraries/Device/STM32F4xx/Include -I../src -I../src/inc -Ofast -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

