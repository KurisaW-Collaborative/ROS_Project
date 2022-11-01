#include "system.h"
void systemInit(void)
{ 
	//Delay function initialization
	//��ʱ������ʼ��
	delay_init(168);
	
	//Interrupt priority group setting
	//�ж����ȼ���������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	//Initialize the hardware interface connected to the LED lamp
	//��ʼ����LED�����ӵ�Ӳ���ӿ�
	LED_Init();                          
	
	delay_ms(500);					//=====��ʱ�ȴ�ϵͳ�ȶ�
	
	uart_init(115200);			//����1��ʼ��
	
	uart5_init(115200);			//����LD14�״�ͨ��
	
}
