#ifndef __IIC_H__
#define __IIC_H__

//IIC总线通信
#include<reg52.h>
#define DELAY_10us 10
typedef unsigned char un8;
typedef unsigned int un16;

sbit SDA = P2 ^ 0;//数据线
sbit SCL = P2 ^ 1;//时钟线

void iicStart(void);
void iicEnd(void);
un8 iicReadByte(void);
un8 iicSendByte(un8 send);
un8 Read24C02(un8 address);
void delay(un16 n);

void Write24C02(un8 write, un8 address);
un8 Read24C02(un8 address);
#endif // !__IIC_H__
