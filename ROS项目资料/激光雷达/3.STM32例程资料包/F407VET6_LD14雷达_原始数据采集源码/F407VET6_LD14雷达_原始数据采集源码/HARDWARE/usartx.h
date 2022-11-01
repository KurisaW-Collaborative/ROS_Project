#ifndef __USRATX_H
#define __USRATX_H 

#include "sys.h"

#define ANGLE_PER_FRAME 12
#define HEADER 0x54
#define POINT_PER_PACK 12
#define LENGTH  0x2C 	//����Ϊ��һ֡���ݽ��յ��ĵ�����Ŀǰ�̶���12������λԤ��

typedef struct __attribute__((packed)) Point_Data
{
	u16 distance;//����
	u8 confidence;//���Ŷ�
	
}LidarPointStructDef;

typedef struct __attribute__((packed)) Pack_Data
{
	uint8_t header;
	uint8_t ver_len;
	uint16_t speed;
	uint16_t start_angle;
	LidarPointStructDef point[POINT_PER_PACK];
	uint16_t end_angle;
	uint16_t timestamp;
	uint8_t crc8;
}LiDARFrameTypeDef;


void uart5_init(u32 bound);
int UART5_IRQHandler(void);
void data_process(void);


#endif

