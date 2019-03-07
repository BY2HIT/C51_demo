#include<STC12C5A.h>
#include<intrins.h>

sbit SPEAK=P4^6;

void delay_ms(unsigned int n);
void delay_us(unsigned int x);

void Delay5ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 92;
	do
	{
		while (--j);
	} while (--i);
}

void main()
{
	P4M0=0xff;
	P4M1=0x00;
	P4SW|=0x40;
	while(1)
	{
		SPEAK=1;
		delay_us(1);
		SPEAK=0;
		delay_us(1);
	}
}

void delay_ms(unsigned int n)
{
	unsigned char i, j, k;
	while(n--)
	{
		i = 1;
		j = 234;
		k = 113;
		do
		{
			do
			{
				while (--k);
			} while (--j);
		} while (--i);
	}
}

void delay_us(unsigned int x)		//@12.000MHz  50us
{
	unsigned int i;

	while(x--)
	{
		i = 1000;
		while (--i);
	}
}


