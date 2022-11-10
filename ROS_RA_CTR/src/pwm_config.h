/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-16     金鑫       the first version
 */
#ifndef SRC_PWM_CONFIG_H_
#define SRC_PWM_CONFIG_H_

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>

#define PWM1_DEV_NAME   ("pwm8")
#define PWM1_CH         (0)

#define PWM2_DEV_NAME   ("pwm8")
#define PWM2_CH         (1)

#define PWM_PERIOD  1000000

int pwm_init(void);
rt_err_t pwm_set_pulse(struct rt_device_pwm * pwm_dev,rt_uint32_t pulse_val);
rt_err_t pwm_enable(struct rt_device_pwm * pwm_dev);
rt_err_t pwm_disable(struct rt_device_pwm * pwm_dev);

#endif /* SRC_PWM_CONFIG_H_ */
