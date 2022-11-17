/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-05     Yifang       the first version
 */
#include "pid_config.h"
#include <finsh.h>

#define speed 800000    // pid的基准值，80%
float middle = 162.0;
extern rt_uint32_t number;

rt_uint32_t pwm_l,pwm_r;

float kp = 800000;
float ki = 0.0;
float kd = 200;

float dia = 0;  //pid的增量值

extern struct rt_device *pwm1;
extern struct rt_device *pwm2;

extern rt_uint32_t ain1_pin,ain2_pin,bin1_pin,bin2_pin;

rt_thread_t pid_thread = RT_NULL;

extern rt_mutex_t number_protect;

void pid_compute(rt_uint32_t val);

void pid_thread_entry(void *parameter)
{
    while(1)
    {
        rt_uint32_t num = 0;
        num = 0;

        rt_mutex_take(number_protect, RT_WAITING_FOREVER);
        num = number;       //对全局变量进行互斥保护
        rt_mutex_release(number_protect);

        pid_compute(num);       //对这个变量进行pid运算

        rt_thread_mdelay(30);
    }
}


int pid_init(void)
{
    pid_thread = rt_thread_create("pid_thread", pid_thread_entry, RT_NULL, 2048, 9, 300);
    if(pid_thread)
    {
        rt_thread_startup(pid_thread);
    }
    else
    {
        rt_kprintf("pid_thread create failed!\n");
        return -RT_EOK;
    }
}

void pid_compute(rt_uint32_t val)
{
    static float error,ierror,derror,errorlast;

    /*
     * middle:传过来的中值
     * val:pid_thread_entry中的num值
     */

    error = middle - val;
    ierror = ierror + error;
    derror = error - errorlast;
    errorlast = error;

    if(ierror > 3000)
    {
        ierror = 3000;
    }
    else if(ierror < -3000)
    {
        ierror = -3000;
        dia = kp * error / 100 + ki * ierror + kd * derror / 10;    // 增量值计算

        pwm_r = speed - dia;
        pwm_r = speed + dia;

        pwm_abs(pwm1, pwm_r);
    }
}

void pwm_limit(rt_int32_t *pwm1, rt_int32_t *pwm2)
{
    if(*pwm1 > 1000000)
        *pwm1 = 1000000;
    else if(*pwm1 < -1000)
        *pwm1 = -1000000;

    if(*pwm2 > 1000000)
        *pwm2 = 1000000;
    else if(*pwm2 <= -1000)
        *pwm2 = -1000000;
}

void pwm_abs(rt_int32_t pwm_1, rt_uint32_t pwm_2)   //根据pid_compute()获得的基准值来控制小车的前进和后退方向
{
    if(pwm_1 < 0)
    {
        rt_pin_write(ain2_pin, PIN_HIGH);
        rt_pin_write(ain1_pin, PIN_LOW);

        pwm_1 = -pwm_1;
    }
    else if(pwm_1 >= 0)
    {
        rt_pin_write(ain1_pin, PIN_HIGH);
        rt_pin_write(ain2_pin, PIN_LOW);
    }
    if(pwm_2 < 0)
    {
        rt_pin_write(bin2_pin, PIN_HIGH);
        rt_pin_write(bin1_pin, PIN_LOW);
    }
    else if(pwm_2 >= 0)
    {
        rt_pin_write(bin1_pin, PIN_HIGH);
        rt_pin_write(bin2_pin, PIN_LOW);
    }

    pwm_limit(&pwm_1, &pwm_2);  //进入线幅函数
    pwm_set_pulse(pwm1, pwm_1);
    pwm_set_pulse(pwm2, pwm_2);
}

int pid_set(int argc, char *argv[])
{
    if(argc < 4)
    {
        rt_kprintf("the format is <kp> <ki> <kd>\r\n");
    }
    else
    {
        kp = atof(argv[1]);
        ki = atof(argv[2]);
        kd = atof(argv[3]);
        rt_kprintf("SET OK! KP:%f KI:%f KD:%f",kp,ki,kd);
    }
    return RT_EOK;
}
MSH_CMD_EXPORT(pid_set,pid parameter set);


