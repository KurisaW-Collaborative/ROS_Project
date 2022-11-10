/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-31     Yifang       the first version
 */
#include <rtdevice.h>

#include "mpu6050.h"

#define MPU6XXX_DEVICE_NAME  "i2c0"

struct mpu6xxx_device *dev;

void mpu6050_RD_thread_entry(void *parameter)
{

    int i;

    for (i = 0; i < 50; i++)
    {
        mpu6xxx_get_accel(dev, &accel);
        mpu6xxx_get_gyro(dev, &gyro);

        rt_kprintf("\n--------------The %d calculation--------------\n",i);
        rt_kprintf("| accel.x = %4d | accel.y = %4d | accel.z = %4d |\n", accel.x, accel.y, accel.z);
        rt_kprintf("| gyro.x = %4d  | gyro.y = %4d  | gyro.z = %4d  |\n", gyro.x, gyro.y, gyro.z);
        rt_kprintf("------------------------------------------------\n\n");

        rt_thread_mdelay(1000);
    }
    mpu6xxx_deinit(dev);
}

int mpu6050_RD(void *parameter)
{
    rt_thread_t ret = 0;

    dev = mpu6xxx_init(MPU6XXX_DEVICE_NAME, RT_NULL);

    if (dev == RT_NULL)
    {
        rt_kprintf("mpu6xxx init failed\n");
        return -1;
    }
    rt_kprintf("mpu6xxx init succeed\n");

    ret = rt_thread_create("mpu_thread", mpu6050_RD_thread_entry, RT_NULL, 512, 22, 500);
    if(ret == RT_NULL)
    {
        rt_kprintf("mpu_thread create failed.\n");
    }

    rt_thread_startup(ret);
}
MSH_CMD_EXPORT(mpu6050_RD,mpu6050_RD);
