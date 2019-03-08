#include<intrins.h>

#define uchar unsigned char   	//�궨��

#define DS1302_SECOND_WRITE 0x80
#define DS1302_MINUTE_WRITE 0x82
#define DS1302_HOUR_WRITE 0x84
#define DS1302_WEEK_WRITE 0x8A
#define DS1302_DAY_WRITE 0x86
#define DS1302_MONTH_WRITE 0x88
#define DS1302_YEAR_WRITE 0x8C

//��������
sbit SCLK = P0^4;   			//����ʵʱʱ��ʱ�������� //
sbit SDA = P0^5;     		//����ʵʱʱ������������ //
sbit RST = P0^6;    		//����ʵʱʱ�Ӹ�λ������ //

//ʱ�����
char sec,min,hour,week,day,month,year;

//��������
uchar DS1302Read(uchar cmd);
void DS1302Write(uchar cmd, uchar dat);
void DS1302WriteByte(uchar dat);
uchar DS1302ReadByte();
void Init_DS1302();
void Delay2us();


/************************************************
*����: uchar DS1302Read()
*˵��: ��д��ַ���������
*����: ��cmd��Ӧ��ַ�ж�ȡһ���ֽڵ�����
*����: DS1302WriteByte(),DS1302ReadByte()
*����: cmd:Ҫд��Ŀ����ֽ�
*���: dat:��ȡ������
*************************************************/
uchar DS1302Read(uchar cmd){
	uchar dat;
	RST=0;								//��ʼCE����Ϊ0
	SCLK=0;								//��ʼʱ������Ϊ0
	RST=1;								//��ʼCE��Ϊ1�����俪ʼ
	DS1302WriteByte(cmd);	//���������֣�Ҫ��ȡ��ʱ��/������ַ
	dat=DS1302ReadByte();	//��ȡҪ�õ���ʱ��/����
	RST=0; 								//��ȡ������CE��Ϊ0���������ݵĴ���
	SCLK=1; 							//ʱ��������
	RST=1; 	
	return dat;						//���صõ���ʱ��/����
}
/************************************************
*����: DS1302Write
*˵��: ��д��ַ����д����
*����: ��cmd��Ӧ��ַ��дһ���ֽڵ�����
*����: DS1302WriteByte()
*����: cmd:Ҫд��Ŀ�����,dat:Ҫд�������
*���: ��
*************************************************/
void DS1302Write(uchar cmd, uchar dat){
	RST=0;								//��ʼCE����Ϊ0
	SCLK=0; 							//��ʼʱ������Ϊ0
	RST=1; 								//��ʼCE��Ϊ1�����俪ʼ
	DS1302WriteByte(cmd); //���������֣�Ҫд���ʱ��/������ַ
	DS1302WriteByte(dat); //д��Ҫ�޸ĵ�ʱ��/����
	RST=0; 								//��ȡ������CE��Ϊ0���������ݵĴ���
	SCLK=1; 							//ʱ��������
	RST=1; 	
}
/************************************************
*����: DS1302WriteByte
*˵��: ��
*����: д��8bit����
*����: delayus()
*����: dat:Ҫд�������
*���: ��
*************************************************/
void DS1302WriteByte(uchar dat){
	uchar i;
	SCLK=0;//��ʼʱ������Ϊ0
	Delay2us();
	for(i=0;i<8;i++)//��ʼ����8���ֽڵ�����
	{
		SDA=dat&0x01;//ȡ���λ��ע��DS1302�����ݺ͵�ַ���Ǵ����λ��ʼ�����
		Delay2us();
		SCLK=1;//ʱ�������ߣ����������أ�SDA�����ݱ�����
		Delay2us();
		SCLK=0;//ʱ�������ͣ�Ϊ��һ����������׼��
		dat>>=1;//��������һλ��׼��������һλ����
	}
}
/************************************************
*����: DS1302ReadByte()
*˵��: ��
*����: ��ȡ8bit������
*����: delayus()
*����: ��
*���: dat:��ȡ������
*************************************************/
uchar DS1302ReadByte(){
	uchar i,dat;
	Delay2us();
	for(i=0;i<8;i++)
	{
		dat>>=1;				//Ҫ���ص���������һλ
		if(SDA==1)			//��������Ϊ��ʱ��֤����λ����Ϊ1
			dat|=0x80;		//Ҫ�������ݵĵ�ǰֵ��Ϊ1,������,��Ϊ0
		SCLK=1;					//����ʱ����
		Delay2us();
		SCLK=0;					//�����½���
		Delay2us();
		
	}
return dat;//���ض�ȡ��������
}

/************************************************
*����: void Init_DS1302(void)
*˵��: ��1302д��һ����ʼ��ֵ
*����: д�����ڣ���ʱ�ӵ�ֵ
*����: DS1302Write����
*����: ��
*���: ��
*************************************************/
void Init_DS1302(){
	DS1302Write(0x8e,0x00);									//д������
	DS1302Write(DS1302_SECOND_WRITE,0x00); 	//��ʼ��ֵΪ0
	DS1302Write(DS1302_MINUTE_WRITE,0x20);	//��ʼ����ֵΪ0
	DS1302Write(DS1302_HOUR_WRITE,0x09); 		//��ʼΪ24Сʱģʽ��ʼʱ��Ϊ0��
	DS1302Write(DS1302_DAY_WRITE,0x25); 		//2011��1��1������6
	DS1302Write(DS1302_MONTH_WRITE,0x12);
	DS1302Write(DS1302_YEAR_WRITE,0x10);
	DS1302Write(DS1302_WEEK_WRITE,0x06);
	DS1302Write(0x90,0xA5); 								//���
	DS1302Write(0xc0,0xf0); 								//��ʼ��һ�α�ʾ
	DS1302Write(0x8e,0x80);
}
/**************************************
ȡ��ʱ�䣬����ת��Ϊʮ����
***************************************/
void get_time(){
	int a,b,c;
	a=DS1302Read(0x81);
	b=(a&0x70)>>4; //ʮλ
	c=a&0x0f;	//��λ
	sec=10*b+c;
	
	a=DS1302Read(0x83);
	b=(a&0x70)>>4; //ʮλ
	c=a&0x0f;	//��λ
	min=10*b+c;
	
	a=DS1302Read(0x85);
	b=(a&0x70)>>4; //ʮλ
	c=a&0x0f;	//��λ
	hour=10*b+c;
	
	a=DS1302Read(0x8B);
	week=a&0x07;

	a=DS1302Read(0x87);
	b=(a&0x70)>>4; //ʮλ
	c=a&0x0f;	//��λ
	day=10*b+c;
	
	a=DS1302Read(0x89);
	b=(a&0x70)>>4; //ʮλ
	c=a&0x0f;	//��λ
	month=10*b+c;
	
	a=DS1302Read(0x8D);
	b=(a&0x70)>>4; //ʮλ
	c=a&0x0f;	//��λ
	year=10*b+c;
}

//��ʱ����
void Delay2us()		//@24.000MHz
{
	unsigned char i;
	_nop_();
	_nop_();
	i = 9;
	while (--i);
}
