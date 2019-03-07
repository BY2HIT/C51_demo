#include<STC12C5A.h>

sbit led1=P3^6;
sbit led2=P3^7;

void delay();

void main() 
{
	led1=0;
	led2=1;
	
	while(1)
	{
		led1=~led1;
		led2=~led2;
		delay();
	}
}

void delay()
{
	unsigned int i;
	for(i=0;i<50000;i++)
	{
		;
	}
}


