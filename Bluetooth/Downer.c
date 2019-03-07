#include<STC12C5A.h>

//定义全局变量
int chr;   //接收
char flag;	//接收标志

//声明函数
void display(int a);
void UartInit();
void UART_send_string(int *p);
void Delay100us();

//主函数
void main()
{
	UartInit();
	while(1)
	{
		if(flag==1)
		{
			while(1)
			display(chr);
			flag=0;
		}
	}
}

/*************************数码管驱动**************************************/
//SMG驱动
void display(int a)
{      
	int SMG_dat[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; //数字库
	int B[4];    			//四位数码管
	int i,j;
	
	P2M0=0xff;          //推挽
	P2M1=0x00;
	
	for(i=0;i<4;i++)
	{  //将a 传入，并拆成四位！
		B[i]=a%10;
		a=a/10;
	}
	P0=0xF7;  //第一位亮  
	for(j=0;j<4;j++)
	{
		P2=SMG_dat[B[j]];
		Delay100us();       //消影
		P0=P0>>1;
		P2=0x00;
	}
}
void Delay100us()		//@24.000MHz
{
	unsigned char i, j;
	i = 3;
	j = 82;
	do
	{
		while (--j);
	} while (--i);
} 

/*******************通信部分（核心）*************************************/
//串口初始化
void UartInit()		//9600bps@24.000MHz
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xFB;		//独立波特率发生器时钟为Fosc/12,即12T
	BRT = 0xF3;			//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	EA=1;
	ES=1;
}

void UART_send_string(int *p)
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
	int a;
	if(RI==1)
	{
		RI=0;
		a=SBUF;
		if(a>99)
		{
			chr=(a-100)*100;
		}
		else
		{
			chr=chr+a;
			flag=1;
		}
	}
}



