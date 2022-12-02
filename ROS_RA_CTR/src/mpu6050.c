/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-31     Yifang       the first version
 */
#include <rtdevice.h>

#include <rtthread.h>
#include <rthw.h>

#include "mpu6050.h"
#include <math.h>

#define MPU6XXX_DEVICE_NAME  "i2c0"

struct mpu6xxx_device *dev;

#define Kp 17.5f // ��������֧�������������ٶȼ�/��ǿ��
#define Ki 0.008f // ��������֧���ʵ�������ƫ�����ν�
#define halfT 0.000f // �������ڵ�һ��

float q0 = 1, q1 = 0, q2 = 0, q3 = 0; // ��Ԫ����Ԫ�أ�������Ʒ���
float exInt = 0, eyInt = 0, ezInt = 0; // ��������С�������
float Yaw,Pitch,Roll; //ƫ���ǣ������ǣ�������

rt_int16_t vx, vy, vz;
rt_int16_t ex, ey, ez;

int i = 0;//����ֵ

void mpu6050_RD_thread_entry(void *parameter)
{
    while(1)
    {
        mpu6xxx_get_accel(dev, &accel);
        mpu6xxx_get_gyro(dev, &gyro);

        ex = accel.x;
        ey = accel.y;
        ez = accel.z;

        vx = gyro.x;
        vy = gyro.y;
        vz = gyro.z;

        IMUupdate(vx,vy,vz,ex,ey,ez);

//        rt_kprintf("\n--------------The %d calculation--------------\n",i);
//        rt_kprintf("| accel.x = %4d | accel.y = %4d | accel.z = %4d |\n", (int)accel.x, (int)accel.y, (int)accel.z);
//        rt_kprintf("| gyro.x = %4d  | gyro.y = %4d  | gyro.z = %4d  |\n", (int)gyro.x, (int)gyro.y, (int)gyro.z);
//        rt_kprintf("------------------------------------------------\n\n");

//        rt_thread_mdelay(1000);
    }
    mpu6xxx_deinit(dev);
}

void mpu6050_RD()
{
    rt_thread_t ret = 0;

    dev = mpu6xxx_init(MPU6XXX_DEVICE_NAME, RT_NULL);

    if (dev == RT_NULL)
    {
        rt_kprintf("mpu6xxx init failed\n");
        return -1;
    }
    rt_kprintf("mpu6xxx init succeed\n");

    ret = rt_thread_create("mpu_thread", mpu6050_RD_thread_entry, RT_NULL, 1024, 19, 500);
    if(ret == RT_NULL)
    {
        rt_kprintf("mpu_thread create failed.\n");
    }

    rt_thread_startup(ret);
}
MSH_CMD_EXPORT(mpu6050_RD,mpu6050_RD);



//mpu6050��̬�ں��㷨(��Ԫ����)
void IMUupdate(rt_int16_t gx, rt_int16_t gy, rt_int16_t gz, rt_int16_t ax, rt_int16_t ay, rt_int16_t az)
{
    float norm;

    // ����������
    norm = sqrt(ax*ax + ay*ay + az*az);

    ax = ax / norm; //��λ��
    ay = ay / norm;
    az = az / norm;

    // ���Ʒ��������
    vx = 2*(q1*q3 - q0*q2);
    vy = 2*(q0*q1 + q2*q3);
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    // ���������ͷ��򴫸��������ο�����֮��Ľ���˻����ܺ�
    ex = (ay*vz - az*vy);
    ey = (az*vx - ax*vz);
    ez = (ax*vy - ay*vx);

    // ������������������
    exInt = exInt + ex*Ki;
    eyInt = eyInt + ey*Ki;
    ezInt = ezInt + ez*Ki;

    // ������������ǲ���
    gx = gx + Kp*ex + exInt;
    gy = gy + Kp*ey + eyInt;
    gz = gz + Kp*ez + ezInt;

    // ������Ԫ���ʺ�������
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

    // ��������Ԫ
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch ,ת��Ϊ����

    Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // rollv

//    Yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3; //�˴�û�м�ֵ��ע��
    Yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3;

//    rt_kprintf("\n--------------The %d calculation-----------------------\n",i);
//    rt_kprintf("| Pitch = %.4f | Roll = %.4f | Yaw = %.4f |\n", Pitch, Roll, Yaw);
//    rt_kprintf("-------------------------------------------------------\n\n");
//    i++;
}


////�������˲������뺯��
//float dt=0.001;//ע�⣺dt��ȡֵΪkalman�˲�������ʱ��
//float angle, angle_dot;//�ǶȺͽ��ٶ�
//float P[2][2] = {{ 1, 0 },
//                 { 0, 1 }};
//float Pdot[4] ={ 0,0,0,0};
//float Q_angle=0.001, Q_gyro=0.005; //�Ƕ��������Ŷ�,���ٶ��������Ŷ�
//float R_angle=0.5 ,C_0 = 1;
//float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
//
////�������˲�
//float Kalman_Filter(float angle_m, float gyro_m)//angleAx �� gyroGy
//{
//        angle+=(gyro_m-q_bias) * dt;
//        angle_err = angle_m - angle;
//        Pdot[0]=Q_angle - P[0][1] - P[1][0];
//        Pdot[1]=- P[1][1];
//        Pdot[2]=- P[1][1];
//        Pdot[3]=Q_gyro;
//        P[0][0] += Pdot[0] * dt;
//        P[0][1] += Pdot[1] * dt;
//        P[1][0] += Pdot[2] * dt;
//        P[1][1] += Pdot[3] * dt;
//        PCt_0 = C_0 * P[0][0];
//        PCt_1 = C_0 * P[1][0];
//        E = R_angle + C_0 * PCt_0;
//        K_0 = PCt_0 / E;
//        K_1 = PCt_1 / E;
//        t_0 = PCt_0;
//        t_1 = C_0 * P[0][1];
//        P[0][0] -= K_0 * t_0;
//        P[0][1] -= K_0 * t_1;
//        P[1][0] -= K_1 * t_0;
//        P[1][1] -= K_1 * t_1;
//        angle += K_0 * angle_err; //���ŽǶ�
//        q_bias += K_1 * angle_err;
//        angle_dot = gyro_m-q_bias;//���Ž��ٶ�
//
//        return angle;
//}
