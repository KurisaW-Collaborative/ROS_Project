/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-21     Yifang       the first version
 */
#ifndef SRC_ODOMETRY_H_
#define SRC_ODOMETRY_H_

#include "encoder_cb.h"
#include "mpu6050.h"

void odometry_thread(void);

#endif /* SRC_ODOMETRY_H_ */
