#include "stc12c5a.h"

void delay()		//@24.000MHz

{
	unsigned char i, j, k;

	i = 46;
	j = 153;
	k = 245;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}




void main()
{
	unsigned int a[17]={0x00,0x00,0x00,0x06,0x00,0x38,0x3F,0x3E,0x79,0x00,0x6E,0x3F,0xBE,0x00,0x00,0x00,0x00};
	unsigned int b[4]={0x0E,0x0D,0x0B,0x07};
	int i,j,k;
	P2M0=0xFF;
	P2M1=0x00;
	while (1)
	{
		for(k=0;k<13;k++)
		{
			for(i=0;i<1500;i++)
			{
				P2=a[k];
				P0=b[0];
				for(j=0;j<100;j++);
			
				P2=a[k+1];
				P0=b[1];
				for(j=0;j<100;j++);
			
				P2=a[k+2];
				P0=b[2];
				for(j=0;j<100;j++);
			
				P2=a[k+3];
				P0=b[3];
				for(j=0;j<100;j++);
			}
		}
	}
}


