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
#include "mpu6xxx.h"
#include "sensor_inven_mpu6xxx.h"

struct mpu6xxx_device *mpu_device;
struct mpu6xxx_3axes *gyro;


void mpu6050_RD_thread_entry(void *parameter)
{


    while(1)
    {
        mpu6xxx_get_gyro(mpu_device, gyro);
        rt_kprintf("X:%d Y:%d Z:%d\n", gyro->x, gyro->y, gyro->z);
        rt_thread_delay(1000);
    }


}

int mpu6050_RD(void *parameter)
{
    rt_thread_t ret = 0;

    struct rt_sensor_config cfg;

    /* mpu6050 config */
    cfg.intf.dev_name = "i2c0";
    cfg.intf.user_data = (void *)MPU6XXX_ADDR_DEFAULT;
    cfg.irq_pin.pin = RT_PIN_NONE;

    /* mpu6050 init */
    rt_hw_mpu6xxx_init("mpu", &cfg);

    ret = rt_thread_create("mpu_thread", mpu6050_RD_thread_entry, RT_NULL, 512, 18, 500);
    if(ret == RT_NULL)
    {
        rt_kprintf("mpu_thread create failed.\n");
    }

    rt_thread_startup(ret);
}
MSH_CMD_EXPORT(mpu6050_RD,mpu6050_RD);
