/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-06     Yifang       the first version
 */
#ifndef SRC_ENCODER_CB_H_
#define SRC_ENCODER_TEST_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "hal_data.h"

void exit1_callback(external_irq_callback_args_t *p_args);
void exit4_callback(external_irq_callback_args_t *p_args);

#endif /* SRC_ENCODER_CB_H_ */
