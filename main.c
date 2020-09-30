#include"iic.h"

sbit in_hc138_0 = P2 ^ 2;
sbit in_hc138_1 = P2 ^ 3;
sbit in_hc138_2 = P2 ^ 4;

sbit K1 = P3 ^ 1;
sbit K2 = P3 ^ 0;
sbit K3 = P3 ^ 2;
sbit K4 = P3 ^ 3;

un8 dis_num[] = { 0x3f,0x06, 0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,/*0-9*/
							0x77,0x7c,0x58, 0x5e, 0x79,0x71,/*a-f*/		0x80/*.*/ };

un8 value;

void main()
{
	void digitalTube(un16 val);
	char matButton(void);

	un16 input, left, right;
	un8 state;
	left = right = input = 0;
	while (1)
	{
		if (matButton())
			switch (value)
			{
			case 16://send input to eeprom
				Write24C02(input, 1);
				break;

			case 17:case 18:case 19://get data from eeprom and send in to input
				input = Read24C02(1);
				break;

			case 12:// +
				left = input;
				input = 0;
				state = 1;
				break;
			case 13:// -
				left = input;
				input = 0;
				state = 2;
				break;
			case 14:// *
				left = input;
				input = 0;
				state = 3;
				break;
			case 15:// /
				left = input;
				input = 0;
				state = 4;
				break;

			case 10:// =
				right = input;
				switch (state)
				{
				case 1://+
					input = left + right;
					break;
				case 2://-
					input = left - right;
					break;
				case 3://*
					input = left * right;
					break;
				case 4:// /
					input = left / right;
					break;
				default:
					break;
				}
				break;
			case 11:// C
				left = right = input = 0;
				break;

			default:
				input = input * 10 + value;
				break;
			}
		digitalTube(input);
	}

}

char matButton(void)
{
	char findZero(un8);

	P1 = 0x0f;
	if (P1 != 0x0f)
	{
		delay(1000);
		if (P1 != 0x0f)
		{
			value = 3 - findZero(P1);
			P1 = 0xf0;
			value += (4 * findZero(P1 / 0x10));
			while (P1 != 0xf0)
				delay(1000);
			return 1;
		}
	}
	if (!(K1 && K2 && K3 && K4))
	{
		switch (P3 | 0xf0)
		{
		case 0xfd:value = 16; return 1;//K1
		case 0xfe:value = 17; return 1;//K2
		case 0xfb:value = 18; return 1;//K3
		case 0xf7:value = 19; return 1;//K4
		default:
			break;
		}
	}
	return 0;
}
char findZero(un8 n)
{
	char count;
	for (count = 0; n % 2; n /= 2)
		count++;
	return count;
}

void digitalTube(un16 val)
{
	void delay(un16 n);
	void hc138(un8 n);
	un8 count;
	if (val)
		for (count = 0; count <= 7; count++, val /= 10)
		{
			hc138(count);
			P0 = val ? dis_num[val % 10] : 0x00;
			delay(200);
			P0 = 0x00;
		}
	else
	{
		hc138(0);
		P0 = dis_num[0];//0x79;
	}
}
void hc138(un8 n)
{
	if (0 <= n && n <= 7)
	{
		in_hc138_0 = n % 2;
		n /= 2;
		in_hc138_1 = n % 2;
		n /= 2;
		in_hc138_2 = n % 2;
	}
}