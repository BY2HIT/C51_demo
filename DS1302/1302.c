#include<STC12C5A.h>
#include"DS1302.h"

unsigned char SMG_dat[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//数字库

void display(unsigned long a);
void Delay100us();

void main(){
	Init_DS1302();
	while(1){
		get_time();
		display(hour*100+min);
	}
}



void display(unsigned long a){      //SMG驱动
	unsigned char B[4];
	int i,j;
	
	P2M0=0xff;  //推挽
	P2M1=0x00;

	for(i=0;i<4;i++){  //将a 传入，并拆成四位！
		B[i]=a%10;
		a=a/10;
	}

	P0=0xF7;
		
	for(j=0;j<4;j++){
		P2=SMG_dat[B[j]];
		Delay100us();  //消影
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







