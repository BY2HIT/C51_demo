#include<STC12C5A.h>

//�궨��
#define uchar unsigned char
#define uint unsigned int

//�����
sbit zero=P3^4;      //����0
sbit enter=P3^5;		 //enter

//����ȫ�ֱ���  
int chr;			//�����ַ���
char flag;		//����ɹ���־

//��������
void UartInit();
void UART_send_string(int a);
void display(int a);
int getKEY();
int getNum();
void Delay100us()	;

//������
void main()
{
	int a,b,c;
	UartInit();
	a=getNum();
	b = a / 100 + 100;    //8λ�������255������λ���ֲ�����鷢�ͣ�����λ+100�����λ����
	c = a % 100;
	UART_send_string(b);
	Delay100us();
	UART_send_string(c);
}

//SMG����
void display(int a)
{      
	int SMG_dat[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f}; //���ֿ�
	int B[4];    			//��λ�����
	int i,j;
	
	P2M0=0xff;          //����
	P2M1=0x00;
	
	for(i=0;i<4;i++)
	{  //��a ���룬�������λ��
		B[i]=a%10;
		a=a/10;
	}
	P0=0xF7;  //��һλ��  
	for(j=0;j<4;j++)
	{
		P2=SMG_dat[B[j]];
		Delay100us();       //��Ӱ
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

/**************************�����������*******************************/
//�����������
int getKEY()
{
	uchar scanner[3]={0xFE,0xFD,0xFB}; //ɨ����
	int i=0;
	int x=0;  //������
	int y=0;  //������
	uchar ifPress;
	int keyNumber=0;
	x=0;
	y=0;
	for(i=0;i<3;i++)
	{
		x=i+1;
		P1=scanner[i];    	//ɨ���i��
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
		int key;    	//��������
		int number=0;	//��������
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
/*************************ͨ�Ų��֣����ģ�**********************/
//���ڳ�ʼ��
void UartInit()		//9600bps@24.000MHz
{
	PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xFB;		//���������ʷ�����ʱ��ΪFosc/12,��12T
	BRT = 0xF3;			//�趨���������ʷ�������װֵ
	AUXR |= 0x01;		//����1ѡ����������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		//�������������ʷ�����
	EA=1;
	ES=1;
}

//�����ַ���
void UART_send_string(int a)
{
	ES=0;

	SBUF=(unsigned char)a;
	while(!TI);
	TI=0;

	ES=1;
}

//����װ��
void UART_server() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		chr=SBUF;
		flag=1;
	}
}



