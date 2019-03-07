#include<STC12C5A.h>
#define uchar unsigned char

sbit LED0=P3^7;
sbit zero=P3^4;
sbit enter=P3^5;

unsigned char SMG_dat[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//数字库
uchar scanner[3]={0xFE,0xFD,0xFB};																						//扫描行

void display(unsigned long a);
void delay(unsigned int a);

unsigned int t;   //时间
int ss=0;
int mm=0;
int hh=0;
unsigned int num=0;
int num2=0;

//主函数
void main()
{
	LED0=0;
	AUXR&=0x7F;  //定时器中断寄存器设置
	TMOD=0x02;
	TL0=0xEC;		
	TH0=0xEC;		
	EA=1;					//启动总中断
	ET0=1;				//启动定时器0中断
	TR0=1;				//启动定时器0
	EX1=1;				//按键中断  3.3
	IT1=0;				//按下
	IP=0x04;			//按键中断优先
	while(1)
	{	
		if(mm<60&&hh<24)
		{
			t=mm+hh*100;
		}
		if(mm>60||hh>24)
		{
			t=0;
		}
		display(t);
		P0=0xFF;
	}
}

//SMG驱动
void display(unsigned long a)
{      
	unsigned char B[4];
	int i,j;
	
	P2M0=0xff;  //推挽
	P2M1=0x00;

	for(i=0;i<4;i++)
	{  //将a 传入，并拆成四位！
		B[i]=a%10;
		a=a/10;
	}
	P0=0xF7;
	for(j=0;j<4;j++)
	{
		if(j==2)
		{                  //中间那位小数点闪烁
			if(LED0==0)
			{
				P2=SMG_dat[B[j]]|0x80;
			}
			else
			{
				P2=SMG_dat[B[j]];
			}
		}
		else
		{
			P2=SMG_dat[B[j]];
		}
		delay(500);
		P0=P0>>1;
		P2=0x00;
	}
}

void delay(unsigned int a)
{
	unsigned int j;
	for(j=0;j<a;j++)
	{
		;
	}
}

int getKEY()
{
	//矩阵键盘读取
	int i=0;
	int x=0; //行坐标
	int y=0;  //列坐标
	uchar ifPress;
	int keyNumber=0;
	x=0;
	y=0;
	for(i=0;i<3;i++)
	{
		x=i+1;
		P1=scanner[i];    //扫描第i行
		ifPress=P1 & 0xE0;
		if(ifPress!=0xE0)
		{
			switch (ifPress)
			{
				case 0xC0 : 
					y=1;
					break;
				case 0xA0:
					y=2;
					break;
				case 0x60:
					y=3;
					break;
				default : 
					x=0;
					break;
			}
			keyNumber=3*x-3+y;
			return keyNumber;
			break;
		}	
	}
	if(zero==0)
	{
		return 0;
	}
	return 10;
	//没按下任何按键
}

void INT1() interrupt 2
{
	//按键中断函数
	unsigned char key;
	unsigned long number=0;
	while(1)
	{
		key=getKEY();
		display(number);
		if(key!=10)
		{
			number = (number*10+key)%10000;
			while(getKEY()!=10)
			{
				display(number);
			}
			delay(100);
		}
		mm=number%100;
		hh=number/100;
		if(mm<60&&hh<24)
		{
			t=mm+hh*100;
		}
		if(enter==0)
		{
			while(enter==0);
			break;
		}
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
			t=mm+hh*100;
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


