################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HC-06.c \
../src/controller.c \
../src/finsh_uart.c \
../src/hal_entry.c \
../src/pwm_config.c 

OBJS += \
./src/HC-06.o \
./src/controller.o \
./src/finsh_uart.o \
./src/hal_entry.o \
./src/pwm_config.o 

C_DEPS += \
./src/HC-06.d \
./src/controller.d \
./src/finsh_uart.d \
./src/hal_entry.d \
./src/pwm_config.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	arm-none-eabi-gcc -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\board\ports" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\board" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\libraries\HAL_Drivers\config" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\libraries\HAL_Drivers" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\ra\fsp\inc\api" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\ra\fsp\inc\instances" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\ra\fsp\inc" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\ra_cfg\fsp_cfg\bsp" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\ra_cfg\fsp_cfg" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\ra_gen" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rt-thread\components\drivers\include" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rt-thread\components\finsh" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rt-thread\components\libc\compilers\common" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rt-thread\components\libc\compilers\newlib" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rt-thread\components\libc\posix\io\poll" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rt-thread\components\libc\posix\io\stdio" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rt-thread\components\libc\posix\ipc" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rt-thread\include" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rt-thread\libcpu\arm\common" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rt-thread\libcpu\arm\cortex-m4" -include"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_Project-Team\car_pwm_ra\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

