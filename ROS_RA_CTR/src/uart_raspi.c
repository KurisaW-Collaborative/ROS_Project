/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-22     Yifang       the first version
 */
/* ʵ�ִ����жϷ�ʽ���д����ַ��Ķ�ȡ */
#include "uart_raspi.h"
#include "mpu6050.h"

extern float span;

char ch_buf = 0;

/* �����ź��� */
rt_sem_t uart3_rx_sem = RT_NULL;

struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;      /* ��ʼ�����ò��� */

/* �����豸��� */
rt_device_t serial_raspi;

/* ���ڽ���״̬��� */
rt_uint16_t USART_RX_STA;

#define uart_name "uart3"      /* �����豸���� */

static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    rt_err_t result = RT_EOK;

    result = rt_sem_release(uart3_rx_sem);
    USART_RX_STA = 1;   //�յ����ݣ���־λ��1

    return result;
}

static char uart_getchar(void)
{
    /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�����ȴ������ź��� */
    while (rt_device_read(serial_raspi, -1, &ch_buf, 1) != 1)
    {
        /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
        rt_sem_take(uart3_rx_sem, RT_WAITING_FOREVER);
        USART_RX_STA = 0;   //���ձ�־λ����
    }
    return ch_buf;
}


static void uart_raspi_thread_entry(void *parameter)
{
    /* menu */
    rt_kprintf("\x1b[2J\x1b[H");//ִ������
    rt_kprintf("-------------------------------------------------\n");
    rt_kprintf("Please input your control mode or other function:\n");
    rt_kprintf("1. Car control      :please input (1)\n");
    rt_kprintf("2. Mpu6050 Function :please input (2)\n");
    rt_kprintf("3. exit menu        :please input <c> || <C>\r\n");
    rt_kprintf("-------------------------------------------------\n");

    while((ch_buf = (int)uart_getchar()) != RT_NULL)
    {
        rt_kprintf("\nWrite data is 0x%x",ch_buf);
        //С�����ƽ���
        if(ch_buf == 0x31)
        {
            rt_kprintf("\x1b[2J\x1b[H");//ִ������
            rt_kprintf("--> car go forward      :please input <f> || <F>\r\n");
            rt_kprintf("--> car go backup       :please input <b> || <B>\r\n");
            rt_kprintf("--> car turn left       :please input <l> || <L>\r\n");
            rt_kprintf("--> car turn right      :please input <r> || <R>\r\n");
            rt_kprintf("--> car stop            :please input <p> || <P>\r\n");
            rt_kprintf("--> car control exit    :please input <ctl+c>");

            while((ch_buf = (int)uart_getchar()) != RT_NULL)
            {
                //С������
                if (ch_buf == 0x77 || ch_buf == 0x57)
                {
                    rt_kprintf("\n0x%x:car_forward",ch_buf);
                    car_forward();

                    continue;
                }
                else if (ch_buf == 0x73 || ch_buf == 0x53)
                {
                    rt_kprintf("\n0x%x:car_backup",ch_buf);
                    car_backup();

                    continue;
                }
                else if (ch_buf == 0x61 || ch_buf == 0x41)
                {
                    rt_kprintf("\n0x%x:car_TL",ch_buf);
                    car_TL();

                    continue;
                }
                else if (ch_buf == 0x64 || ch_buf == 0x44)
                {
                    rt_kprintf("\n0x%x:car_TR",ch_buf);
                    car_TR();

                    continue;
                }

                else if (ch_buf == 0x70 || ch_buf == 0x50)
                {
                    rt_kprintf("\n0x%x:car_stop",ch_buf);
                    car_stop();

                    continue;
                }
                else if(ch_buf == 0x3)
                {
                    rt_kprintf("\ncar control exit");
                    break;
                }
            }
        }
        //MPU6050��̬����
        else if(ch_buf == 0x32)
        {
            rt_kprintf("\x1b[2J\x1b[H");//ִ������
            while((ch_buf = (int)uart_getchar()) != RT_NULL)
            {
                mpu6050_RD();
                if(ch_buf == 0x6d)
                {
                    break;
                    rt_kprintf("\nexit mpu6050_RD");
                }
            }
        }
        //�˳��˵�
        else if(ch_buf == 0x63)
        {
            rt_kprintf("\x1b[2J\x1b[H");//ִ������
            rt_kprintf("\nexit menu successful!");
            break;
        }
    }
}

int uart_raspi_init(void)
{
    char *buf;

    config.baud_rate = BAUD_RATE_115200;        /* �޸Ĵ��ڲ����� */
    config.data_bits = DATA_BITS_8;             /* ����λΪ8 */
    config.stop_bits = STOP_BITS_1;              /* ֹͣλΪ1 */
    config.rx_bufsz  = 128;
    config.parity    = PARITY_NONE;             /* ����żУ��λ */

    uart3_rx_sem = rt_sem_create("uart3_rx_sem", 0, RT_IPC_FLAG_FIFO);

    serial_raspi = rt_device_find(uart_name);
    if(serial_raspi == RT_NULL)
    {
        rt_kprintf("find &s failed!\n",serial_raspi);
        return RT_ERROR;
    }

    rt_device_open(serial_raspi,RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_STREAM | RT_DEVICE_OFLAG_RDWR); /* �ж���ѯģʽ�򿪴����豸 */

    rt_device_control(serial_raspi, RT_DEVICE_CTRL_CONFIG, &config);

    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(serial_raspi, uart_input);

    rt_device_write(serial_raspi, 0, &buf, sizeof(&buf));

    rt_thread_t ret = rt_thread_create("uart_raspi_thread", uart_raspi_thread_entry, RT_NULL, 2048, 19, 100);
    rt_thread_startup(ret);

    return 0;
}
//INIT_APP_EXPORT(uart_raspi_init);
MSH_CMD_EXPORT(uart_raspi_init,uart_raspi_init);
