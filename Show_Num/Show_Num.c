#include<STC12C5A.h>
#define uchar unsigned char
uchar SMG_dat[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
void show4bitNum(int a);
void delay();
void main (){
		show4bitNum(1314);
}

void show4bitNum(int a){
	uchar B[4];
	int i,j;
	
	P2M0=0xff;  //推挽
	P2M1=0x00;

	for(i=0;i<4;i++){  //将a 传入，并拆成四位！
		B[i]=a%10;
		a=a/10;
	}
	while (1){
		P0=0xF7;
		
		for(j=0;j<4;j++){
			P2=SMG_dat[B[j]];
			delay();
			P0=P0>>1;
			P2=0x00;
		}
	}
}

void delay(){
	unsigned int j;
	for(j=0;j<500;j++){
		;
	}
}



