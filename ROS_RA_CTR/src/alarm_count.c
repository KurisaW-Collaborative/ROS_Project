/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-08     Yifang       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <time.h>
static int set_datetime(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    time_t now;

     if(argc != 6) {
         rt_kprintf("Usage:set_datetime 2022 03 28 21 57 00 \r\n");
     }
    /* 设置日期 */
    ret = set_date(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC date failed\n");
        return ret;
    }

    /* 设置时间 */
    ret = set_time(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC time failed\n");
        return ret;
    }


    /* 获取时间 */
    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(set_datetime, rtc sample);
