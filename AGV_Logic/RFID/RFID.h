#ifndef __RFID_H_
#define __RFID_H_

#include "sys.h"



extern u32 RFID_value;

typedef struct
{
	u8 error;							//当=0：数据错误   =1：数据正确
	u8 crc16High;					//奇偶校验高八位
	u8 crc16Low;					//奇偶校验低八位
}CRC16Def;

extern CRC16Def CRC16RFID,CRC16Wireless,CRC16Mag;



u8 RFID_exchange(u8 date_in);														//RFID接收数据一个字节的高低4位调换
u32 RFID_format(u8 * RFID_data);												//
CRC16Def RFIDCRC16_check(u8 * RFID_data,u8 len);				//奇偶校验
CRC16Def WireLessCRC16_check(u8 * RFID_data,u8 len);		//奇偶校验
u32 RFID_receive(u8 * RFID_data);												//
#endif
