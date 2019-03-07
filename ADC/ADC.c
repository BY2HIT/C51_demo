/***���ڵ�ѹ��***/
//ͨ������1����'a','b'����'c'
//�ֱ𷵻�P1.0,P1.1,P1.7�ɼ��õ��ĵ�ѹֵ

#include <stc12c5a.h>
unsigned char chr;
unsigned char flag;
void UART_init()	  //���ڳ�ʼ��
{
	PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR &= 0xFB;		//���������ʷ�����ʱ��ΪFosc/12,��12T
	BRT = 0xF3;		//�趨���������ʷ�������װֵ
	AUXR |= 0x01;		//����1ѡ����������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		//�������������ʷ�����
	EA=1;ES=1;			//�������ж�
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
	AUXR1 &=0xfb;			// ADRJ=0 ,��ȡģʽ
	P1ASF=0X83;     //����P1.7��P1.1��P1.0ΪADģʽ����ΪADͨ��1000 0011��
	ADC_CONTR |= 0x80;  //��AD��Դ
	Delay500ms();       //�ȴ�һ��ʱ����AD��Դ�ȶ�
	ADC_CONTR |= 0x60;   //��������ٶ�ת��
	ADC_CONTR &=0xef; //����ADC�ж� ADC_FLAG = 0;            
}
void Init_P1_x(char channel)
{
	ADC_CONTR &=0xef; //����ADC�ж� ADC_FLAG = 0;
	ADC_CONTR &= 0XF8; //����ͨ��x��P1.x����ΪAD����
	ADC_CONTR |= channel;  //����ͨ��7��P1.x����ΪAD����
	ADC_CONTR |= 0X08;  //��ADCת��һ��
	while(ADC_CONTR & 0X10 != 0);   //�ȴ�ADת�����
}
int Get_AD_date()
{
	unsigned int t1,t2,date;
	t1=ADC_RES;	//����λ
	t2=ADC_RESL;	 //�Ͱ�λ
	date=(t1<<2)+t2;
	return date;
}
void uart_print1(unsigned int V0,unsigned int V1,unsigned int V7)//��������
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

/*void uart_print(int V0,int V1,int V7)//ԭʼ����
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


