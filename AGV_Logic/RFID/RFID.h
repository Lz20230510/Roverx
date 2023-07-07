#ifndef __RFID_H_
#define __RFID_H_

#include "sys.h"



extern u32 RFID_value;

typedef struct
{
	u8 error;							//��=0�����ݴ���   =1��������ȷ
	u8 crc16High;					//��żУ��߰�λ
	u8 crc16Low;					//��żУ��Ͱ�λ
}CRC16Def;

extern CRC16Def CRC16RFID,CRC16Wireless,CRC16Mag;



u8 RFID_exchange(u8 date_in);														//RFID��������һ���ֽڵĸߵ�4λ����
u32 RFID_format(u8 * RFID_data);												//
CRC16Def RFIDCRC16_check(u8 * RFID_data,u8 len);				//��żУ��
CRC16Def WireLessCRC16_check(u8 * RFID_data,u8 len);		//��żУ��
u32 RFID_receive(u8 * RFID_data);												//
#endif
