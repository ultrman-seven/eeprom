//IIC总线通信
#include"iic.h"

/*
传送数据时，单片机首先发送一个字节的被写入器件的存储区的首地址，
收到存储器器件的应答后，单片机就逐个发送各数据字节，但每发送一个字节后都要等待应答。

AT24C系列器件片内地址在接收到每一个数据字节地址后自动加1，在芯片的“一次装载字节数”（不同芯片字节数不同）限度内，
只需输入首地址。装载字节数超过芯片的“一次装载字节数”时，数据地址将“上卷”，前面的数据将被覆盖。

当要写入的数据传送完后，单片机应发出终止信号以结束写入操作。
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
读出过程
	 单片机先发送该器件的7位地址码和写方向位“0”（“伪写”），
发送完后释放SDA线并在SCL线上产生第9个时钟信号。被选中的存储器器件在确认是自己的地址后，
在SDA线上产生一个应答信号作为回应。

然后，再发一个字节的要读出器件的存储区的首地址，收到应答后，
单片机要重复一次起始信号并发出器件地址和读方向位（“1”），收到器件应答后就可以读出数据字节，
每读出一个字节，单片机都要回复应答信号。当最后一个字节数据读完后，单片机应返回以“非应答”（高电平），
并发出终止信号以结束读出操作。
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
 I2C总线进行数据传送时，时钟信号为高电平期间，数据线上的数据必须保持稳定，
 只有在时钟线上的信号为低电平期间，数据线上的高电平或低电平状态才允许变化。
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

void iicStart(void)//SCL线为高电平期间，SDA线由高电平向低电平的变化表示起始信号
{
	SCL = SDA = 1;
	delay(DELAY_10us);
	SDA = 0;
	delay(DELAY_10us);
	SCL = 0;
}

void iicEnd(void)//SCL线为高电平期间，SDA线由低电平向高电平的变化表示终止信号。  
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