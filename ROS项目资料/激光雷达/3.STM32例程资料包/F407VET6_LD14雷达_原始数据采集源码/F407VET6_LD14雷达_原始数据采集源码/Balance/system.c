#include "system.h"
void systemInit(void)
{ 
	//Delay function initialization
	//延时函数初始化
	delay_init(168);
	
	//Interrupt priority group setting
	//中断优先级分组设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	//Initialize the hardware interface connected to the LED lamp
	//初始化与LED灯连接的硬件接口
	LED_Init();                          
	
	delay_ms(500);					//=====延时等待系统稳定
	
	uart_init(115200);			//串口1初始化
	
	uart5_init(115200);			//用于LD14雷达通信
	
}
