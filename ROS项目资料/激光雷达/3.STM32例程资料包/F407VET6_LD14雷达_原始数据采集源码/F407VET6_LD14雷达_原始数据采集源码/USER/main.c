/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��V1.0
�޸�ʱ�䣺2022-04-25

Company: WHEELTEC Co.Ltd
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update��2022-4-24

All rights reserved
***********************************************/

/*************************************
ʹ�ô���5����LD14�״����ݣ�ʹ�ô���1������ճɹ��Ĵ���
����˵����
LD14�״�ӿ�      ��Ƭ���ӿ�
-----------------------------------
   P5V----------------5V	
   GND----------------GND
   TX-----------------D2
   PWM----------------���Ӳ�����)
***************************************/

#include "sys.h"
#include "system.h"

u16 receive_cnt;//����ɹ���������֡����


int main(void)
{ 
	systemInit();					//Initialization function     ��ʼ������
	while(1)
	{ 
		
		printf("�ɹ�����%d��\n",receive_cnt);
		delay_ms(5);
		
	}

}





