#include<STC12C5A.h>
#include<intrins.h>
//����ds18b20��
sbit DQ = P3^5;

unsigned char SMG_dat[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};//���ֿ�

//ȫ�ֱ���
float temperature;   //�¶�
unsigned char tempL=0; 
unsigned char tempH=0; 

//��������
void DSinit();
void Delay750us();
void Delay500us();
void Delay45us();
void Delay15us();
void Delay30us();
void delay(unsigned int k);
void WriteOneChar(unsigned char dat);
unsigned char ReadOneChar();
void display(unsigned long a);
float ReadTemperature();

//������
void main(){
	long show;
	int t;
	float i,j;
//	P4SW=0x20;   //4.5 ����Ϊio��
	while(1){
		show=0;
		i=ReadTemperature();
		j=i;
		i=i/1;
		j=(j-i)/1*100;
		t=i*100+j;
		while(show<1000){
			show++;
			display(t);
		}
	}
}


//��ʼ��18b20
void DSinit()    //18B20��ʼ��
{
  unsigned int i;
	DQ=1;
	i=100;
  while(i>0)i--;  
	DQ=0;
	Delay750us();
  DQ=1;
	Delay500us();
}

//д������
void WriteOneChar(unsigned char dat){
	unsigned char i=0;
	for(i=8;i>0;i--){
		DQ=0;
		Delay15us();
		DQ=dat&0x01;
		Delay45us();
		DQ=1;
		dat>>=1;
	}
	delay(4);
	DQ=1;
}

//��ȡ����
unsigned char ReadOneChar(){
	unsigned char i=0;
	unsigned char t=0;
	unsigned char dat=0;
	for (i=8;i>0;i--){
		DQ=1;
		_nop_();   //2us
		_nop_();
		t = 9;
		while (--t);
		DQ=0;
		_nop_();    //6us
		_nop_();
		t = 33;
		while (--t);
		dat>>=1;
		DQ=1; 
		_nop_();    //4us
		_nop_();
		t = 21;
		while (--t);
		
		if(DQ){    //����
			dat|=0x80;
		}
		Delay30us();
	}
	return(dat);
}

//��ȡ�¶�
float ReadTemperature(){
	DSinit();
	WriteOneChar(0xcc); 
	WriteOneChar(0x44);
	delay(125);
	DSinit();
	WriteOneChar(0xcc);
	WriteOneChar(0xbe); 
	tempL=ReadOneChar(); 
	tempH=ReadOneChar(); 
	temperature=((tempH*256)+tempL)*0.0625;
	delay(200);
	return(temperature);
}

//��ʱ����
void delay(unsigned int k){
	unsigned int n;
	n=0;
	while(n < k){
		n++;
	}
}
void Delay15us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 87;
	while (--i);
}
void Delay30us()		//@24.000MHz
{
	unsigned char i, j;

	i = 1;
	j = 176;
	do
	{
		while (--j);
	} while (--i);
}

void Delay500us()		//@24.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}
void Delay750us()		//@24.000MHz
{
	unsigned char i, j;

	i = 18;
	j = 127;
	do
	{
		while (--j);
	} while (--i);
}

void Delay45us()		//@24.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 9;
	do
	{
		while (--j);
	} while (--i);
}


//SMG����
void display(unsigned long a){      
	unsigned char B[4];
	int i,j;
	
	P2M0=0xff;  //����
	P2M1=0x00;

	for(i=0;i<4;i++){  //��a ���룬�������λ��
		B[i]=a%10;
		a=a/10;
	}
	P0=0xF7;
	for(j=0;j<4;j++){
		if(j==2){
			P2=SMG_dat[B[j]]|0x80;
		}else if(j!=0){
			P2=SMG_dat[B[j]];
		}else {
			P2=0x58;
		}
		
		delay(500);
		P0=P0>>1;
		P2=0x00;
	}
}

