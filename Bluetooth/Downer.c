#include<STC12C5A.h>

//����ȫ�ֱ���
int chr;   //����
char flag;	//���ձ�־

//��������
void display(int a);
void UartInit();
void UART_send_string(int *p);
void Delay100us();

//������
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

/*************************���������**************************************/
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

/*******************ͨ�Ų��֣����ģ�*************************************/
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



