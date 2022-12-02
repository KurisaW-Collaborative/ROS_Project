/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-29     Yifang       the first version
 */
#include "pid.h"
#include "rthw.h"

#define speed  800000
float middle = 0.05;

extern float span;

rt_int32_t pwm_l,pwm_r;
float kp = 800000.0;
float ki = 0.00;
float kd = 200.0;

float dia=0;

extern struct rt_device_pwm * pwm1 ;
extern struct rt_device_pwm * pwm2 ;

extern rt_uint32_t ain1_pin,ain2_pin,bin1_pin,bin2_pin;

rt_thread_t pid_thread = RT_NULL;

rt_mutex_t number_protect;

void pid_compute(rt_uint32_t val);

void pid_thread_entry(void *parameter)
{
    while(1)
    {
        float num=0;

        rt_mutex_take(number_protect, RT_WAITING_FOREVER);
        num = span_Calc();
        rt_mutex_release(number_protect);

        pid_compute(num);

        rt_thread_mdelay(5);
    }
}

int pid_init(void)
{
    number_protect = rt_mutex_create("number_protect", RT_IPC_FLAG_FIFO);//对速度差值进行互斥保护

    pid_thread = rt_thread_create("pid_thread", pid_thread_entry, RT_NULL, 2048, 9, 300);
    if(pid_thread)
    {
        rt_thread_startup(pid_thread);
    }
    else {
        rt_kprintf("create pid_thread error\r\n");
        return -RT_ERROR;
    }
    return RT_EOK;
}

void pwm_limit(rt_int32_t * pwm1,rt_int32_t * pwm2)
{
    if(*pwm1>1000000) *pwm1=1000000;
    else if(*pwm1<-1000) *pwm1=-1000000;

    if(*pwm2>1000000) *pwm2=1000000;
    else if(*pwm2<-1000) *pwm2=-1000000;
}

void pwm_abs(rt_int32_t pwm_1,rt_int32_t pwm_2)
{
    if(pwm_1<0)
    {
        rt_pin_write(ain2_pin, PIN_HIGH);
        rt_pin_write(ain1_pin, PIN_LOW);
        //rt_kprintf("l : h\r\n");
        pwm_1 = -pwm_1;
    }
    else if(pwm_1>=0)
    {
        rt_pin_write(ain1_pin, PIN_LOW);
        rt_pin_write(ain2_pin, PIN_HIGH);
        //rt_kprintf("l : f\r\n");
    }
    if(pwm_2<0)
    {
        rt_pin_write(bin2_pin, PIN_HIGH);
        rt_pin_write(bin1_pin, PIN_LOW);
        pwm_2 = -pwm_2;
        //rt_kprintf("r : h\r\n");
    }
    else if(pwm_2>=0)
    {
        rt_pin_write(bin1_pin, PIN_LOW);
        rt_pin_write(bin2_pin, PIN_HIGH);
        //rt_kprintf("r : f\r\n");
    }
    pwm_limit(&pwm_1, &pwm_2);
    //rt_kprintf("%d %d\r\n",pwm_1,pwm_2);
    pwm_set_pulse(pwm1, pwm_1);
    pwm_set_pulse(pwm2, pwm_2);
}


void pid_compute(rt_uint32_t val)
{
    static float error,ierror,derror,errorlast;

    error = middle - val;
    ierror = ierror + error;
    derror = error - errorlast;
    errorlast = error;
    if(ierror > 3000)
        ierror = 3000;
    else if(ierror < -3000)
        ierror = -3000;
    dia = kp*error/100 + ki*ierror + kd*derror/10;
    //dia = dia * 5;

    pwm_l = speed + dia;
    pwm_r = speed - dia;

    pwm_abs(pwm_l, pwm_r);
}

int pid_set(int argc,char **argv)
{
    if(argc<4)
    {
        rt_kprintf("the formate is <kp> <ki> <kd>\r\n");
    }
    else {
        kp = atof(argv[1]);
        ki = atof(argv[2]);
        kd = atof(argv[3]);
        rt_kprintf("SET OK ! KP:%f KI:%f KD:%f",kp,ki,kd);
    }
    return RT_EOK;
}
MSH_CMD_EXPORT(pid_set,pid parameter set);
