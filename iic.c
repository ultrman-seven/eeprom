//IIC����ͨ��
#include"iic.h"

/*
��������ʱ����Ƭ�����ȷ���һ���ֽڵı�д�������Ĵ洢�����׵�ַ��
�յ��洢��������Ӧ��󣬵�Ƭ����������͸������ֽڣ���ÿ����һ���ֽں�Ҫ�ȴ�Ӧ��

AT24Cϵ������Ƭ�ڵ�ַ�ڽ��յ�ÿһ�������ֽڵ�ַ���Զ���1����оƬ�ġ�һ��װ���ֽ���������ͬоƬ�ֽ�����ͬ���޶��ڣ�
ֻ�������׵�ַ��װ���ֽ�������оƬ�ġ�һ��װ���ֽ�����ʱ�����ݵ�ַ�����Ͼ���ǰ������ݽ������ǡ�

��Ҫд������ݴ�����󣬵�Ƭ��Ӧ������ֹ�ź��Խ���д�������
*/
void Write24C02(un8 write, un8 address)
{
	iicStart();
	iicSendByte(0xa0);
	iicSendByte(address);
	iicSendByte(write);
	iicEnd();
}

/*
��������
	 ��Ƭ���ȷ��͸�������7λ��ַ���д����λ��0������αд������
��������ͷ�SDA�߲���SCL���ϲ�����9��ʱ���źš���ѡ�еĴ洢��������ȷ�����Լ��ĵ�ַ��
��SDA���ϲ���һ��Ӧ���ź���Ϊ��Ӧ��

Ȼ���ٷ�һ���ֽڵ�Ҫ���������Ĵ洢�����׵�ַ���յ�Ӧ���
��Ƭ��Ҫ�ظ�һ����ʼ�źŲ�����������ַ�Ͷ�����λ����1�������յ�����Ӧ���Ϳ��Զ��������ֽڣ�
ÿ����һ���ֽڣ���Ƭ����Ҫ�ظ�Ӧ���źš������һ���ֽ����ݶ���󣬵�Ƭ��Ӧ�����ԡ���Ӧ�𡱣��ߵ�ƽ����
��������ֹ�ź��Խ�������������
*/
un8 Read24C02(un8 address)
{
	iicStart();
	iicSendByte(0xa0);
	while (SDA);
	iicSendByte(address);
	while (SDA);
	iicStart();
	iicSendByte(0xa1);
	iicEnd();
	return iicReadByte();
}

/*
 I2C���߽������ݴ���ʱ��ʱ���ź�Ϊ�ߵ�ƽ�ڼ䣬�������ϵ����ݱ��뱣���ȶ���
 ֻ����ʱ�����ϵ��ź�Ϊ�͵�ƽ�ڼ䣬�������ϵĸߵ�ƽ��͵�ƽ״̬������仯��
*/

un8 iicSendByte(un8 send)
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

un8 iicReadByte(void)
{
	un8 result, count;
	for (result = count = SCL = 0; count < 8; count++)
	{
		delay(DELAY_10us);
		SCL = 1;
		delay(DELAY_10us);
		result <<= 1;
		result |= SDA;
		SCL = 0;
	}
	return result;
}

void iicStart(void)//SCL��Ϊ�ߵ�ƽ�ڼ䣬SDA���ɸߵ�ƽ��͵�ƽ�ı仯��ʾ��ʼ�ź�
{
	SCL = SDA = 1;
	delay(DELAY_10us);
	SDA = 0;
	delay(DELAY_10us);
	SCL = 0;
}

void iicEnd(void)//SCL��Ϊ�ߵ�ƽ�ڼ䣬SDA���ɵ͵�ƽ��ߵ�ƽ�ı仯��ʾ��ֹ�źš�  
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