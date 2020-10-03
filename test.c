#include"iic.h"
#define ADDRESS 1
#define GAP delay(50000);P0 = 0x80;delay(50000);
un8 dis_num[] = { 0x3f,0x06, 0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,/*0-9*/
							0x77,0x7c,0x58, 0x5e, 0x79,0x71,/*a-f*/		0x80/*.*/ };
void main()
{
	un8 result, count;
	
	iicStart();
	P0 = dis_num[iicSendByte(0xa0)];
	GAP
	P0 = dis_num[iicSendByte(ADDRESS)+1];
	GAP
	P0 = dis_num[iicSendByte(0x04)+2];
	GAP
	//iicEnd();
	
	iicStart();
	P0 = dis_num[iicSendByte(0xa0)];
	GAP
	P0 = dis_num[iicSendByte(ADDRESS)+1];
	GAP
	iicStart();
	P0 = dis_num[iicSendByte(0xa1)+2];
	GAP
	//P0 = dis_num[iicReadByte()];
	
	for (result = count = SCL = 0; count < 8; count++)
	{
		delay(DELAY_10us);
		SCL = 1;
		delay(DELAY_10us);
		result <<= 1;
		result |= SDA;
		P0 = dis_num[SDA];
		GAP
		SCL = 0;
	}
	GAP
	P0 = dis_num[result];
	GAP
	iicEnd();
	
//    un8 num;
//    Write24C02(0x03,ADDRESS);
//    num = Read24C02(ADDRESS);
//    P0 = dis_num[num];
    while (1)
    ;
}