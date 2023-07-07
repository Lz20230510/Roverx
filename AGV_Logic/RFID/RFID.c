#include "HEAD.h"
/********************RFID数据接收并处理******************/
//u32 RFID_receive(u8 * RFID_data)
//数据输入：u16 RFID_data ：读取到的RFID  数据 
//数据输出：无
//返回：u32 读取到的RFID值 ,当返回数据为0xFFFFFFFF时，卡无效
/******************************************************/
u32 RFID_receive(u8 * RFID_data)
{
	u32 temp=0;
	CRC16RFID=RFIDCRC16_check(RFID_data,9);
	if(CRC16RFID.error)			//判断校验
	{
		temp=RFID_format(RFID_data);		//获取数据
	}
	else temp=0xFFFFFFFF;							//temp校验错误temp=0xFFFFFFFF
	return temp;											//返回数据
}


/********************RFID数据整定******************/
//u32 RFID_format(u8 * RFID_data)
//数据输入：u16 RFID_data ：读取到的RFID  
//数据输出：无
//返回：u32 读取到的RFID值
/******************************************************/
u32 RFID_format(u8 * RFID_data)
{
	u32 temp=0;
	
	temp=RFID_exchange(*(RFID_data+6));					//提取数据的第一组
	temp=(temp<<8)|RFID_exchange(*(RFID_data+5));		//提取数据的第二组
	temp=(temp<<8)|RFID_exchange(*(RFID_data+4));		//提取数据的第三组
	temp=(temp<<8)|RFID_exchange(*(RFID_data+3));		//提取数据的第四组
	
	return temp;
}


/********************RFID数据高低4位调换******************/
//u8 RFID_exchange(u8 date_in)
//数据输入8 date_in：需要改调换高低8位的数据 
//数据输出：无
//返回：u8 返回调换后的数据
/******************************************************/
u8 RFID_exchange(u8 date_in)
{
	u8 temp=0;
	temp=((date_in&0xF0)>>4)&0x0F;			//获得低四位
	temp=((date_in<<4)&0xF0)|temp;				//获得高四位
	return temp;
}


/********************CRC16奇偶校验*****************/
//CRC16Def WireLessCRC16_check(u8 * RFID_data,u8 len)			
//数据输入u8 * RFID_data：读取到的RFID数据,u8 数据长度	:
//数据输出：无
//返回：1:数据正确  0：数据错误
/******************************************************/
CRC16Def WireLessCRC16_check(u8 * RFID_data,u8 len)
{	
	static CRC16Def CRC16;
	u8 i,j;
  u16 wCrc=0xffff;
	u16 wPolynom=0xA001,temp=0; 	//是否移植到哪都能用
  for(i=0;i<len-2;i++)					//求CRC16校验值
  {
    wCrc^=RFID_data[i];
    for (j=0;j<8;j++)
    {
      if (wCrc&0x0001)wCrc=(wCrc>>1)^wPolynom;
      else wCrc=wCrc>>1;
    }
  } 
	temp=((temp|RFID_data[len-1])<<8)|RFID_data[len-2];
	if(wCrc==temp) CRC16.error=1;															//如果校验正确返回1，如果错误返回0							
	else CRC16.error=0;
	CRC16.crc16High=(u8)(wCrc>>8);									//获取高8位校验
	CRC16.crc16Low=(u8)wCrc;										//获取低8位校验
	return CRC16;
}


/********************CRC16奇偶校验*****************/
//CRC16Def RFIDCRC16_check(u8 * RFID_data,u8 len)			
//数据输入u8 * RFID_data：读取到的RFID数据,u8 数据长度	:
//数据输出：无
//返回：1:数据正确  0：数据错误
/******************************************************/
CRC16Def RFIDCRC16_check(u8 * RFID_data,u8 len)
{	
	static CRC16Def CRC16;
	u8 i,j;
  u16 wCrc=0xffff;
	u16 wPolynom=0xA001,temp=0; 	//是否移植到哪都能用
  for(i=0;i<len-2;i++)					//求CRC16校验值
  {
    wCrc^=RFID_data[i];
    for (j=0;j<8;j++)
    {
      if (wCrc&0x0001)wCrc=(wCrc>>1)^wPolynom;
      else wCrc=wCrc>>1;
    }
  } 
	temp=((temp|RFID_data[len-1])<<8)|RFID_data[len-2];
	if(wCrc==temp) CRC16.error=1;															//如果校验正确返回1，如果错误返回0							
	else CRC16.error=0;
	CRC16.crc16High=(u8)(wCrc>>8);									//获取高8位校验
	CRC16.crc16Low=(u8)wCrc;										//获取低8位校验
	return CRC16;
}


