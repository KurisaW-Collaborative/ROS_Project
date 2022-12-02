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

#define SAMPLE_UART_NAME                 "uart3"
#define DATA_CMD_END                     '\r'       /* ����λ����Ϊ \r�����س��� */
#define ONE_DATA_MAXLEN                  10         /* ���������ݵ���󳤶� */

/* ���ڽ�����Ϣ���ź��� */
static struct rt_semaphore rx_sem;
rt_device_t ros_serial;

extern odometry_data[21];

float odometry_right=0,odometry_left=0;//���ڵõ����������ٶ�

union recieveData  //���յ�������
{
    float d;    //�������ٶ�
    unsigned char data[4];
}leftdata,rightdata;       //���յ�����������

extern float rotate_speed2;
extern float rotate_speed1;

char recv_data[] = {0};

/* �������ݻص����� */
static rt_err_t uart_rx_ind(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    if (size > 0)
    {
        rt_sem_release(&rx_sem);
    }
    return RT_EOK;
}

static char uart_sample_get_char(void)
{
    char ch;

    while (rt_device_read(ros_serial, 0, &ch, 1) == 0)
    {
        rt_sem_control(&rx_sem, RT_IPC_CMD_RESET, RT_NULL);
        rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
    }
    return ch;
}

/* ���ݽ����߳� */
static void data_parsing(void)
{
    char ch;
    char data[ONE_DATA_MAXLEN];
    static char i = 0;

    /* ������̼��߳� */
    odometry_thread();

    while (1)
    {
        ch = uart_sample_get_char();
        rt_device_write(ros_serial, 0, &ch, 1);
        if(ch == DATA_CMD_END)
        {
            if(sizeof(data) == 10)
            {
                //�����������ٶ�
                for(int t=0;t<4;t++)
                {
                    rightdata.data[t]=data[t];
                    leftdata.data[t]=data[t+4];
                }

                //�����������ٶ�
                odometry_right = rightdata.d;
                odometry_left = leftdata.d;

//                rotate_speed2 = rightdata.d;
//                rotate_speed1 = leftdata.d;
            }

            data[i++] = '\0';
            rt_kprintf("data=%s\r\n",data);
            i = 0;
            continue;
        }
        i = (i >= ONE_DATA_MAXLEN-1) ? ONE_DATA_MAXLEN-1 : i;
        data[i++] = ch;
    }
}

static int uart_data_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    char uart_name[RT_NAME_MAX];
    char str[] = "hello RT-Thread & Raspi!\r\n";

    if (argc == 2)
    {
        rt_strncpy(uart_name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(uart_name, SAMPLE_UART_NAME, RT_NAME_MAX);
    }

    /* ����ϵͳ�еĴ����豸 */
    ros_serial = rt_device_find(uart_name);
    if (!ros_serial)
    {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    /* ��ʼ���ź��� */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* ���жϽ��ռ���ѯ����ģʽ�򿪴����豸 */
    rt_device_open(ros_serial, RT_DEVICE_FLAG_INT_RX);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(ros_serial, uart_rx_ind);
    /* �����ַ��� */
    rt_device_write(ros_serial, 0, str, (sizeof(str) - 1));

    /* ���� ros_serial �߳� */
    rt_thread_t thread = rt_thread_create("ros_serial", (void (*)(void *parameter))data_parsing, RT_NULL, 1024, 25, 10);
    /* �����ɹ��������߳� */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}

/* ������ msh �����б��� */
MSH_CMD_EXPORT(uart_data_sample, uart device sample);
