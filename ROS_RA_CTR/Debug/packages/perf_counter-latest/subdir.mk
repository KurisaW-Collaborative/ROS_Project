################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../packages/perf_counter-latest/perf_counter.c 

OBJS += \
./packages/perf_counter-latest/perf_counter.o 

C_DEPS += \
./packages/perf_counter-latest/perf_counter.d 


# Each subdirectory must supply rules for building sources it contributes
packages/perf_counter-latest/%.o: ../packages/perf_counter-latest/%.c
	arm-none-eabi-gcc -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\board\ports" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\board" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\libraries\HAL_Drivers\config" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\libraries\HAL_Drivers" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\packages\mpu6xxx-latest\inc" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\packages\perf_counter-latest" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\ra\fsp\inc\api" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\ra\fsp\inc\instances" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\ra\fsp\inc" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\ra_cfg\fsp_cfg\bsp" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\ra_cfg\fsp_cfg" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\ra_gen" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\components\drivers\include" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\components\drivers\sensors" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\components\finsh" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\components\libc\compilers\common" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\components\libc\compilers\newlib" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\components\libc\posix\io\poll" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\components\libc\posix\io\stdio" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\components\libc\posix\ipc" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\components\utilities\ulog" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\include" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\libcpu\arm\common" -I"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rt-thread\libcpu\arm\cortex-m4" -include"D:\RT-Thread\RT-ThreadStudio\workspace_backup\ROS_RA_CTR\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

