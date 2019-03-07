#include <stc12c5a.h>

void delay();

void main()
{
	unsigned char LOVE[4]={0x38,0x3F,0x3E,0x79};
	unsigned char position[4]={0x0E,0x0D,0x0B,0x07};
	int i;
	P2M0=0xFF;
	P2M1=0x00;
	for(i=0;i<4;i++)
	{
		P0=position[i];
		P2=LOVE[i];
		delay();
		P0=0xff;
	}
}

void delay()
{
	unsigned int j;
	for(j=0;j<9000;j++)
	{
		;
	}
}

