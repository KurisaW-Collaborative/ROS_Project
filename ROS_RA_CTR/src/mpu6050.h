/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-05     Yifang       the first version
 */
#ifndef SRC_MPU6050_H_
#define SRC_MPU6050_H_

#include "mpu6xxx.h"

static struct mpu6xxx_3axes accel, gyro;

void mpu6050_RD();
void IMUupdate(rt_int16_t gx, rt_int16_t gy, rt_int16_t gz, rt_int16_t ax, rt_int16_t ay, rt_int16_t az);

#endif /* SRC_MPU6050_H_ */
