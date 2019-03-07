#include<reg52.h>
sbit KEY=P3^2;
sbit LED1=P3^6;
sbit LED2=P3^7;

void main()
{
		KEY = 1;
		EA = 1;
		EX0 = 1;
		IT0 = 1;
		LED1 = 0;
	  LED2 = 1;
		while(1);
}

void INTO_Service() interrupt 0
{
		LED1 = ~LED1;
		LED2 = ~LED2;
}

