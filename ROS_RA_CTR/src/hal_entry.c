/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-06     Yifang       the first version
 */

#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>
#include <rthw.h>

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

     set_date(2022, 11, 11);
     set_time(12, 00, 00);

     while(1)
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
