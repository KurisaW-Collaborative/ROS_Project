/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-22     Yifang       the first version
 */
#include <rtthread.h>
#include <rtdevice.h>

/* 串口信号量 */
rt_sem_t uart3_rx_sem = RT_NULL;

/* 串口字符收发缓冲区 */
static char *buf = "hello HC-06!\r\n";

struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;      /* 初始化配置参数 */

static struct rt_thread HC_05_thread;
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

static void HC_05_thread_entry(void *parameter)
{
    rt_err_t ret = RT_EOK;
    char *buf;

    while(1)
    {
        /* 从串口读取n个字节的数据，没有读到则等待接收信号量 */
        while(rt_device_read(serial,-1,&buf,sizeof(&buf)) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(uart3_rx_sem, RT_WAITING_FOREVER);
        }

        ret = rt_device_write(serial,0,&buf,sizeof(&buf));
        if(ret == 0)
        {
            buf = "rt_device_write failed!\r\n";
            rt_device_write(serial, 0, &buf, sizeof(&buf));
            rt_kprintf("rt_device_write failed!\r\n");
        }
        else if(ret == 'F')
        {
            car_forward();
        }
    }
}

int uart3_blutooth(void)
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

    rt_device_open(serial,RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_RDWR ); /* 中断轮询模式打开串口设备 */

    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);

    rt_device_write(serial, 0, &buf, sizeof(&buf));

    rt_thread_init(&HC_05_thread,                 /* 创建一个邮箱接收数据的线程 */
                    "HC_05_thread",
                    HC_05_thread_entry,
                    RT_NULL,
                    &thread_stack[0],
                    sizeof(thread_stack),
                    15,
                    1000);
    rt_thread_startup(&HC_05_thread);

    return 0;
}
//INIT_APP_EXPORT(uart3_blutooth);
MSH_CMD_EXPORT(uart3_blutooth,uart3_blutooth);
