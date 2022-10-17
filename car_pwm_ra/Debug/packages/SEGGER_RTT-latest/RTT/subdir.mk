################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../packages/SEGGER_RTT-latest/RTT/SEGGER_RTT.c \
../packages/SEGGER_RTT-latest/RTT/SEGGER_RTT_printf.c 

OBJS += \
./packages/SEGGER_RTT-latest/RTT/SEGGER_RTT.o \
./packages/SEGGER_RTT-latest/RTT/SEGGER_RTT_printf.o 

C_DEPS += \
./packages/SEGGER_RTT-latest/RTT/SEGGER_RTT.d \
./packages/SEGGER_RTT-latest/RTT/SEGGER_RTT_printf.d 


# Each subdirectory must supply rules for building sources it contributes
packages/SEGGER_RTT-latest/RTT/%.o: ../packages/SEGGER_RTT-latest/RTT/%.c
	arm-none-eabi-gcc -I"D:\RT-ThreadStudio\workspace\car_pwm_ra" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\board\ports" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\board" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\libraries\HAL_Drivers\config" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\libraries\HAL_Drivers" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\packages\SEGGER_RTT-latest\RTT" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\packages\SEGGER_RTT-latest" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\ra\fsp\inc\api" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\ra\fsp\inc\instances" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\ra\fsp\inc" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\ra_cfg\fsp_cfg\bsp" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\ra_cfg\fsp_cfg" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\ra_gen" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\rt-thread\components\drivers\include" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\rt-thread\components\finsh" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\rt-thread\components\libc\compilers\common" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\rt-thread\components\libc\compilers\newlib" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\rt-thread\components\libc\posix\io\poll" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\rt-thread\components\libc\posix\io\stdio" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\rt-thread\components\libc\posix\ipc" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\rt-thread\include" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\rt-thread\libcpu\arm\common" -I"D:\RT-ThreadStudio\workspace\car_pwm_ra\rt-thread\libcpu\arm\cortex-m4" -include"D:\RT-ThreadStudio\workspace\car_pwm_ra\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

