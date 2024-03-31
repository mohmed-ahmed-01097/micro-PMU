################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/CMSIS/Source/FastMathFunctions/arm_cos_f32.c \
../Libraries/CMSIS/Source/FastMathFunctions/arm_sin_f32.c 

OBJS += \
./Libraries/CMSIS/Source/FastMathFunctions/arm_cos_f32.o \
./Libraries/CMSIS/Source/FastMathFunctions/arm_sin_f32.o 

C_DEPS += \
./Libraries/CMSIS/Source/FastMathFunctions/arm_cos_f32.d \
./Libraries/CMSIS/Source/FastMathFunctions/arm_sin_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/CMSIS/Source/FastMathFunctions/%.o: ../Libraries/CMSIS/Source/FastMathFunctions/%.c
	@echo compiling  $< & arm-atollic-eabi-gcc -c "$<" -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -DSTM32F4XX -DSTM32F40XX -DUSE_STDPERIPH_DRIVER -DARM_MATH_CM4 -D__FPU_PRESENT=1 -D__FPU_USED=1 -I"G:\STM\TrueSTUDIO for STM32 9.3.0\ARMTools\arm-atollic-eabi\include" -I"G:\STM\TrueSTUDIO for STM32 9.3.0\ARMTools\lib\gcc\arm-atollic-eabi\6.3.1\include" -I"G:\STM\TrueSTUDIO for STM32 9.3.0\ARMTools\lib\gcc\arm-atollic-eabi\6.3.1\include-fixed" -I../Libraries/CMSIS/Include -I../Libraries/Device/STM32F4xx/Include -I../src -I../src/inc -Og -ffunction-sections -fdata-sections -g -fstack-usage -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -specs=nano.specs -o "$@"

