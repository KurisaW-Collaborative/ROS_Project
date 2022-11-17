/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-10     Sherman       first version
 * 2021-11-03     Sherman       Add icu_sample
 */

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>

#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#include "src/encoder_cb.h"
#include "controller.h"

#define LED3_PIN    BSP_IO_PORT_01_PIN_06
#define USER_INPUT  "P105"

void hal_entry(void)
{
    rt_kprintf("\nHello RT-Thread!\n");

    /* Initialize the IOPORT module and configure the pins
     * Note: The default pin configuration name in the RA Configuraton tool is g_bsp_pin_cfg */
    fsp_err_t err = FSP_SUCCESS;
    err = R_IOPORT_Open(&g_ioport_ctrl, &g_bsp_pin_cfg);
    /* Handle any errors. This function should be defined by the user. */
    assert(FSP_SUCCESS == err);

    /* Initializes the module. */
     err = R_GPT_Open(&g_timer3_ctrl, &g_timer3_cfg);
     /* Handle any errors. This function should be defined by the user. */
     assert(FSP_SUCCESS == err);

     err = FSP_SUCCESS;
     err = R_GPT_Open(&g_timer2_ctrl, &g_timer2_cfg);
     /* Handle any errors. This function should be defined by the user. */
     assert(FSP_SUCCESS == err);


     /* Configure the external interrupt. */
     err = R_ICU_ExternalIrqOpen(&g_external_irq1_ctrl, &g_external_irq1_cfg);
     assert(FSP_SUCCESS == err);

     err = FSP_SUCCESS;
     err = R_ICU_ExternalIrqOpen(&g_external_irq4_ctrl, &g_external_irq4_cfg);
     assert(FSP_SUCCESS == err);

     /* Start the timer. */
     (void) R_GPT_Start(&g_timer3_ctrl);
     (void) R_GPT_Start(&g_timer2_ctrl);


     /* 使能引脚中断 */
     err =  R_ICU_ExternalIrqEnable(&g_external_irq1_ctrl);
     assert(FSP_SUCCESS == err);

     err = FSP_SUCCESS;
     err =  R_ICU_ExternalIrqEnable(&g_external_irq4_ctrl);
     assert(FSP_SUCCESS == err);

     set_date(2022, 11, 10);
     set_time(20, 17, 20);


    while (1)
    {
        rt_pin_write(LED3_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED3_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}

void irq_callback_test(void *args)
{
    rt_kprintf("\n IRQ00 triggered \n");
}

void icu_sample(void)
{
    /* init */
    rt_uint32_t pin = rt_pin_get(USER_INPUT);
    rt_kprintf("\n pin number : 0x%04X \n", pin);
    rt_err_t err = rt_pin_attach_irq(pin, PIN_IRQ_MODE_RISING, irq_callback_test, RT_NULL);
    if(RT_EOK != err)
    {
        rt_kprintf("\n attach irq failed. \n");
    }
    err = rt_pin_irq_enable(pin, PIN_IRQ_ENABLE);
    if(RT_EOK != err)
    {
        rt_kprintf("\n enable irq failed. \n");
    }
}
MSH_CMD_EXPORT(icu_sample, icu sample);

//void RTC_Init(void)
//{
//    /**********日期宏定义**********/
//    #define RTC_YEAR_SET 2022       //年
//    #define RTC_MON_SET 8           //月
//    #define RTC_MDAY_SET 3          //日
//    /*通过蔡勒公式计算星期*/
//    #define RTC_WDAY_SET (RTC_YEAR_SET-2000 \
//                      + ((RTC_YEAR_SET-2000)/4) \
//                      - 35 + (26*(RTC_MON_SET+1))/10 \
//                      + RTC_MDAY_SET -1 )%7
//
//    /**********时间宏定义**********/
//    #define RTC_HOUR_SET 0          //时
//    #define RTC_SEC_SET 0           //秒
//    #define RTC_MIN_SET 0           //分
//
//   //初始化时设定的时间
//   rtc_time_t set_time =
//   { .tm_sec = RTC_SEC_SET,  //秒
//     .tm_min = RTC_MIN_SET,  //分
//     .tm_hour = RTC_HOUR_SET,  //小时
//     .tm_mday = RTC_MDAY_SET,  //日（一个月中）
//     .tm_wday = RTC_WDAY_SET,   //星期
//     .tm_mon = RTC_MON_SET,   //月份
//     .tm_year = RTC_YEAR_SET-1900, //年份（如今年是2022，则这里输入2022-1900=122）
//   };
//   /*打开RTC模块*/
//   R_RTC_Open (g_rtc0.p_ctrl, g_rtc0.p_cfg);
//   /*时钟源设置，如果在FSP Configuration设置"Set Source Clock in Open"为"enabled"，那这一步可以被跳过*/
//   R_RTC_ClockSourceSet (g_rtc0.p_ctrl);
//   /*若RTC时钟已经使用纽扣电池工作了一段时间，则可以使用这个函数获取当前日历并设置当前时间*/
//   //R_RTC_CalendarTimeGet(RTC.p_ctrl,&set_time);
//   /*这个函数至少调用一次以启动RTC*/
//   R_RTC_CalendarTimeSet (g_rtc0.p_ctrl, &set_time); //设置当前时间
//   /*设置周期中断的周期为1秒*/
//   R_RTC_PeriodicIrqRateSet (g_rtc0.p_ctrl, RTC_PERIODIC_IRQ_SELECT_1_SECOND);
//}

