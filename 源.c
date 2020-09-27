//IIC����ͨ��
#include<reg52.h>
#define DELAY_10us 10
typedef unsigned char un8;
typedef unsigned int un16;

un8 SDA, SCL;
//sbit sda = P2 ^ 0;
//sbit scl = P2 ^ 1;

un8 iccSendByte(un8 send)
{
	un8 count = 8;
	un16 maxWait = 1000;
	while (count--)
	{
		SCL = 0;
		SDA = send >> 7;
		send = send << 1;
		delay(DELAY_10us);
		SCL = 1;
		delay(DELAY_10us);
		SCL = 0;
		SDA = 1;
	}
	SCL = 1;
	delay(DELAY_10us);
	while (SDA)
		if (!maxWait--)
		{
			SCL = 0;
			return 0;
		}
	SCL = 0;
	return 1;
}

void iccStart(void)//SCL��Ϊ�ߵ�ƽ�ڼ䣬SDA���ɸߵ�ƽ��͵�ƽ�ı仯��ʾ��ʼ�ź�
{
	SCL = SDA = 1;
	delay(DELAY_10us);
	SDA = 0;
	delay(DELAY_10us);
	SCL = 0;
}

void iccEnd(void)//SCL��Ϊ�ߵ�ƽ�ڼ䣬SDA���ɵ͵�ƽ��ߵ�ƽ�ı仯��ʾ��ֹ�źš�  
{	
	SDA = 0;
	SCL = 1;
	delay(DELAY_10us);
	SDA = 1;
	delay(DELAY_10us);
}

void delay(un16 n)
{
	while (n--);
}