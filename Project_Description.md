引脚中断：

捕获外部引脚脉冲值（双边触发，当有上升沿向下降沿转变或下降沿向上升沿转变时都会自动触发一次引脚中断，并通知定时器开始计时）



获取引脚->绑定中断回调函数->使能引脚中断



引脚中断回调函数

先获取引脚的电平状态，判断是高电平还是低电平,当第一次检测到低电平向高电平跳变时，将计数器清零操作（此处需要注意在需要对第一个上升沿的计数值与上一次的计数值进行比较，如果当前计数值大于上一次计数值）



定时器读取脉冲：



```
读取编码器的值，正负代表旋转方向

转速（1秒转多少圈） = 单位时间内的计数值 / 总分辨率 * 时间系数

由于A、B信号正交，因此可以根据两个信号哪个先哪个后来判断方向，根据每个信号脉冲数量的多少及每圈电机产生多少脉冲数就可以算出当前行走的距离，如果再加上定时器的话还可以计算出速度。

编码器脉冲数（一圈）= 20*13*2=520		说明---20：减速比为1-20 | 13：编码器线数为13ppr | 2：上升沿和下降沿都会触发计数值（只使用A相），所以理论上编码器转动一圈会有520个脉冲
```

![image-20221112145050334](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202211121450740.png)

![image-20221112211021693](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202211122110083.png)


---


#### 电机篇

蓝：电源线
白：编码器A相
黄：编码器B相
黑：地线

AIN1_PIN： "P403"
AIN2_PIN ："P404"
BIN1_PIN ："P405"
BIN2_PIN ："P406"

TIM7：电机左占空比设置（PWM）（GTIOC7A:P304）
TIM8：电机右占空比设置（PWM）（GTIOC8A:P107）

IRQ1：编码器1周期读取（Periodic）（GTIOC5A:P104）(A相)
IRQ4：编码器2周期读取（Periodic）（GTIOC6A:P111）(A相)

TIM3：定时器1计时（编码器1）（Periodic）
TIM2：定时器2计时（编码器2）（Periodic）

#### MPU6050

I2C0：MPU6050
slave Address：0x68
SDA0：P401
SCL0：P400

#### UART（串口通信）

uart3：树莓派通信端口
TXD3：P310
RXD3：P309

#### 术语

频率：计数器得到的脉冲个数除以定时时间

脉冲 = 周期

占空比：高电平持续有效时间 / 周期

1GHz=1000MHz，1**MHz**=1000kHz，1kHz=1000Hz 1**s**=1000ms，1 ms=1000μs，1μs=1000ns

---

学习参考手册：[[野火]瑞萨RA系列FSP库开发实战指南--基于野火启明6M5开发板](https://doc.embedfire.com/mcu/renesas/fsp_ra/zh/latest/doc/chapter27/chapter27.html)



#### GPIO

GPIO获取计数值（取平均值）

清零

#### 1 ms real time int

计算速度

PID

PWN enable



![image-20221107211153752](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202211072111252.png)

![image-20221108171131312](https://raw.githubusercontent.com/kurisaW/picbed/main/img/202211081711661.png)



测试数据：

|      | 占空比 | 一圈所花费时间 |  速度  |
| :--: | :----: | :------------: | :----: |
|  前  |  50%   |     0.2s/r     | 86cm/r |
|  后  |  50%   |     0.3s/r     | 49cm/r |
|  左  |  30%   |     0.7s/r     | 22cm/r |
|  右  |  70%   |    0.15s/r     | 90cm/r |

