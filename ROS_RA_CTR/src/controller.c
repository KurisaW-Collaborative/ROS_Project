/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-06     Yifang       the first version
 */


#include "controller.h"
#include "stdlib.h"

rt_uint32_t ain1_pin,ain2_pin,bin1_pin,bin2_pin;

extern struct rt_device_pwm * pwm1;
extern struct rt_device_pwm * pwm2;

extern rt_uint32_t pwm1_period;
extern rt_uint32_t pwm2_period;

extern rt_uint32_t pwm1_pulse;
extern rt_uint32_t pwm2_pulse;

//extern rt_uint8_t path_num;

struct Controller controller = {
        .car_status = CAR_STOP,
        .arrive_flag = 0,
        .path_inf = {1,1},
        .left_pulse = PWM_PERCENT_INIT,
        .right_pulse = PWM_PERCENT_INIT,
        .start = car_start,
        .stop = car_stop,
};

int car_init(void)
{
    rt_err_t ret = RT_EOK;

    controller.car_status = CAR_STOP;
    ain1_pin = rt_pin_get(AIN1_PIN);
    ain2_pin = rt_pin_get(AIN2_PIN);
    bin1_pin = rt_pin_get(BIN1_PIN);
    bin2_pin = rt_pin_get(BIN2_PIN);

    rt_pin_mode(ain1_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(ain2_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(bin1_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(bin2_pin, PIN_MODE_OUTPUT);

    rt_pin_write(ain1_pin, PIN_LOW);
    rt_pin_write(ain2_pin, PIN_LOW);
    rt_pin_write(bin1_pin, PIN_LOW);
    rt_pin_write(bin2_pin, PIN_LOW);

    rt_kprintf("car init is successful...\n");

    ret = pwm_init();
    return ret;
}
INIT_APP_EXPORT(car_init);
//MSH_CMD_EXPORT(car_init,car_init);

//extern int turn_flag;
//extern rt_uint8_t a;

rt_err_t car_start(int argc, char **argv)
{
    rt_err_t ret = RT_EOK;
    controller.car_status = CAR_RUN;
    if(argc == 2)
    {
//        path_num = atoi(argv[1]);
//        rt_kprintf("now path is %d\r\n",path_num);
    }
//    turn_flag = 0;
//    a = 0;
    rt_pwm_enable(pwm1, 0);     /* 閸︺劍顒濇径鍕▏閼宠棄鐨潪锔炬畱pwm濞夛拷 */
    rt_pwm_enable(pwm2, 0);

    return ret;
}

rt_err_t car_stop(void)
{
    rt_err_t ret = RT_EOK;
    controller.car_status = CAR_STOP;
    rt_pwm_disable(pwm1, 0);
    rt_pwm_disable(pwm2, 0);
    return ret;
}

/************************************************/
rt_err_t car_forward(void)
{
    rt_err_t ret = RT_EOK;

    rt_pin_write(ain1_pin, PIN_LOW);
    rt_pin_write(ain2_pin, PIN_HIGH);
    rt_pin_write(bin1_pin, PIN_LOW);
    rt_pin_write(bin2_pin, PIN_HIGH);

    pwm_set_pulse(pwm1,pwm1_pulse);
    pwm_set_pulse(pwm2,pwm2_pulse);

    rt_pwm_enable(pwm1, 0);
    rt_pwm_enable(pwm2, 0);
//    pid_init();

    return ret;
}
MSH_CMD_EXPORT(car_forward,car_forward);
//INIT_APP_EXPORT(car_forward);

rt_err_t car_backup(void)
{
    rt_err_t ret = RT_EOK;

    rt_pin_write(ain1_pin, PIN_HIGH);
    rt_pin_write(ain2_pin, PIN_LOW);
    rt_pin_write(bin1_pin, PIN_HIGH);
    rt_pin_write(bin2_pin, PIN_LOW);

    pwm_set_pulse(pwm1,pwm1_pulse);
    pwm_set_pulse(pwm2,pwm2_pulse);

    rt_pwm_enable(pwm1, 0);
    rt_pwm_enable(pwm2, 0);
//    pid_init();

    return ret;
}
MSH_CMD_EXPORT(car_backup,car_backup);

rt_err_t car_TL(void)
{
    rt_err_t ret = RT_EOK;


    rt_pin_write(ain1_pin, PIN_LOW);
    rt_pin_write(ain2_pin, PIN_HIGH);
    rt_pin_write(bin1_pin, PIN_HIGH);
    rt_pin_write(bin2_pin, PIN_LOW);

    rt_pwm_enable(pwm1, 0);
    rt_pwm_enable(pwm2, 0);


    int pulse1_val_L = pwm1_period * 0.9;
    int pulse2_val_R = pwm2_period * 0.9;

    pwm_set_pulse(pwm1,pulse1_val_L);
    pwm_set_pulse(pwm2,pulse2_val_R);

    for(int i = 0;i < 50; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            pwm_set_pulse(pwm1,pulse1_val_L);
            pwm_set_pulse(pwm2,pulse2_val_R);
            pulse1_val_L -= pulse1_val_L / 5000;
            pulse2_val_R -= pulse2_val_R / 5000;
        }
    }

    pwm_disable(pwm1);
    pwm_disable(pwm2);


    return ret;
}
MSH_CMD_EXPORT(car_TL,car_TL);

rt_err_t car_TR(void)
{
    rt_err_t ret = RT_EOK;

    rt_pin_write(ain1_pin, PIN_HIGH);
    rt_pin_write(ain2_pin, PIN_LOW);
    rt_pin_write(bin1_pin, PIN_LOW);
    rt_pin_write(bin2_pin, PIN_HIGH);

    rt_pwm_enable(pwm1, 0);
    rt_pwm_enable(pwm2, 0);

    int pulse1_val_L = pwm1_period * 0.9;
    int pulse2_val_R = pwm2_period * 0.9;

    pwm_set_pulse(pwm1,pulse1_val_L);
    pwm_set_pulse(pwm2,pulse2_val_R);

    for(int i = 0;i < 50; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            pwm_set_pulse(pwm1,pulse1_val_L);
            pwm_set_pulse(pwm2,pulse2_val_R);
            pulse1_val_L -= pulse1_val_L / 5000;
            pulse2_val_R -= pulse2_val_R / 5000;
        }
    }

    pwm_disable(pwm1);
    pwm_disable(pwm2);

    return ret;
}
MSH_CMD_EXPORT(car_TR,car_TR);


void car_control(float rightspeed,float leftspeed)//小车速度转化和控制函数
{
    float k2=17.179;         //速度转换比例,转/分钟
    //将从串口接收到的速度转换成实际控制小车的速度？还是PID？
    int right_speed=(int)k2*rightspeed;
    int left_speed=(int)k2*leftspeed;

    pwm_set_pulse(pwm1, right_speed+10000);
    pwm_set_pulse(pwm2, left_speed+10000);
}
/************************************************/
//
//rt_err_t car_left(void)
//{
//    rt_err_t ret = RT_EOK;
//
//    pwm_set_pulse(pwm1, 350000);
//    pwm_set_pulse(pwm2, PWM_PERIOD);
//
//    rt_kprintf("turn left\r\n");
//
//    for(int i=0; i<1000; i++)
//    {
//        for(int j=0; j<1000; j++)
//        {
//            for(int k=0; k<5; k++)
//            {
//                int h = 0;
//                h++;
//            }
//        }
//    }
//    rt_kprintf("exit left!\r\n");
//    pwm_set_pulse(pwm1, PWM_PERIOD * controller.left_pulse / 100);
//    pwm_set_pulse(pwm2, PWM_PERIOD * controller.right_pulse / 100);
//
//    return ret;
//}
//
//rt_err_t car_right(void)
//{
//    rt_err_t ret = RT_EOK;
//
//    pwm_set_pulse(pwm1, PWM_PERIOD);
//    pwm_set_pulse(pwm2, 350000);
//
//    rt_kprintf("turn left\r\n");
//
//    for(int i=0; i<1000; i++)
//    {
//        for(int j=0; j<1000; j++)
//        {
//            for(int k=0; k<5; k++)
//            {
//                int h = 0;
//                h++;
//            }
//        }
//    }
//    rt_kprintf("exit right!\r\n");
//    pwm_set_pulse(pwm1, PWM_PERIOD * controller.left_pulse / 100);
//    pwm_set_pulse(pwm2, PWM_PERIOD * controller.right_pulse / 100);
//
//    return ret;
//}

//rt_err_t car_turn(int argc, char **argv)
//{
//    rt_err_t ret = RT_EOK;
//    rt_uint32_t times = 0;
//    rt_uint32_t delay = 0;
//
//    times = atoi(argv[1]);
//    delay = atoi(argv[2]);
//    rt_enter_critical();    /* 閺夆晜绋戦崣鍡樼▔鐎靛憡娅曢柛鏍皑缁辨繄鎷崘銊ヮ唺闁革絻鍔嬬粭鍌炴煥娓氬﹦绀夌紒顖濆吹缁儤绂掑鍥уЁ闁告繂绉寸花鍙夌▔椤撶喐鐒� */
//
//    rt_pin_write(ain1_pin, PIN_LOW);
//    rt_pin_write(ain2_pin, PIN_HIGH);
//    rt_pin_write(bin1_pin, PIN_HIGH);
//    rt_pin_write(bin2_pin, PIN_LOW);
//
//    pwm_set_pulse(pwm1,800000);
//    pwm_set_pulse(pwm2,800000);
//
//    rt_pwm_enable(pwm1, 0);
//    rt_pwm_enable(pwm2, 0);
//
//    for(int i=0; i<1000; i++)
//    {
//        for(int j=0; j < times; j++)
//        {
//            for(int k=0; k<6; k++)
//            {
//                int h = 0;
//                h++;
//            }
//        }
//    }
//
//
//    rt_pin_write(ain1_pin, PIN_HIGH);
//    rt_pin_write(ain2_pin, PIN_LOW);
//    rt_pin_write(bin1_pin, PIN_HIGH);
//    rt_pin_write(bin2_pin, PIN_LOW);
//
//    for(int i=0; i<1000; i++)
//    {
//        for(int j=0; j < delay; j++)
//        {
//            for(int k=0; k<4; k++)
//            {
//                int h = 0;
//                h++;
//            }
//        }
//    }
//
////    a = 0;
//    rt_exit_critical();
//
//    return ret;
//}

#if(ENABLE_CAR_MSH)

MSH_CMD_EXPORT(car_start,make car start);
MSH_CMD_EXPORT(car_stop,make car stop);
//MSH_CMD_EXPORT(car_turn,make car turn);
//MSH_CMD_EXPORT(car_left,make car left);
//MSH_CMD_EXPORT(car_right,make car right);

#endif



