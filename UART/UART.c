#include<STC12C5A.h>

char chr;
int flag;
char i;

void UartInit();
void UART_send_string(unsigned char *p);
void Delay();

void main ()
{
	flag=1;
	P2M0=0xFF;
	P2M1=0x00;
	P2=0xFF;
	UartInit();
	
	while(1)
	{
		if(flag==1)
		{
			flag=0;
			if(chr=='a')
			{
				P0=0x00;
				for(i=0;i<100;i++)
				{
					Delay();
				}
				P0=0xFF;
			}
			else
			{
				P0=0xFF;
			}
			UART_send_string("OK!\n");
		}
	}
}

void UartInit()		//9600bps@24.000MHz
{
	PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0x64;		//�趨��ʱ��ֵ
	TH1 = 0x64;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1		
	EA=1;
	ES=1;
}

void UART_send_string(unsigned char *p)
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
	if(RI==1)
	{
		RI=0;
		chr=SBUF;
		flag=1;
	}
}

void Delay()		//@24.000MHz
{
	unsigned char i, j, k;

	i = 1;
	j = 234;
	k = 113;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}



