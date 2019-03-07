#include<STC12C5A.h>

//宏定义
#define uchar unsigned char
#define uint unsigned int

//定义脚
sbit zero=P3^4;      //按键0
sbit enter=P3^5;		 //enter

//定义全局变量  
int chr;			//返回字符串
char flag;		//传输成功标志

//声明函数
void UartInit();
void UART_send_string(int a);
void display(int a);
int getKEY();
int getNum();
void Delay100us()	;

//主函数
void main()
{
	int a,b,c;
	UartInit();
	a=getNum();
	b = a / 100 + 100;    //8位数据最大255，将四位数字拆成两组发送，高两位+100与低两位区别
	c = a % 100;
	UART_send_string(b);
	Delay100us();
	UART_send_string(c);
}

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

/**************************矩阵键盘驱动*******************************/
//矩阵键盘驱动
int getKEY()
{
	uchar scanner[3]={0xFE,0xFD,0xFB}; //扫描行
	int i=0;
	int x=0;  //行坐标
	int y=0;  //列坐标
	uchar ifPress;
	int keyNumber=0;
	x=0;
	y=0;
	for(i=0;i<3;i++)
	{
		x=i+1;
		P1=scanner[i];    	//扫描第i行
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
}

int getNum()
{
		int key;    	//按键数字
		int number=0;	//整个数字
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
				Delay100us();
			}
			if(enter==0)
			{
				Delay100us();
				while(enter==0);
				break; 
			}
		}
	return number;
}
/*************************通信部分（核心）**********************/
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

//发送字符串
void UART_send_string(int a)
{
	ES=0;

	SBUF=(unsigned char)a;
	while(!TI);
	TI=0;

	ES=1;
}

//接收装置
void UART_server() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		chr=SBUF;
		flag=1;
	}
}



