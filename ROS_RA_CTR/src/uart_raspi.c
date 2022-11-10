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
#include "uart_raspi.h"

/* 用于pid基准值 */
rt_uint32_t number = 0;

/* 串口信号量 */
rt_sem_t uart3_rx_sem = RT_NULL;

struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;      /* 初始化配置参数 */

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
    int char_flag = RT_NULL;        //输入字符标志位
//    fsp_err_t err = FSP_SUCCESS;

    /* 定时器使能，用于捕获中断引脚计数 */
    (void) R_GPT_Enable(&g_timer3_ctrl);
    (void) R_GPT_Enable(&g_timer2_ctrl);

    /* car control menu */
    rt_kprintf("please input your control mode:\n");
    rt_kprintf("1. car go forward :please input <f> || <F>\r\n");
    rt_kprintf("2. car go backup  :please input <b> || <B>\r\n");
    rt_kprintf("3. car turn left  :please input <l> || <L>\r\n");
    rt_kprintf("4. car turn right :please input <r> || <R>\r\n");
    rt_kprintf("5. car stop       :please input <p> || <P>\r\n");
    rt_kprintf("6. exit menu      :please input <c> || <C>\r\n");

    while(1)
    {
        while((ch = (int)uart_getchar()) != RT_NULL)
        {
            rt_kprintf("Write data is 0x%x\n",ch);

            if(ch != RT_NULL)       // 对输入字符进行检测，如果收到字符，将标志位置1
            {
                char_flag = 1;
                if (ch < 0)
                {
                    continue;
                }

                if (ch == 0x77 || ch == 0x57)
                {
                    rt_kprintf("0x%x:car_forward\n",ch);
                    car_forward();
                    char_flag = 0;

                    continue;
                }
                else if (ch == 0x73 || ch == 0x53)
                {
                    rt_kprintf("0x%x:car_backup\n",ch);
                    car_backup();
                    char_flag = 0;

                    continue;
                }
                else if (ch == 0x61 || ch == 0x41)
                {
                    rt_kprintf("0x%x:car_TL\n",ch);
                    car_TL();
                    char_flag = 0;

                    continue;
                }
                else if (ch == 0x64 || ch == 0x44)
                {
                    rt_kprintf("0x%x:car_TR\n",ch);
                    car_TR();
                    char_flag = 0;

                    continue;
                }

                else if (ch == 0x70 || ch == 0x50)
                {
                    rt_kprintf("0x%x:car_stop\n",ch);
                    car_stop();
                    char_flag = 0;

                    continue;
                }
                else if(ch == 0x3)
                {
                    rt_kprintf("exit control!\n");
                    char_flag = 0;
                    break;
                }
                else
                {
                    continue;
                }
            }

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

    rt_thread_t ret = rt_thread_create("uart_raspi_thread", uart_raspi_thread_entry, RT_NULL, 2048, 15, 1000);
    rt_thread_startup(ret);

    return 0;
}
//INIT_APP_EXPORT(uart_raspi_init);
MSH_CMD_EXPORT(uart_raspi_init,uart_raspi_init);
