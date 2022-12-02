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

extern float rad_speed1;       //���ٶ�
extern float line_speed1;      //���ٶ�

extern float rad_speed2;       //���ٶ�
extern float line_speed2;      //���ٶ�

extern float Yaw;

extern rt_device_t ros_serial;

float rad_speed = 0.0;
float line_speed = 0.0;

//union recieveData  //���յ�������
//{
//    float d;    //�������ٶ�
//    unsigned char data[4];
//}leftdata,rightdata;       //���յ�����������

union odometry  //��̼����ݹ�����
{
    float odoemtry_float;
    unsigned char odometry_char[4];
}x_data,y_data,theta_data,vel_linear,vel_angular;     //Ҫ��������̼����ݣ��ֱ�Ϊ��X��Y�����ƶ��ľ��룬��ǰ�Ƕȣ����ٶȣ����ٶ�

char odometry_data[21]={0};   //���͸����ڵ���̼���������

void SendData_thread_entry(void)
{
    //��̼����ݻ�ȡ
    x_data.odoemtry_float = rotate_speed1;//��λmm
    y_data.odoemtry_float = rotate_speed2;//��λmm
    theta_data.odoemtry_float = Yaw;//��λrad
    vel_linear.odoemtry_float = line_speed;//��λmm/s
    vel_angular.odoemtry_float = rad_speed;//��λrad/s

    //��������̼����ݴ浽Ҫ���͵�����
    for(int j=0;j<4;j++)
    {
        odometry_data[j]=x_data.odometry_char[j];
        odometry_data[j+4]=y_data.odometry_char[j];
        odometry_data[j+8]=theta_data.odometry_char[j];
        odometry_data[j+12]=vel_linear.odometry_char[j];
        odometry_data[j+16]=vel_angular.odometry_char[j];
    }

    odometry_data[20]='\n';//��ӽ�����

    //�������ݵ�����
    for(int i=0;i<21;i++)
    {
        rt_device_write(ros_serial, 0, odometry_data[i], 1);
        rt_thread_mdelay(1);
    }
}

void odometry_thread(void)
{
    /* ���ٶȡ����ٶȳ�ʼ��ȡ��ֵ */
    rad_speed = (rad_speed1 + rad_speed2) / 2;
    line_speed = (rad_speed1 + rad_speed2) / 2;

    rt_thread_t OdSendThread = rt_thread_create("od_send_th", SendData_thread_entry, RT_NULL, 2048, 18, 500);
    if(OdSendThread != RT_NULL)
    {
        rt_thread_startup(OdSendThread);
    }

}


