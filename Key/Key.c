#include<reg52.h>
sbit KEY=P3^2;
sbit LED1=P3^6;
sbit LED2=P3^7;

void Delay(unsigned int x)
{
	int i,j;
	for(i=x;i>0;i--)
	{
		for(j=110;j>0;j--);
	}
}

void main()
{
		KEY = 1;
		LED1 = 1;
		LED2 = 1;
		while(1)
		{
				if(KEY == 0)
				{
						Delay(10);
						if(KEY == 0)
						{
								LED1 = 0;
								LED2 = 0;
						}
				}
				else
				{
					LED1 = 1;
					LED2 = 1;
				}
		}
}





