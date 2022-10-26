/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-25     Yifang       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <shell.h>

//#include "controller.h"
#include "finsh_uart.h"

static char ch = 0;
static int count = 0;
static rt_thread_t uart_thread = RT_NULL;

void finsh_uart_thread_entry(void *parameter)
{
    while(1)
    {
        while((ch = (int)finsh_getchar()) != 0)
        {
            switch(ch)
            {
            case 0x00:
                car_stop();
                rt_kprintf("car_stop...\n");
            case 0x01:
                car_forward();
                rt_kprintf("car_forward...\n");
            case 0x02:
                car_backup();
                rt_kprintf("car_backup...\n");
            case 0x03:
                car_TL();
                rt_kprintf("car_TL...\n");
            case 0x04:
                car_TR();
                rt_kprintf("car_TR...\n");
            }
            count++;
        }
        break;
    }
    rt_thread_delete(uart_thread);
    rt_kprintf("finsh_uart_thread%d is delete...\n",count);
}

void finsh_uart_thread(void)
{
    uart_thread = rt_thread_create("finsh_uart_thread", finsh_uart_thread_entry, RT_NULL, 1024, 20, 50);
    if(uart_thread == RT_NULL)
    {
        rt_kprintf("finsh_uart_thread create failed...\n");
    }
    else
    {
        rt_thread_startup(uart_thread);
    }

}
//INIT_APP_EXPORT(finsh_uart_thread);
