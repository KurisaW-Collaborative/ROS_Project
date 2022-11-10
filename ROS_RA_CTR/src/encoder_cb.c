/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-05     Yifang       the first version
 */
#include "encoder_cb.h"
#include <sys/time.h>
#include <time.h>

#define DBG_LVL               DBG_LOG
#include <rtdbg.h>

bsp_io_level_t p_port_value_port_104;   //pin�豸���
bsp_io_level_t p_port_value_port_111;   //pin�豸���
timer_info_t info;                      //��ʱ�����

static rt_bool_t time_flag = 0;                //�����ر�־λ
static uint32_t g_capture_num = 0;               //����������֮�������
static uint32_t g_capture_num0 = 0;              //��0�������ض�ʱ������ֵ
static uint32_t g_capture_num1 = 0;              //��1�������ض�ʱ������ֵ
static uint32_t g_capture_frequency = 0;         //Ƶ��
static uint32_t g_capture_duty_cycle = 0;        //ռ�ձ�
static uint32_t g_capture_duty_cycle_num = 0;    //+ռ�ձ�����
static uint32_t g_period_num = 0;                //�������ڴ���
static uint32_t g_capture_duty_cycle_num0 =0;//�½��ض�ʱ������ֵ

static uint32_t g2_capture_num = 0;               //����������֮�������
static uint32_t g2_capture_num0 = 0;              //��0�������ض�ʱ������ֵ
static uint32_t g2_capture_num1 = 0;              //��1�������ض�ʱ������ֵ
static uint32_t g2_capture_frequency = 0;         //Ƶ��
static uint32_t g2_capture_duty_cycle = 0;        //ռ�ձ�
static uint32_t g2_capture_duty_cycle_num = 0;    //+ռ�ձ�����
static uint32_t g2_period_num = 0;                //�������ڴ���
static uint32_t g2_capture_duty_cycle_num0 =0;//�½��ض�ʱ������ֵ

static int count1 = 0;
static uint32_t g_capture_num1_total = 0;       //�����������ڴ洢13�������������ܵļ���ֵ
static uint32_t g_capture_duty_cycle_num1_total = 0;     //���ڴ洢13���½��������ܼ���ֵ

static int count2 = 0;
static uint32_t g_capture_num2_total = 0;       //�����������ڴ洢13�������������ܵļ���ֵ
static uint32_t g_capture_duty_cycle_num2_total = 0;     //���ڴ洢13���½��������ܼ���ֵ



/* Callback function */
void exit1_callback(external_irq_callback_args_t *p_args)
{
    time_t now;

    /*��ȡ�˿ڵ�ƽ״̬������ǵ͵�ƽ���������½��أ��ߵ�ƽ����������*/
    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_04, &p_port_value_port_104);        //��ȡ���������ŵ�ƽײ��״̬

    /* ��ȡ��ʱ������ֵ */
    timer_info_t info;
    (void) R_GPT_InfoGet(&g_timer3_ctrl, &info);

    uint32_t frequency1 = info.clock_frequency;//��ʱ��ʱ��Ƶ�ʣ�50MHz
    uint32_t current_period_counts1 = info.period_counts;//��ʱ������ֵ

    /* ��ȡ��ǰ����ֵ */
    timer_status_t status;
    (void) R_GPT_StatusGet(&g_timer3_ctrl, &status);

    if(p_port_value_port_104==BSP_IO_LEVEL_HIGH)//������
       {
           if(time_flag == 0)
           {
               time_flag = 1;     //������״̬Ϊ������ʱ������Ҫ�Լ����������������������ռ�ձȵļ������㣩
               g_capture_num0 = status.counter;   //�Լ��������ж�ȡ
               g_capture_duty_cycle_num = 0;    //����ռ�ձ����ڵ��������
           }
           else
           {
               time_flag = 0;
               g_capture_num1 = status.counter;

               if(g_capture_num1 >= g_capture_num0)       //�˴���Ҫ�����ж��Ƿ�˴����������������һ�ε��������Ƿ���ͬһ�����ڣ���ֹ����ֵΪ��ֵ��
               {
                   g_capture_num = g_capture_num1 - g_capture_num0;
//                   LOG_I("g_capture1_num is %d\n",g_capture_num);
               }
               else
               {
                   g_capture_num = current_period_counts1 - g_capture_num0 + g_capture_num1;         //�������ͬһ�����ڣ����ҵ�ǰֵ����ֵС����һ���ڵļ���ֵ��������Ҫ���㣨0xffffffff-��һ�μ���ֵ+��ǰ����ֵ��
                   g_capture_frequency = frequency1 / g_capture_num;       //����Ƶ�ʣ�ʱ������ / ��������Ҳ�������趨�����ʱ�������ڵ��ת���˶���Ȧ�� s/hz
//                   LOG_I("g_capture1_num is %d\n",g_capture_num);  //�����������ڵļ���ֵ
//                   LOG_I("g_capture1_frequency is %d\n",g_capture_frequency);   //����ʱ�������ڵ��ת����Ȧ��
               }
           }
       }

    else
       {
           if(time_flag==1)     //  ��ʱ�����ر�־λ�Ѿ���0���1���Ե�ǰ����ֵ���ж�ȡ����������
           {
               g_capture_duty_cycle_num0 = status.counter;
               if(g_capture_duty_cycle_num0 >= g_capture_num0)
               {
                   g_capture_duty_cycle_num = g_capture_duty_cycle_num0 - g_capture_num0;
//                   LOG_I("g_capture1_duty_cycle_num is %d\n",g_capture_duty_cycle_num);
               }
               else
               {
                   g_capture_duty_cycle_num = current_period_counts1 - g_capture_num0 + g_capture_duty_cycle_num0; //�½��صĵ����ڼ���ֵ
                   g_capture_duty_cycle = (g_capture_duty_cycle_num*100 / (float)g_capture_num);//ռ�ձ�
//                   LOG_I("g_capture1_duty_cycle_num is %d\n",g_capture_duty_cycle_num);
//                   LOG_I("g_capture1_duty_cycle is %d\n",g_capture_duty_cycle);
               }
           }
       }

    /* �������������� */
    g_capture_num1_total  += g_capture_num;     //��ÿ�����ڵ������ؼ���ֵ���θ�ֵ���ñ���
    g_capture_duty_cycle_num1_total += g_capture_duty_cycle_num;       //��ÿ�����ڵ��½��ؼ���ֵ���θ�ֵ���ñ���


    if(count1 == 0)
    {
        LOG_I("The first g_capture1_total_num is : %d\n",g_capture_num1_total);
        LOG_I("The first g_capture_duty_cycle_num1_total is : %d\n",g_capture_duty_cycle_num1_total);
        now = time(RT_NULL);
        LOG_I("The first g_capture1_time is : %s\n", ctime(&now));
    }
    else if(count1 > 13)
    {
        count1 = 0;

        LOG_I("The result g_capture1_total_num is : %d\n",g_capture_num1_total / 13);
        LOG_I("The result g_capture_duty_cycle_num1_total is : %d\n",g_capture_duty_cycle_num1_total / 13);
        now = time(RT_NULL);
        LOG_I("The result g_capture1_time is : %s\n", ctime(&now));
    }
    else
    {
        count1++;     //����ÿ����һ�ξ�����һ�Σ���13���������ۼ�
    }
}

/* Callback function */
void exit4_callback(external_irq_callback_args_t *p_args)
{
    time_t now;

    /*��ȡ�˿ڵ�ƽ״̬������ǵ͵�ƽ���������½��أ��ߵ�ƽ����������*/
    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_11, &p_port_value_port_111);        //��ȡ���������ŵ�ƽ״̬

    /* ��ȡ��ʱ������ֵ */
    timer_info_t info;
    (void) R_GPT_InfoGet(&g_timer2_ctrl, &info);

    uint32_t frequency2 = info.clock_frequency;//��ʱ��ʱ��Ƶ��
    uint32_t current_period_counts2 = info.period_counts;//��ʱ������

    /* ��ȡ��ǰ����ֵ */
    timer_status_t status;
    (void) R_GPT_StatusGet(&g_timer2_ctrl, &status);

    if(p_port_value_port_111 == BSP_IO_LEVEL_HIGH)//������
       {
           if(time_flag == 0)
           {
               time_flag = 1;
               g2_capture_num0 = status.counter;
               g2_capture_duty_cycle_num = 0;
           }
           else
           {
               time_flag = 0;
               g2_capture_num1 = status.counter;
               if(g2_capture_num1 >= g2_capture_num0)
               {
                   g2_capture_num = g2_capture_num1 - g2_capture_num0;
//                   LOG_I("g_capture2_num is %d\n",g2_capture_num);
               }
               else
               {
                   g2_capture_num = current_period_counts2 + g2_capture_num1 - g_capture_num0;
                   g2_capture_frequency = frequency2 / g2_capture_num;//���㵥��ʱ��������С��������������
//                   LOG_I("g_capture2_num is %d%\n",g2_capture_num);
//                   LOG_I("g_capture2_frequency is %d\n",g2_capture_frequency);
               }
           }
       }
    else
       {
           if(time_flag==1)
           {
               g2_capture_duty_cycle_num0 = status.counter;
               if(g2_capture_duty_cycle_num0 >= g2_capture_num0)
               {
                   g2_capture_duty_cycle_num = g2_capture_duty_cycle_num0 - g2_capture_num0;
//                   LOG_I("g2_capture2_duty_cycle_num is %d\n",g2_capture_duty_cycle_num);
               }
               else
               {
                   g2_capture_duty_cycle_num = current_period_counts2 - g2_capture_num0 + g_capture_duty_cycle_num0;
                   g2_capture_duty_cycle = (g2_capture_duty_cycle_num*100 / (float)g_capture_num);//ռ�ձ�
//                   LOG_I("g_capture2_duty_cycle_num is %d%\n",g_capture_duty_cycle_num);
//                   LOG_I("g_capture2_duty_cycle is %d\n",g_capture_duty_cycle);
               }
           }
       }

    /* �������������� */
    g_capture_num2_total  += g2_capture_num;     //��ÿ�����ڵ������ؼ���ֵ���θ�ֵ���ñ���
    g_capture_duty_cycle_num2_total += g2_capture_duty_cycle_num;       //��ÿ�����ڵ��½��ؼ���ֵ���θ�ֵ���ñ���


    if(count2 == 0)
    {
        LOG_I("The first g_capture_num2_total is : %d\n",g_capture_num2_total);
        LOG_I("The first g_capture_duty_cycle_num2_total is : %d\n",g_capture_duty_cycle_num2_total);
        now = time(RT_NULL);
        LOG_I("The first g_capture2_time is : %s\n", ctime(&now));
    }
    else if(count1 > 13)
    {
        count2 = 0;

        LOG_I("The result g_capture_num2_total is : %d\n",g_capture_num2_total / 13);
        LOG_I("The result g_capture_duty_cycle_num2_total is : %d\n",g_capture_duty_cycle_num2_total / 13);
        now = time(RT_NULL);
        LOG_I("The result g_capture2_time is : %s\n", ctime(&now));
    }
    else
    {
        count2++;     //����ÿ����һ�ξ�����һ�Σ���13���������ۼ�
    }
}

