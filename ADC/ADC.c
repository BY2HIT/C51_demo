/***串口电压表***/
//通过串口1发送'a','b'或者'c'
//分别返回P1.0,P1.1,P1.7采集得到的电压值

#include <stc12c5a.h>
unsigned char chr;
unsigned char flag;
void UART_init()	  //串口初始化
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xFB;		//独立波特率发生器时钟为Fosc/12,即12T
	BRT = 0xF3;		//设定独立波特率发生器重装值
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	EA=1;ES=1;			//开启总中断
}
static char *itoa(int value,char *string,int radix)
{    
	int i,d;   
	int flag=0;
	char *ptr=string;
	/*This implementation only work for decimal numbers.*/
	if(radix!=10)
	{
		*ptr=0;       
		return string;
	}
	if(!value)   
	{
		*ptr++=0x30;       
		*ptr=0;
		return string;
	}
	/*If this is a negative value insert the minus sign.*/   
	if (value<0)
	{
		*ptr++='-';       
		value *=-1;
	}
	for (i=10000;i>0;i/=10)   
	{
		d=value/i;        
		if(d||flag)        
		{ 
			*ptr++=(char)(d+0x30);           
			value-=(d*i);
			flag=1;
		}
	}
	/*Null terminate the string.*/   
	*ptr=0;
	return string;
}
void UART_send_string(unsigned char*p)
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
void Delay500ms()		//@24.000MHz
{
	unsigned char i, j, k;
	for(i=0;i<46;i++)
		for(j=0;j<153;j++)
			for(k=0;k<245;k++);
}

void ADC_init()
{
	AUXR1 &=0xfb;			// ADRJ=0 ,读取模式
	P1ASF=0X83;     //设置P1.7、P1.1、P1.0为AD模式（作为AD通道1000 0011）
	ADC_CONTR |= 0x80;  //打开AD电源
	Delay500ms();       //等待一段时间让AD电源稳定
	ADC_CONTR |= 0x60;   //设置最快速度转换
	ADC_CONTR &=0xef; //清零ADC中断 ADC_FLAG = 0;            
}
void Init_P1_x(char channel)
{
	ADC_CONTR &=0xef; //清零ADC中断 ADC_FLAG = 0;
	ADC_CONTR &= 0XF8; //设置通道x（P1.x）作为AD输入
	ADC_CONTR |= channel;  //设置通道7（P1.x）作为AD输入
	ADC_CONTR |= 0X08;  //打开ADC转换一次
	while(ADC_CONTR & 0X10 != 0);   //等待AD转换完成
}
int Get_AD_date()
{
	unsigned int t1,t2,date;
	t1=ADC_RES;	//高两位
	t2=ADC_RESL;	 //低八位
	date=(t1<<2)+t2;
	return date;
}
void uart_print1(unsigned int V0,unsigned int V1,unsigned int V7)//处理数据
{
	char string[10];
	int volte0_zhengshu,volte1_zhengshu,volte7_zhengshu;
	int volte0_xiaoshu,volte1_xiaoshu,volte7_xiaoshu;
	volte0_zhengshu=V0*5/1023;
	volte1_zhengshu=V1*5/1023;
	volte7_zhengshu=V7*5/1023;
	volte0_xiaoshu=(V0*5000/1023)%1000;
	volte1_xiaoshu=(V1*5000/1023)%1000;
	volte7_xiaoshu=(V7*5000/1023)%1000;
	if (flag==1)
	{
		flag=0;
		if(chr=='a')//1.0
		{
			while(ADC_CONTR & 0X10 != 0);
			UART_send_string(itoa(volte0_zhengshu,string,10));
			UART_send_string(".");
			UART_send_string(itoa(volte0_xiaoshu,string,10));
			UART_send_string("V\n");
		}
		if(chr=='b')//1.1
		{
			while(ADC_CONTR & 0X10 != 0);
			UART_send_string(itoa(volte1_zhengshu,string,10));
			UART_send_string(".");
			UART_send_string(itoa(volte1_xiaoshu,string,10));
			UART_send_string("V\n");
		}
		if(chr=='c')//1.7
		{
			while(ADC_CONTR & 0X10 != 0);
			UART_send_string(itoa(volte7_zhengshu,string,10));
			UART_send_string(".");
			UART_send_string(itoa(volte7_xiaoshu,string,10));
			UART_send_string("V\n");
		}
	}
}	

/*void uart_print(int V0,int V1,int V7)//原始数据
{
	char string[10];
	if (flag==1)
	{
		flag=0;
		if(chr=='a')//1.0
		{
			while(ADC_CONTR & 0X10 != 0);
			UART_send_string(itoa(V0,string,10));
			UART_send_string("\n");	
		}
		if(chr=='b')//1.1
		{
			while(ADC_CONTR & 0X10 != 0);
			UART_send_string(itoa(V1,string,10));
			UART_send_string("\n");	
		}
		if(chr=='c')//1.7
		{
			while(ADC_CONTR & 0X10 != 0);
			UART_send_string(itoa(V7,string,10));		
		  	UART_send_string("\n");		
		}
	}
}	*/
void main()
{
	int VOL0,VOL1,VOL7;		 
	UART_init();
	ADC_init();
	while(1)
	{
		Init_P1_x(0);
		VOL0=Get_AD_date();
		Init_P1_x(1);
		VOL1=Get_AD_date();           
		Init_P1_x(7);
		VOL7=Get_AD_date();
		uart_print1(VOL0,VOL1,VOL7);
//		uart_print(VOL0,VOL1,VOL7);
	}
}

void UART_sevrer() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		chr=SBUF;
		flag=1;
	}
}


