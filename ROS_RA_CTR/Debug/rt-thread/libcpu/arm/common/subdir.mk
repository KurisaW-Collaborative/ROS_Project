################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/libcpu/arm/common/backtrace.c \
../rt-thread/libcpu/arm/common/div0.c \
../rt-thread/libcpu/arm/common/showmem.c 

OBJS += \
./rt-thread/libcpu/arm/common/backtrace.o \
./rt-thread/libcpu/arm/common/div0.o \
./rt-thread/libcpu/arm/common/showmem.o 

C_DEPS += \
./rt-thread/libcpu/arm/common/backtrace.d \
./rt-thread/libcpu/arm/common/div0.d \
./rt-thread/libcpu/arm/common/showmem.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/libcpu/arm/common/%.o: ../rt-thread/libcpu/arm/common/%.c
	arm-none-eabi-gcc -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\board\ports" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\board" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\libraries\HAL_Drivers\config" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\libraries\HAL_Drivers" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\packages\mpu6xxx-latest\inc" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra\fsp\inc\api" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra\fsp\inc\instances" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra\fsp\inc" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra_cfg\fsp_cfg\bsp" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra_cfg\fsp_cfg" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra_gen" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\drivers\include" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\drivers\sensors" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\finsh" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\libc\compilers\common" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\libc\compilers\newlib" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\libc\posix\io\poll" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\libc\posix\io\stdio" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\libc\posix\ipc" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\utilities\ulog" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\include" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\libcpu\arm\common" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\libcpu\arm\cortex-m4" -include"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

