#include"iic.h"
#define ADDRESS 2
un8 dis_num[] = { 0x3f,0x06, 0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,/*0-9*/
							0x77,0x7c,0x58, 0x5e, 0x79,0x71,/*a-f*/		0x80/*.*/ };
void main()
{
    un8 num;
    Write24C02(0x08,ADDRESS);
    num = Read24C02(ADDRESS);
    P0 = dis_num[num];
    while (1)
    ;
}