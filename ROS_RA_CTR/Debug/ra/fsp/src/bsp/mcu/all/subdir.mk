################################################################################
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/mcu/all/bsp_clocks.c \
../ra/fsp/src/bsp/mcu/all/bsp_common.c \
../ra/fsp/src/bsp/mcu/all/bsp_delay.c \
../ra/fsp/src/bsp/mcu/all/bsp_group_irq.c \
../ra/fsp/src/bsp/mcu/all/bsp_guard.c \
../ra/fsp/src/bsp/mcu/all/bsp_io.c \
../ra/fsp/src/bsp/mcu/all/bsp_irq.c \
../ra/fsp/src/bsp/mcu/all/bsp_register_protection.c \
../ra/fsp/src/bsp/mcu/all/bsp_rom_registers.c \
../ra/fsp/src/bsp/mcu/all/bsp_sbrk.c \
../ra/fsp/src/bsp/mcu/all/bsp_security.c 

OBJS += \
./ra/fsp/src/bsp/mcu/all/bsp_clocks.o \
./ra/fsp/src/bsp/mcu/all/bsp_common.o \
./ra/fsp/src/bsp/mcu/all/bsp_delay.o \
./ra/fsp/src/bsp/mcu/all/bsp_group_irq.o \
./ra/fsp/src/bsp/mcu/all/bsp_guard.o \
./ra/fsp/src/bsp/mcu/all/bsp_io.o \
./ra/fsp/src/bsp/mcu/all/bsp_irq.o \
./ra/fsp/src/bsp/mcu/all/bsp_register_protection.o \
./ra/fsp/src/bsp/mcu/all/bsp_rom_registers.o \
./ra/fsp/src/bsp/mcu/all/bsp_sbrk.o \
./ra/fsp/src/bsp/mcu/all/bsp_security.o 

C_DEPS += \
./ra/fsp/src/bsp/mcu/all/bsp_clocks.d \
./ra/fsp/src/bsp/mcu/all/bsp_common.d \
./ra/fsp/src/bsp/mcu/all/bsp_delay.d \
./ra/fsp/src/bsp/mcu/all/bsp_group_irq.d \
./ra/fsp/src/bsp/mcu/all/bsp_guard.d \
./ra/fsp/src/bsp/mcu/all/bsp_io.d \
./ra/fsp/src/bsp/mcu/all/bsp_irq.d \
./ra/fsp/src/bsp/mcu/all/bsp_register_protection.d \
./ra/fsp/src/bsp/mcu/all/bsp_rom_registers.d \
./ra/fsp/src/bsp/mcu/all/bsp_sbrk.d \
./ra/fsp/src/bsp/mcu/all/bsp_security.d 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/mcu/all/%.o: ../ra/fsp/src/bsp/mcu/all/%.c
	arm-none-eabi-gcc -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\board\ports" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\board" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\libraries\HAL_Drivers\config" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\libraries\HAL_Drivers" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\packages\mpu6xxx-latest\inc" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra\arm\CMSIS_5\CMSIS\Core\Include" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra\fsp\inc\api" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra\fsp\inc\instances" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra\fsp\inc" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra_cfg\fsp_cfg\bsp" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra_cfg\fsp_cfg" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\ra_gen" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\drivers\include" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\drivers\sensors" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\finsh" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\libc\compilers\common" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\libc\compilers\newlib" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\libc\posix\io\poll" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\libc\posix\io\stdio" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\libc\posix\ipc" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\components\utilities\ulog" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\include" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\libcpu\arm\common" -I"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rt-thread\libcpu\arm\cortex-m4" -include"D:\Desktop\ROS_Project-Team\ROS_RA_CTR\rtconfig_preinc.h" -std=gnu11 -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

