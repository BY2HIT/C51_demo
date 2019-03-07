#include<stc12c5a.h>
#define uchar unsigned char
uchar SMG_dat[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};   //数字库
uchar scanner[3]={0xFE,0xFD,0xFB};																			//扫描行
void main()
{
	int matrix();
	P2M0=0xFF;
	P2M1=0x00;
	P0=0xF0;
	while(1)
	{
		P2=SMG_dat[matrix()];
	}
}

int matrix()
{
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
	return 0;
}



