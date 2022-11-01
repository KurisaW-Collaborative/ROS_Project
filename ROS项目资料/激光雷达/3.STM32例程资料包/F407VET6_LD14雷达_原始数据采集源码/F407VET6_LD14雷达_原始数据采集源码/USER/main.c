/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：V1.0
修改时间：2022-04-25

Company: WHEELTEC Co.Ltd
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update：2022-4-24

All rights reserved
***********************************************/

/*************************************
使用串口5接收LD14雷达数据，使用串口1输出接收成功的次数
接线说明：
LD14雷达接口      单片机接口
-----------------------------------
   P5V----------------5V	
   GND----------------GND
   TX-----------------D2
   PWM----------------不接不控速)
***************************************/

#include "sys.h"
#include "system.h"

u16 receive_cnt;//计算成功接收数据帧次数


int main(void)
{ 
	systemInit();					//Initialization function     初始化函数
	while(1)
	{ 
		
		printf("成功接收%d次\n",receive_cnt);
		delay_ms(5);
		
	}

}





