/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-22     Yifang       the first version
 */
/* 实现串口中断方式进行串口字符的读取 */

#include <rtthread.h>
#include <rtdevice.h>
#include <rtdef.h>

#include "uart_raspi.h"

/* 串口信号量 */
rt_sem_t uart3_rx_sem = RT_NULL;

struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;      /* 初始化配置参数 */

static struct rt_thread uart_raspi_thread;
static char thread_stack[1024];

/* 串口设备句柄 */
static rt_device_t serial;

#define uart_name "uart3"      /* 串口设备名称 */

static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    rt_err_t result = RT_EOK;

    result = rt_sem_release(uart3_rx_sem);

    return result;
}

static int uart_getchar(void)
{
    char ch = 0;

    /* 从串口读取一个字节的数据，没有读到则等待接收信号量 */
    while (rt_device_read(serial, -1, &ch, 1) != 1)
    {
        /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
        rt_sem_take(uart3_rx_sem, RT_WAITING_FOREVER);
    }
    return ch;
}


static void uart_raspi_thread_entry(void *parameter)
{
    int ch = 0;

    while(1)
    {
        ch = (int)uart_getchar();
        rt_kprintf("Write data is 0x%x\n",ch);

        /* car control menu */
        rt_kprintf("please input your control mode:\n");
        rt_kprintf("1. car go forward :please input <f> || <F>\r\n");
        rt_kprintf("2. car go backup  :please input <b> || <B>\r\n");
        rt_kprintf("3. car turn left  :please input <l> || <L>\r\n");
        rt_kprintf("4. car turn right :please input <r> || <R>\r\n");
        rt_kprintf("5. car stop       :please input <p> || <P>\r\n");
        rt_kprintf("6. exit menu      :please input <c> || <C>\r\n");

        if (ch < 0)
        {
            continue;
        }

        /*
         * handle control key
         * go forward: 0x77 0x57   ->'w'&&'W'
         * go backup:  0x73 0x53 ->'s'&&'S'
         * turn left:  0x61 0x41 ->'a'&&'A'
         * turn right: 0x64 0x44 ->'d'&&'D'
         * car stop:   0x70 0x50   ->'p'&&'P'
         * exit menu:  0x3         ->'ctrl'+'C'
         */

        if (ch == 0x77 || ch == 0x57)
        {
            rt_enter_critical();
            car_forward();
            rt_exit_critical();
            rt_kprintf("0x%x:car_forward\n",ch);
            continue;
        }
        else if (ch == 0x73 || ch == 0x53)
        {
            rt_enter_critical();
            car_backup();
            rt_exit_critical();
            rt_kprintf("0x%x:car_backup\n",ch);
            continue;
        }
        else if (ch == 0x61 || ch == 0x41)
        {
            rt_enter_critical();
            car_TL();
            rt_exit_critical();
            rt_kprintf("0x%x:car_TL\n",ch);
            continue;
        }
        else if (ch == 0x64 || ch == 0x44)
        {
            rt_enter_critical();
            car_TR();
            rt_exit_critical();
            rt_kprintf("0x%x:car_TR\n",ch);
            continue;
        }

        else if (ch == 0x70 || ch == 0x50)
        {
            rt_enter_critical();
            car_stop();
            rt_exit_critical();
            rt_kprintf("0x%x:car_stop\n",ch);
            continue;
        }
        else if(ch == 0x3)
        {
            rt_kprintf("exit control!\n");
//            break;
            return ;
        }
    }

}

int uart_raspi_init(void)
{
    char *buf;

    config.baud_rate = BAUD_RATE_115200;        /* 修改串口波特率 */
    config.data_bits = DATA_BITS_8;             /* 数据位为8 */
    config.stop_bits = STOP_BITS_1;              /* 停止位为1 */
    config.rx_bufsz  = 128;
    config.parity    = PARITY_NONE;             /* 无奇偶校验位 */

    uart3_rx_sem = rt_sem_create("uart3_rx_sem", 0, RT_IPC_FLAG_FIFO);

    serial = rt_device_find(uart_name);
    if(serial == RT_NULL)
    {
        rt_kprintf("find &s failed!\n",serial);
        return RT_ERROR;
    }

    rt_device_open(serial,RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM | RT_DEVICE_OFLAG_RDWR); /* 中断轮询模式打开串口设备 */

    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);

    rt_device_write(serial, 0, &buf, sizeof(&buf));

    rt_thread_init(&uart_raspi_thread,                 /* 创建一个邮箱接收数据的线程 */
                    "uart_raspi_thread",
                    uart_raspi_thread_entry,
                    RT_NULL,
                    &thread_stack[0],
                    sizeof(thread_stack),
                    15,
                    1000);
    rt_thread_startup(&uart_raspi_thread);

    return 0;
}
//INIT_APP_EXPORT(uart_raspi_init);
MSH_CMD_EXPORT(uart_raspi_init,uart_raspi_init);
