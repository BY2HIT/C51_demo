#include<STC12C5A.h>

char chr;
int flag;
char i;

void UartInit();
void UART_send_string(unsigned char *p);
void Delay();

void main ()
{
	flag=1;
	P2M0=0xFF;
	P2M1=0x00;
	P2=0xFF;
	UartInit();
	
	while(1)
	{
		if(flag==1)
		{
			flag=0;
			if(chr=='a')
			{
				P0=0x00;
				for(i=0;i<100;i++)
				{
					Delay();
				}
				P0=0xFF;
			}
			else
			{
				P0=0xFF;
			}
			UART_send_string("OK!\n");
		}
	}
}

void UartInit()		//9600bps@24.000MHz
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0x64;		//设定定时初值
	TH1 = 0x64;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1		
	EA=1;
	ES=1;
}

void UART_send_string(unsigned char *p)
{
	ES=0;
	while(*p!='\0')
	{
		SBUF=*p;
		while(!TI);
		TI=0;
		p++;
	}
	ES=1;
}

void UART_server() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		chr=SBUF;
		flag=1;
	}
}

void Delay()		//@24.000MHz
{
	unsigned char i, j, k;

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



