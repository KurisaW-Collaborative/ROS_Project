#ifndef __USRATX_H
#define __USRATX_H 

#include "sys.h"

#define ANGLE_PER_FRAME 12
#define HEADER 0x54
#define POINT_PER_PACK 12
#define LENGTH  0x2C 	//低五为是一帧数据接收到的点数，目前固定是12，高三位预留

typedef struct __attribute__((packed)) Point_Data
{
	u16 distance;//距离
	u8 confidence;//置信度
	
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

