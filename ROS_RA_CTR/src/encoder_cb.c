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

bsp_io_level_t p_port_value_port_104;   //pin设备句柄
bsp_io_level_t p_port_value_port_111;   //pin设备句柄
timer_info_t info;                      //定时器句柄

static rt_bool_t time_flag = 0;                //上升沿标志位
static uint32_t g_capture_num = 0;               //两个上升沿之间的周期
static uint32_t g_capture_num0 = 0;              //第0个上升沿定时器计数值
static uint32_t g_capture_num1 = 0;              //第1个上升沿定时器计数值
static uint32_t g_capture_frequency = 0;         //频率
static uint32_t g_capture_duty_cycle = 0;        //占空比
static uint32_t g_capture_duty_cycle_num = 0;    //+占空比周期
static uint32_t g_period_num = 0;                //脉冲周期次数
static uint32_t g_capture_duty_cycle_num0 =0;//下降沿定时器计数值

static uint32_t g2_capture_num = 0;               //两个上升沿之间的周期
static uint32_t g2_capture_num0 = 0;              //第0个上升沿定时器计数值
static uint32_t g2_capture_num1 = 0;              //第1个上升沿定时器计数值
static uint32_t g2_capture_frequency = 0;         //频率
static uint32_t g2_capture_duty_cycle = 0;        //占空比
static uint32_t g2_capture_duty_cycle_num = 0;    //+占空比周期
static uint32_t g2_period_num = 0;                //脉冲周期次数
static uint32_t g2_capture_duty_cycle_num0 =0;//下降沿定时器计数值

static int count1 = 0;
static uint32_t g_capture_num1_total = 0;       //整数数组用于存储13个上升沿周期总的计数值
static uint32_t g_capture_duty_cycle_num1_total = 0;     //用于存储13个下降沿周期总计数值

static int count2 = 0;
static uint32_t g_capture_num2_total = 0;       //整数数组用于存储13个上升沿周期总的计数值
static uint32_t g_capture_duty_cycle_num2_total = 0;     //用于存储13个下降沿周期总计数值



/* Callback function */
void exit1_callback(external_irq_callback_args_t *p_args)
{
    time_t now;

    /*读取端口电平状态，如果是低电平则发生的是下降沿，高电平则是上升沿*/
    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_04, &p_port_value_port_104);        //获取编码器引脚电平撞他状态

    /* 获取定时器参数值 */
    timer_info_t info;
    (void) R_GPT_InfoGet(&g_timer3_ctrl, &info);

    uint32_t frequency1 = info.clock_frequency;//定时器时钟频率：50MHz
    uint32_t current_period_counts1 = info.period_counts;//定时器计数值

    /* 获取当前计数值 */
    timer_status_t status;
    (void) R_GPT_StatusGet(&g_timer3_ctrl, &status);

    if(p_port_value_port_104==BSP_IO_LEVEL_HIGH)//上升沿
       {
           if(time_flag == 0)
           {
               time_flag = 1;     //当引脚状态为上升沿时，首先要对计数器进行清零操作（对正占空比的计数清零）
               g_capture_num0 = status.counter;   //对计数器进行读取
               g_capture_duty_cycle_num = 0;    //对正占空比周期的清零操作
           }
           else
           {
               time_flag = 0;
               g_capture_num1 = status.counter;

               if(g_capture_num1 >= g_capture_num0)       //此处需要加入判断是否此处的上升沿相较于上一次的上升沿是否处于同一周期内（防止计数值为负值）
               {
                   g_capture_num = g_capture_num1 - g_capture_num0;
//                   LOG_I("g_capture1_num is %d\n",g_capture_num);
               }
               else
               {
                   g_capture_num = current_period_counts1 - g_capture_num0 + g_capture_num1;         //如果不在同一周期内，并且当前值计数值小于上一周期的计数值，进行需要计算（0xffffffff-上一次计数值+当前计数值）
                   g_capture_frequency = frequency1 / g_capture_num;       //计算频率：时钟周期 / 脉冲数（也就是在设定的这个时钟周期内电机转动了多少圈） s/hz
//                   LOG_I("g_capture1_num is %d\n",g_capture_num);  //单个脉冲周期的计数值
//                   LOG_I("g_capture1_frequency is %d\n",g_capture_frequency);   //单个时钟周期内电机转动的圈数
               }
           }
       }

    else
       {
           if(time_flag==1)     //  此时上升沿标志位已经从0变成1，对当前计数值进行读取并进行运算
           {
               g_capture_duty_cycle_num0 = status.counter;
               if(g_capture_duty_cycle_num0 >= g_capture_num0)
               {
                   g_capture_duty_cycle_num = g_capture_duty_cycle_num0 - g_capture_num0;
//                   LOG_I("g_capture1_duty_cycle_num is %d\n",g_capture_duty_cycle_num);
               }
               else
               {
                   g_capture_duty_cycle_num = current_period_counts1 - g_capture_num0 + g_capture_duty_cycle_num0; //下降沿的单周期计数值
                   g_capture_duty_cycle = (g_capture_duty_cycle_num*100 / (float)g_capture_num);//占空比
//                   LOG_I("g_capture1_duty_cycle_num is %d\n",g_capture_duty_cycle_num);
//                   LOG_I("g_capture1_duty_cycle is %d\n",g_capture_duty_cycle);
               }
           }
       }

    /* 计数器矫正处理 */
    g_capture_num1_total  += g_capture_num;     //将每个周期的上升沿计数值依次赋值给该变量
    g_capture_duty_cycle_num1_total += g_capture_duty_cycle_num;       //将每个周期的下降沿计数值依次赋值给该变量


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
        count1++;     //周期每触发一次就自增一次，到13清零重新累计
    }
}

/* Callback function */
void exit4_callback(external_irq_callback_args_t *p_args)
{
    time_t now;

    /*读取端口电平状态，如果是低电平则发生的是下降沿，高电平则是上升沿*/
    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_11, &p_port_value_port_111);        //获取编码器引脚电平状态

    /* 获取定时器参数值 */
    timer_info_t info;
    (void) R_GPT_InfoGet(&g_timer2_ctrl, &info);

    uint32_t frequency2 = info.clock_frequency;//定时器时钟频率
    uint32_t current_period_counts2 = info.period_counts;//定时器周期

    /* 获取当前计数值 */
    timer_status_t status;
    (void) R_GPT_StatusGet(&g_timer2_ctrl, &status);

    if(p_port_value_port_111 == BSP_IO_LEVEL_HIGH)//上升沿
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
                   g2_capture_frequency = frequency2 / g2_capture_num;//计算单个时钟周期内小车的脉冲周期数
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
                   g2_capture_duty_cycle = (g2_capture_duty_cycle_num*100 / (float)g_capture_num);//占空比
//                   LOG_I("g_capture2_duty_cycle_num is %d%\n",g_capture_duty_cycle_num);
//                   LOG_I("g_capture2_duty_cycle is %d\n",g_capture_duty_cycle);
               }
           }
       }

    /* 计数器矫正处理 */
    g_capture_num2_total  += g2_capture_num;     //将每个周期的上升沿计数值依次赋值给该变量
    g_capture_duty_cycle_num2_total += g2_capture_duty_cycle_num;       //将每个周期的下降沿计数值依次赋值给该变量


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
        count2++;     //周期每触发一次就自增一次，到13清零重新累计
    }
}

