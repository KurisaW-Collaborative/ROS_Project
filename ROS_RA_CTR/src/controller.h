/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-06     Yifang       the first version
 */


#ifndef _SRC_CONTROLLER_H_
#define _SRC_CONTROLLER_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "hal_data.h"
#include "pwm_config.h"
#include "pid.h"

#define ENABLE_CAR_MSH 1
//rt_uint8_t path_num=5;
//int turn_flag=0;

#define AIN1_PIN "P403"
#define AIN2_PIN "P404"
#define BIN1_PIN "P405"
#define BIN2_PIN "P406"

#define PATH_INF_MAX 10
#define PWM_PERCENT_INIT 50

#define UPLOAD_EVENT (1<<8)
#define UPLOAD_EVENT_1 (1<<7)

enum CAR_S
{
    CAR_RUN,
    CAR_STOP,
    CAR_ARRIVE,
    CAR_ERR
};

struct Controller
{
    enum CAR_S car_status;

    rt_uint8_t arrive_flag; /* 到达flag */
    int path_inf[PATH_INF_MAX]; /* 路径信息 */

    rt_uint8_t left_pulse;  /* 左占空比 */
    rt_uint8_t right_pulse; /* 右占空比 */

    rt_err_t (*start)(int argc, char **argv);
    rt_err_t (*stop)(void);
    rt_err_t (*left)(void);
    rt_err_t (*right)(void);
    rt_err_t (*turn)(int argc, char ** argv);
};

int car_init(void);
rt_err_t car_start(int argc, char **argv);
rt_err_t car_stop(void);

rt_err_t car_forward(void);
rt_err_t car_backup(void);
rt_err_t car_TL(void);
rt_err_t car_TR(void);

#endif /* SRC_CONTROLLER_H_ */













