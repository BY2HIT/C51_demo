#include<STC12C5A.h>
#define uchar unsigned char
sbit LED0=P3^7;
unsigned int num=0;
int num2=0;

unsigned char SMG_dat[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void show4bitNum(int a);
void delay();

int ss=0;
int mm=0;
int hh=0;
unsigned int t;

void main()
{
	
	LED0=0;
	
	AUXR &= 0x7F;  //寄存器设置
	TMOD =0x02;
	TL0 = 0xEC;		
	TH0 = 0xEC;		
	EA=1;					//启动总中断
	ET0=1;				//启动定时器0中断
	TR0 = 1;			//启动定时器0
		
	while(1)
	{	
		show4bitNum(t);
		P2=0x00;
	}
}

void show4bitNum(int a)
{
	unsigned char B[4];
	int i,j;
	
	P2M0=0xff;  //推挽
	P2M1=0x00;

	for(i=0;i<4;i++)
	{
		//将a传入，并拆成四位！
		B[i]=a%10;
		a=a/10;
	}

	P0=0xF7;
		
	for(j=0;j<4;j++)
	{
		P2=SMG_dat[B[j]];
		delay();
		P0=P0>>1;
		P2=0x00;
	}
}

void delay()
{
	unsigned int j;
	for(j=0;j<500;j++)
	{
		;
	}
}

void T0_time() interrupt 1
{
	//中断函数
	if(num==10000)
	{
		num=0;
		num2++;
		if(num2==5||num2==10)
		{	
			LED0=~LED0;
		}
		if(num2==10)
		{
			num2=0;
			t=ss+mm*100;
			if(ss<59)
			{
				ss++;
			}
			else
			{
				if(mm<60-1)
				{
					ss=0;
					mm++;
				}
				else
				{
					if(hh<24-1)
					{
						mm=0;
						hh++;
					}
					else
					{
						ss=0;
						mm=0;
						hh=0;
					}
				}
			}
		}
	}
	num++;
}




