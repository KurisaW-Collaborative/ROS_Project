/*
#include <src/pid_ct_ICMTL.h>
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
#include <stdio.h>

#define DBG_LVL               DBG_LOG
#include <rtdbg.h>


float span = 0;//左右轮速度差值

bsp_io_level_t p_port_value_port_104;   //pin设备句柄
bsp_io_level_t p_port_value_port_111;   //pin设备句柄
timer_info_t info;                      //定时器句柄

static rt_bool_t time_flag = 0;                //上升沿标志位
uint32_t g_capture_num = 0;               //两个上升沿之间的周期
static uint32_t g_capture_num0 = 0;              //第0个上升沿定时器计数值
static uint32_t g_capture_num1 = 0;              //第1个上升沿定时器计数值
uint32_t g_capture_frequency = 0;         //频率
uint32_t g_capture_duty_cycle = 0;        //占空比1
uint32_t g_capture_duty_cycle_num = 0;    //+占空比周期
static uint32_t g_capture_duty_cycle_num0 =0;//下降沿定时器计数值

uint32_t g2_capture_num = 0;               //两个上升沿之间的周期
static uint32_t g2_capture_num0 = 0;              //第0个上升沿定时器计数值
static uint32_t g2_capture_num1 = 0;              //第1个上升沿定时器计数值
uint32_t g2_capture_frequency = 0;         //频率
uint32_t g2_capture_duty_cycle = 0;        //占空比2
uint32_t g2_capture_duty_cycle_num = 0;    //+占空比周期
static uint32_t g2_capture_duty_cycle_num0 =0;//下降沿定时器计数值

int count1 = 0;
uint32_t g_capture_num1_total = 0;       //用于存储一次引脚触发计数总值
float capture_num1_total = 0;
float speed_average1_time = 0;
float rotate_speed1 = 0;
float rad_speed1 = 0;       //角速度
float line_speed1 = 0;      //线速度

int count2 = 0;
uint32_t g_capture_num2_total = 0;       //用于存储一次引脚触发计数总值
float capture_num2_total = 0;
float speed_average2_time = 0;
float rotate_speed2 = 0;
float rad_speed2 = 0;
float line_speed2 = 0;      //线速度

time_t now;

/* Callback function */
void exit1_callback(external_irq_callback_args_t *p_args)
{
//    time_t now;

    /*读取端口电平状态，如果是低电平则发生的是下降沿，高电平则是上升沿*/
    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_04, &p_port_value_port_104);        //获取编码器引脚电平状态

    /* 定时器3使能，用于捕获中断引脚计数 */
    (void) R_GPT_Enable(&g_timer3_ctrl);

    /* 获取定时器参数值 */
    timer_info_t info;
    (void) R_GPT_InfoGet(&g_timer3_ctrl, &info);

    uint32_t frequency1 = info.clock_frequency;//定时器时钟频率：取10MHZ
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

               /* 计算电机转动1圈所用总计数值 */
               g_capture_num1_total = g_capture_num; //统计每一次的引脚触发总计数值

               if(count1 < (13 * 50)) //计算100个脉冲周期
               {
                   count1++;
                   capture_num1_total += g_capture_num1_total;
               }
               else
               {
                   capture_num1_total = capture_num1_total / (13 * 50);

                   /* 计算小车转速 */
                   speed_average1_time = capture_num1_total / frequency1;//转动一圈电机所花费的时间
                   rotate_speed1 =  1 / speed_average1_time / 20;   //转速：r/s一秒多少圈
                   rad_speed1 = rotate_speed1 * 6.2832; //角速度：转速*Π
                   line_speed1 = rad_speed1 * 2.5;      //线速度：角速度*半径

                   count1 = 0;
                   capture_num1_total = 0;
               }
           }
           else
           {
               time_flag = 0;
               g_capture_num1 = status.counter;

               if(g_capture_num1 >= g_capture_num0)       //此处需要加入判断是否此处的上升沿相较于上一次的上升沿是否处于同一周期内（防止计数值为负值）
               {
                   g_capture_num = g_capture_num1 - g_capture_num0;
               }
               else
               {
                   g_capture_num = current_period_counts1 - g_capture_num0 + g_capture_num1;         //如果不在同一周期内，并且当前值计数值小于上一周期的计数值，进行需要计算（0xffffffff-上一次计数值+当前计数值）(对总的一个周期进行运算)
               }

               g_capture_frequency = frequency1 / g_capture_num;       //计算引脚触发频率在时钟频率
           }
       }

    else
       {
//         此处对B相进行脉冲统计，统计出单个周期的脉冲值
           if(time_flag==1)     //  此时上升沿标志位已经从0变成1，对当前计数值进行读取并进行运算
           {
               g_capture_duty_cycle_num0 = status.counter;
               if(g_capture_duty_cycle_num0 >= g_capture_num0)
               {
                   g_capture_duty_cycle_num = g_capture_duty_cycle_num0 - g_capture_num0;
               }
               else
               {
                   g_capture_duty_cycle_num = current_period_counts1 - g_capture_num0 + g_capture_duty_cycle_num0; //下降沿的单周期计数值
               }
               g_capture_duty_cycle = (g_capture_duty_cycle_num * 100 / (float)g_capture_num);//低电平触发频率
           }
       }
}

/* Callback function */
void exit4_callback(external_irq_callback_args_t *p_args)
{
//    time_t now;

    /*读取端口电平状态，如果是低电平则发生的是下降沿，高电平则是上升沿*/
    R_IOPORT_PinRead(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_11, &p_port_value_port_111);        //获取编码器引脚电平状态

    /* 启动定时器2 */
    (void) R_GPT_Enable(&g_timer2_ctrl);

    /* 获取定时器参数值 */
    timer_info_t info;
    (void) R_GPT_InfoGet(&g_timer2_ctrl, &info);

    uint32_t frequency2 = info.clock_frequency;//定时器时钟频率:取10MHZ
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

               /* 计算电机转动半圈所用总计数值 */
               g_capture_num2_total = g2_capture_num ; //统计每一次的引脚触发总计数值

               if(count2 < (13 * 50))
               {
                   count2++;
                   capture_num2_total += g_capture_num2_total;
               }
               else
               {
                   capture_num2_total = capture_num2_total / (13 * 50);

                   /* 计算小车转速 */
                   speed_average2_time = capture_num2_total / frequency2;//转动一圈电机所花费的时间
                   rotate_speed2 =  1 / speed_average2_time / 20;   //转速：r/s一秒多少圈
                   rad_speed2 = rotate_speed2 * 6.2832; //角速度：转速*Π
                   line_speed2 = rad_speed2 * 2.5;      //线速度：角速度*半径

                   count2 = 0;
                   capture_num2_total = 0;
               }
           }
           else
           {
               time_flag = 0;
               g2_capture_num1 = status.counter;
               if(g2_capture_num1 >= g2_capture_num0)
               {
                   g2_capture_num = g2_capture_num1 - g2_capture_num0;
               }
               else
               {
                   g2_capture_num = current_period_counts2 + g2_capture_num1 - g2_capture_num0;
               }

               g2_capture_frequency = frequency2 / g2_capture_num;//计算单个计数周期内小车的高电平触发频率
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
               }
               else
               {
                   g2_capture_duty_cycle_num = current_period_counts2 - g2_capture_num0 + g2_capture_duty_cycle_num0;
               }

               g2_capture_duty_cycle = (g2_capture_duty_cycle_num*100 / (float)g2_capture_num);//占空比
           }
       }
}

/* 反馈左右轮速度差值 */
float span_Calc(void)
{
    if(rotate_speed2 > rotate_speed1)
    {
        span = rotate_speed2 - rotate_speed1;
    }
    else
    {
        span = rotate_speed1 - rotate_speed2;
    }
    return span;
}

