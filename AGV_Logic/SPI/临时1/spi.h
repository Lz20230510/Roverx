#ifndef __SPI_H
#define __SPI_H
#include "sys.h"



#define GPIO_SPI2     GPIOB
#define RCC_SPI2      RCC_APB2Periph_GPIOB
#define SPI2_SCK      GPIO_Pin_3
#define SPI2_MISO      GPIO_Pin_4
#define SPI2_MOSI      GPIO_Pin_5



																					  
void SPI1_Init(void);			 //��ʼ��SPI�� 
//void SPIx_SetSpeed(u8 SpeedSet); //����SPI�ٶ�
u8 SPI2_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
		 
#endif

