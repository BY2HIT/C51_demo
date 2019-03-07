#include<intrins.h>

#define uchar unsigned char   	//宏定义

#define DS1302_SECOND_WRITE 0x80
#define DS1302_MINUTE_WRITE 0x82
#define DS1302_HOUR_WRITE 0x84
#define DS1302_WEEK_WRITE 0x8A
#define DS1302_DAY_WRITE 0x86
#define DS1302_MONTH_WRITE 0x88
#define DS1302_YEAR_WRITE 0x8C

//引脚设置
sbit SCLK = P0^4;   			//设置实时时钟时钟线引脚 //
sbit SDA = P0^5;     		//设置实时时钟数据线引脚 //
sbit RST = P0^6;    		//设置实时时钟复位线引脚 //

//时间变量
char sec,min,hour,week,day,month,year;

//函数声明
uchar DS1302Read(uchar cmd);
void DS1302Write(uchar cmd, uchar dat);
void DS1302WriteByte(uchar dat);
uchar DS1302ReadByte();
void Init_DS1302();
void Delay2us();


/************************************************
*名称: uchar DS1302Read()
*说明: 先写地址，后读数据
*功能: 从cmd相应地址中读取一个字节的数据
*调用: DS1302WriteByte(),DS1302ReadByte()
*输入: cmd:要写入的控制字节
*输出: dat:读取的数据
*************************************************/
uchar DS1302Read(uchar cmd){
	uchar dat;
	RST=0;								//初始CE线置为0
	SCLK=0;								//初始时钟线置为0
	RST=1;								//初始CE置为1，传输开始
	DS1302WriteByte(cmd);	//传输命令字，要读取的时间/日历地址
	dat=DS1302ReadByte();	//读取要得到的时间/日期
	RST=0; 								//读取结束，CE置为0，结束数据的传输
	SCLK=1; 							//时钟线拉高
	RST=1; 	
	return dat;						//返回得到的时间/日期
}
/************************************************
*名称: DS1302Write
*说明: 先写地址，后写数据
*功能: 向cmd相应地址中写一个字节的数据
*调用: DS1302WriteByte()
*输入: cmd:要写入的控制字,dat:要写入的数据
*输出: 无
*************************************************/
void DS1302Write(uchar cmd, uchar dat){
	RST=0;								//初始CE线置为0
	SCLK=0; 							//初始时钟线置为0
	RST=1; 								//初始CE置为1，传输开始
	DS1302WriteByte(cmd); //传输命令字，要写入的时间/日历地址
	DS1302WriteByte(dat); //写入要修改的时间/日期
	RST=0; 								//读取结束，CE置为0，结束数据的传输
	SCLK=1; 							//时钟线拉高
	RST=1; 	
}
/************************************************
*名称: DS1302WriteByte
*说明: 无
*功能: 写入8bit数据
*调用: delayus()
*输入: dat:要写入的数据
*输出: 无
*************************************************/
void DS1302WriteByte(uchar dat){
	uchar i;
	SCLK=0;//初始时钟线置为0
	Delay2us();
	for(i=0;i<8;i++)//开始传输8个字节的数据
	{
		SDA=dat&0x01;//取最低位，注意DS1302的数据和地址都是从最低位开始传输的
		Delay2us();
		SCLK=1;//时钟线拉高，制造上升沿，SDA的数据被传输
		Delay2us();
		SCLK=0;//时钟线拉低，为下一个上升沿做准备
		dat>>=1;//数据右移一位，准备传输下一位数据
	}
}
/************************************************
*名称: DS1302ReadByte()
*说明: 无
*功能: 读取8bit的数据
*调用: delayus()
*输入: 无
*输出: dat:读取的数据
*************************************************/
uchar DS1302ReadByte(){
	uchar i,dat;
	Delay2us();
	for(i=0;i<8;i++)
	{
		dat>>=1;				//要返回的数据左移一位
		if(SDA==1)			//当数据线为高时，证明该位数据为1
			dat|=0x80;		//要传输数据的当前值置为1,若不是,则为0
		SCLK=1;					//拉高时钟线
		Delay2us();
		SCLK=0;					//制造下降沿
		Delay2us();
		
	}
return dat;//返回读取出的数据
}

/************************************************
*名称: void Init_DS1302(void)
*说明: 给1302写入一个初始的值
*功能: 写入日期，和时钟的值
*调用: DS1302Write（）
*输入: 无
*输出: 无
*************************************************/
void Init_DS1302(){
	DS1302Write(0x8e,0x00);									//写保护关
	DS1302Write(DS1302_SECOND_WRITE,0x00); 	//初始秒值为0
	DS1302Write(DS1302_MINUTE_WRITE,0x20);	//初始分钟值为0
	DS1302Write(DS1302_HOUR_WRITE,0x09); 		//初始为24小时模式初始时间为0点
	DS1302Write(DS1302_DAY_WRITE,0x25); 		//2011年1月1日星期6
	DS1302Write(DS1302_MONTH_WRITE,0x12);
	DS1302Write(DS1302_YEAR_WRITE,0x10);
	DS1302Write(DS1302_WEEK_WRITE,0x06);
	DS1302Write(0x90,0xA5); 								//充电
	DS1302Write(0xc0,0xf0); 								//初始化一次标示
	DS1302Write(0x8e,0x80);
}
/**************************************
取回时间，并且转化为十进制
***************************************/
void get_time(){
	int a,b,c;
	a=DS1302Read(0x81);
	b=(a&0x70)>>4; //十位
	c=a&0x0f;	//个位
	sec=10*b+c;
	
	a=DS1302Read(0x83);
	b=(a&0x70)>>4; //十位
	c=a&0x0f;	//个位
	min=10*b+c;
	
	a=DS1302Read(0x85);
	b=(a&0x70)>>4; //十位
	c=a&0x0f;	//个位
	hour=10*b+c;
	
	a=DS1302Read(0x8B);
	week=a&0x07;

	a=DS1302Read(0x87);
	b=(a&0x70)>>4; //十位
	c=a&0x0f;	//个位
	day=10*b+c;
	
	a=DS1302Read(0x89);
	b=(a&0x70)>>4; //十位
	c=a&0x0f;	//个位
	month=10*b+c;
	
	a=DS1302Read(0x8D);
	b=(a&0x70)>>4; //十位
	c=a&0x0f;	//个位
	year=10*b+c;
}

//延时函数
void Delay2us()		//@24.000MHz
{
	unsigned char i;
	_nop_();
	_nop_();
	i = 9;
	while (--i);
}
