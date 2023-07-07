#include "HEAD.h"
/********************RFID���ݽ��ղ�����******************/
//u32 RFID_receive(u8 * RFID_data)
//�������룺u16 RFID_data ����ȡ����RFID  ���� 
//�����������
//���أ�u32 ��ȡ����RFIDֵ ,����������Ϊ0xFFFFFFFFʱ������Ч
/******************************************************/
u32 RFID_receive(u8 * RFID_data)
{
	u32 temp=0;
	CRC16RFID=RFIDCRC16_check(RFID_data,9);
	if(CRC16RFID.error)			//�ж�У��
	{
		temp=RFID_format(RFID_data);		//��ȡ����
	}
	else temp=0xFFFFFFFF;							//tempУ�����temp=0xFFFFFFFF
	return temp;											//��������
}


/********************RFID��������******************/
//u32 RFID_format(u8 * RFID_data)
//�������룺u16 RFID_data ����ȡ����RFID  
//�����������
//���أ�u32 ��ȡ����RFIDֵ
/******************************************************/
u32 RFID_format(u8 * RFID_data)
{
	u32 temp=0;
	
	temp=RFID_exchange(*(RFID_data+6));					//��ȡ���ݵĵ�һ��
	temp=(temp<<8)|RFID_exchange(*(RFID_data+5));		//��ȡ���ݵĵڶ���
	temp=(temp<<8)|RFID_exchange(*(RFID_data+4));		//��ȡ���ݵĵ�����
	temp=(temp<<8)|RFID_exchange(*(RFID_data+3));		//��ȡ���ݵĵ�����
	
	return temp;
}


/********************RFID���ݸߵ�4λ����******************/
//u8 RFID_exchange(u8 date_in)
//��������u8 date_in����Ҫ�ĵ����ߵ�8λ������ 
//�����������
//���أ�u8 ���ص����������
/******************************************************/
u8 RFID_exchange(u8 date_in)
{
	u8 temp=0;
	temp=((date_in&0xF0)>>4)&0x0F;			//��õ���λ
	temp=((date_in<<4)&0xF0)|temp;				//��ø���λ
	return temp;
}


/********************CRC16��żУ��*****************/
//CRC16Def WireLessCRC16_check(u8 * RFID_data,u8 len)			
//��������uu8 * RFID_data����ȡ����RFID����,u8 ���ݳ���	:
//�����������
//���أ�1:������ȷ  0�����ݴ���
/******************************************************/
CRC16Def WireLessCRC16_check(u8 * RFID_data,u8 len)
{	
	static CRC16Def CRC16;
	u8 i,j;
  u16 wCrc=0xffff;
	u16 wPolynom=0xA001,temp=0; 	//�Ƿ���ֲ���Ķ�����
  for(i=0;i<len-2;i++)					//��CRC16У��ֵ
  {
    wCrc^=RFID_data[i];
    for (j=0;j<8;j++)
    {
      if (wCrc&0x0001)wCrc=(wCrc>>1)^wPolynom;
      else wCrc=wCrc>>1;
    }
  } 
	temp=((temp|RFID_data[len-1])<<8)|RFID_data[len-2];
	if(wCrc==temp) CRC16.error=1;															//���У����ȷ����1��������󷵻�0							
	else CRC16.error=0;
	CRC16.crc16High=(u8)(wCrc>>8);									//��ȡ��8λУ��
	CRC16.crc16Low=(u8)wCrc;										//��ȡ��8λУ��
	return CRC16;
}


/********************CRC16��żУ��*****************/
//CRC16Def RFIDCRC16_check(u8 * RFID_data,u8 len)			
//��������uu8 * RFID_data����ȡ����RFID����,u8 ���ݳ���	:
//�����������
//���أ�1:������ȷ  0�����ݴ���
/******************************************************/
CRC16Def RFIDCRC16_check(u8 * RFID_data,u8 len)
{	
	static CRC16Def CRC16;
	u8 i,j;
  u16 wCrc=0xffff;
	u16 wPolynom=0xA001,temp=0; 	//�Ƿ���ֲ���Ķ�����
  for(i=0;i<len-2;i++)					//��CRC16У��ֵ
  {
    wCrc^=RFID_data[i];
    for (j=0;j<8;j++)
    {
      if (wCrc&0x0001)wCrc=(wCrc>>1)^wPolynom;
      else wCrc=wCrc>>1;
    }
  } 
	temp=((temp|RFID_data[len-1])<<8)|RFID_data[len-2];
	if(wCrc==temp) CRC16.error=1;															//���У����ȷ����1��������󷵻�0							
	else CRC16.error=0;
	CRC16.crc16High=(u8)(wCrc>>8);									//��ȡ��8λУ��
	CRC16.crc16Low=(u8)wCrc;										//��ȡ��8λУ��
	return CRC16;
}


