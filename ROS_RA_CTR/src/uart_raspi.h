/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-20     ASUS       the first version
 */
#ifndef SRC_HC_06_H_
#define SRC_HC_06_H_

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>

#include "controller.h"
#include "pwm_config.h"
#include "src/encoder_cb.h"

extern rt_err_t car_forward(void);
extern rt_err_t car_backup(void);
extern rt_err_t car_TL(void);
extern rt_err_t car_TR(void);
extern rt_err_t car_stop(void);

int uart_raspi_init(void);

#endif /* SRC_HC_06_H_ */
