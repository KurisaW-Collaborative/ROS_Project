/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-21     Yifang       the first version
 */
#include <rtthread.h>

extern float rotate_speed2;
extern float rotate_speed1;

extern float rad_speed1;       //角速度
extern float line_speed1;      //线速度

extern float rad_speed2;       //角速度
extern float line_speed2;      //线速度

extern float Yaw;

extern rt_device_t ros_serial;

float rad_speed = 0.0;
float line_speed = 0.0;

//union recieveData  //接收到的数据
//{
//    float d;    //左右轮速度
//    unsigned char data[4];
//}leftdata,rightdata;       //接收的左右轮数据

union odometry  //里程计数据共用体
{
    float odoemtry_float;
    unsigned char odometry_char[4];
}x_data,y_data,theta_data,vel_linear,vel_angular;     //要发布的里程计数据，分别为：X，Y方向移动的距离，当前角度，线速度，角速度

char odometry_data[21]={0};   //发送给串口的里程计数据数组

void SendData_thread_entry(void)
{
    //里程计数据获取
    x_data.odoemtry_float = rotate_speed1;//单位mm
    y_data.odoemtry_float = rotate_speed2;//单位mm
    theta_data.odoemtry_float = Yaw;//单位rad
    vel_linear.odoemtry_float = line_speed;//单位mm/s
    vel_angular.odoemtry_float = rad_speed;//单位rad/s

    //将所有里程计数据存到要发送的数组
    for(int j=0;j<4;j++)
    {
        odometry_data[j]=x_data.odometry_char[j];
        odometry_data[j+4]=y_data.odometry_char[j];
        odometry_data[j+8]=theta_data.odometry_char[j];
        odometry_data[j+12]=vel_linear.odometry_char[j];
        odometry_data[j+16]=vel_angular.odometry_char[j];
    }

    odometry_data[20]='\n';//添加结束符

    //发送数据到串口
    for(int i=0;i<21;i++)
    {
        rt_device_write(ros_serial, 0, odometry_data[i], 1);
        rt_thread_mdelay(1);
    }
}

void odometry_thread(void)
{
    /* 角速度、线速度初始化取均值 */
    rad_speed = (rad_speed1 + rad_speed2) / 2;
    line_speed = (rad_speed1 + rad_speed2) / 2;

    rt_thread_t OdSendThread = rt_thread_create("od_send_th", SendData_thread_entry, RT_NULL, 2048, 18, 500);
    if(OdSendThread != RT_NULL)
    {
        rt_thread_startup(OdSendThread);
    }

}


