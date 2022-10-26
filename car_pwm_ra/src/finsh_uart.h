/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-25     Yifang       the first version
 */
#ifndef SRC_FINSH_UART_H_
#define SRC_FINSH_UART_H_

#include <rtthread.h>
#include <rtdevice.h>
#include <shell.h>

#include "controller.h"

void finsh_uart_thread(void);

#endif /* SRC_FINSH_UART_H_ */
