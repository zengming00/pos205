#include "24cxx.h" 
#include "systick.h"

//16ҳ16�ֽ� ��256�ֽ�


#define SDA_PIN    GPIO_Pin_8
#define SCL_PIN    GPIO_Pin_9

#define WP_PIN     GPIO_Pin_4

#define WP_ON()   GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define WP_OFF()   GPIO_ResetBits(GPIOA, GPIO_Pin_4)  //�͵�ƽ����д


//IO��������
void SDA_IN(){
//	GPIO_InitTypeDef  GPIO_InitStructure;
//  GPIO_InitStructure.GPIO_Pin = SDA_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void SDA_OUT(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

//IO��������	 
void IIC_SCL(int v){
	if(v){
		GPIO_SetBits(GPIOB, SCL_PIN);
	}else{
		GPIO_ResetBits(GPIOB, SCL_PIN);
	}
}
void IIC_SDA(int v){
	if(v){
		GPIO_SetBits(GPIOB, SDA_PIN);
	}else{
		GPIO_ResetBits(GPIOB, SDA_PIN);
	}
}
int READ_SDA(){
   return GPIO_ReadInputDataBit(GPIOB, SDA_PIN);
}

//��ʼ��IIC
void IIC_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE);
//Ҫ��©���
  GPIO_InitStructure.GPIO_Pin = SDA_PIN | SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	//��CLKΪ��ʱ,DATA �Ӹߵ��͸ı�
	IIC_SCL(0);
	SDA_OUT();     //sda�����
	IIC_SDA(1);	delay_us(5);
	
	IIC_SCL(1); delay_us(5);
 	IIC_SDA(0); delay_us(5);
	IIC_SCL(0); delay_us(5);
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	//STOP:when CLK is high DATA change form low to high
	IIC_SCL(0);
	SDA_OUT();//sda�����
	IIC_SDA(0); delay_us(5);
	
	IIC_SCL(1); delay_us(5);
	IIC_SDA(1);	delay_us(5);							   	
	IIC_SCL(0); delay_us(5);
}
//�ȴ�Ӧ���źŵ���(�ھ�λ�ɴӻ��ı�sda��
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ack=0;
	IIC_SCL(0);	 
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA(1);	
	IIC_SCL(1); delay_us(5);	 
	if(READ_SDA()){
		ack = 1;
	}else{
		ack = 0;
	}
	IIC_SCL(0); delay_us(5);	 
	return ack;  
} 
//����ACKӦ��
void IIC_Ack(u8 ack)
{
	IIC_SCL(0);
	SDA_OUT();
	if(ack){ //��Ӧ��
		IIC_SDA(1);
	}else{
		IIC_SDA(0);
	}
	delay_us(4);
	IIC_SCL(1);	delay_us(5);
	IIC_SCL(0);	delay_us(5);
}
					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
u8 IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	IIC_SCL(0);
	SDA_OUT(); 	    
    for(t=0;t<8;t++){
		if(txd & 0x80){
			IIC_SDA(1);
		}else{
			IIC_SDA(0);
		}
		delay_us(4);
        txd <<= 1;
		IIC_SCL(1); delay_us(5); //����
		
		IIC_SCL(0); //׼������
    }
	return IIC_Wait_Ack();	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(u8 ack)
{
	u8 i,receive=0;
	
	IIC_SCL(0);	delay_us(5);//ʱ�ӵ͵�ƽ���ڴ���4.7��s
	SDA_IN();//SDA����Ϊ����
    for(i=0; i<8; i++){
        IIC_SCL(1); delay_us(5); //������
        receive <<= 1;
        if(READ_SDA()){
			receive |= 1;   
		}
        IIC_SCL(0); delay_us(5); //׼������
    }					 
    IIC_Ack(ack);
    return receive;
}

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	IIC_Init();
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u8 ReadAddr)
{				  
	u8 temp=0,tmp;	
	WP_OFF();	  	    																 
    IIC_Start();  

	//����������ַ0XA0,��8λ0д���ݣ�1������
	tmp = IIC_Send_Byte(0XA0);	   //����д����
	tmp = IIC_Send_Byte(ReadAddr);//���͵�ַ
    
	IIC_Start();  	 	   
	tmp = IIC_Send_Byte(0XA1);           //�������ģʽ			   
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����
	WP_ON();
	if(0){
		return tmp;
	}	    
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u8 WriteAddr,u8 DataToWrite)
{
	WP_OFF(); 	    																 
    IIC_Start();  
	IIC_Send_Byte(0XA0);	    //����д����
	IIC_Send_Byte(WriteAddr);  //���͵�ַ
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
    IIC_Stop();//����һ��ֹͣ���� 
	WP_ON();
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 




/************************************24C02Ӳ���ӿ�******************************/ 
#define    SData  SDA_PIN   //P2^0;     //I2C  ʱ�� 
#define    SCLK  SCL_PIN    //P2^1;     //I2C  ���� 

/********************************�궨��*******************************************/
#define SCL(x)	x ? GPIO_SetBits(GPIOB , SCLK)  : GPIO_ResetBits(GPIOB , SCLK)
#define	SDA(x)  x ? GPIO_SetBits(GPIOB , SData) : GPIO_ResetBits(GPIOB , SData)
    
/********************************����*******************************************/
u8 ack;

//��ʼ��IIC
void IIC2_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  
  //SDAҪ��©���
  GPIO_InitStructure.GPIO_Pin = SDA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 	WP_OFF();	  	    																 

}
/*******************************************************************
                     �����ߺ���               
����ԭ��: void  Start_I2c();  
����:     ����I2C����,������I2C��ʼ����.  
********************************************************************/
void Start_I2c()
{
  
  SDA(1);  //SDA=1; ������ʼ�����������ź�
  delay_us(1);
  SCL(1);	  //SCL=1;
  delay_us(5);      //��ʼ��������ʱ�����4.7us,��ʱ
  
  SDA(0);  //SDA=0;           /*������ʼ�ź�*/
  delay_us(5);      // ��ʼ��������ʱ�����4��s
  SCL(0);	  //SCL=0;       /*ǯסI2C���ߣ�׼�����ͻ�������� */
  delay_us(2);
}

/*******************************************************************
                      �������ߺ���               
����ԭ��: void  Stop_I2c();  
����:     ����I2C����,������I2C��������.  
********************************************************************/
void Stop_I2c()
{
  
  SDA(0);  //SDA=0;   //���ͽ��������������ź�
  delay_us(1);      //���ͽ���������ʱ���ź�
  SCL(1)	; //SCL=1; ������������ʱ�����4��s
  delay_us(5);
  SDA(0);  //SDA=1; ����I2C���߽����ź�
  delay_us(4);
}

/*******************************************************************
                 �ֽ����ݷ��ͺ���               
����ԭ��: void  SendByte(UCHAR c);
����:     ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
          ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0)     
           ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************/
void  SendByte(unsigned char  c)
{
 unsigned char  BitCnt;
 
 for(BitCnt=0;BitCnt<8;BitCnt++)  //Ҫ���͵����ݳ���Ϊ8λ
    {
     if((c<<BitCnt)&0x80) SDA(1);  //SDA=1; �жϷ���λ
     else SDA(0); // SDA=0                
     delay_us(1);
     SCL(1);      //SCL=1 ��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
	 delay_us(5);  //��֤ʱ�Ӹߵ�ƽ���ڴ���4��s
     SCL(0);	     //SCL = 0
    }
    
    delay_us(2);
    SDA(1);   //SDA=1 λ��������ͷ������ߣ�׼������Ӧ��λ
	delay_us(2);   
   	SCL(1);   //SCL=1
    delay_us(3);
    if(GPIO_ReadInputDataBit(GPIOB ,SData)==1)ack=0;     
    else ack=1;     //�ж��Ƿ���յ�Ӧ���ź�
  	SCL(0);   //SCL=0;
    delay_us(3);
}

/*******************************************************************
                 �ֽ����ݽ��պ���               
����ԭ��: UCHAR  RcvByte();
����:        �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
          ���������Ӧ����Ӧ��ӻ���  
********************************************************************/    
unsigned char   RcvByte()
{
  unsigned char  retc;
  unsigned char  BitCnt;
  
  retc=0; 
  SDA(1); //SDA=1 ��������Ϊ���뷽ʽ
  for(BitCnt=0;BitCnt<8;BitCnt++)
      {
        delay_us(1);           
        SCL(0);    //SCL=0 ��ʱ����Ϊ�ͣ�׼����������λ
		delay_us(5); //ʱ�ӵ͵�ƽ���ڴ���4.7��s
     	
		SCL(1);	 //SCL=1 ��ʱ����Ϊ��ʹ��������������Ч
        delay_us(2);
        retc=retc<<1;
        if(GPIO_ReadInputDataBit(GPIOB , SData)== 1)retc=retc+1; // SDA == 1������λ,���յ�����λ����retc�� 
        delay_us(2);
      }
  SCL(0); // SCL=0;
  delay_us(2);
  return(retc);
}

/********************************************************************
                     Ӧ���Ӻ���
����ԭ��:  void Ack_I2c(bit a);
����:      ����������Ӧ���ź�(������Ӧ����Ӧ���źţ���λ����a����)
********************************************************************/
void Ack_I2c(u8 a)
{
 
  if(a==0)
   
   SDA(0);  //SDA=0;�ڴ˷���Ӧ����Ӧ���ź� 
  else 
   SDA(1);  //SDA=1;
  delay_us(3);    
 
  SCL(1);   //SCL=1;	 
  delay_us(4);	//ʱ�ӵ͵�ƽ���ڴ���4��s
 
  SCL(0)	;   //SCL=0; ��ʱ���ߣ�ǯסI2C�����Ա��������
  delay_us(2); 
 
}



//AT2402�Ĺ��ܺ���
/*******************************************************************
                    �����ӵ�ַ�������Ͷ��ֽ����ݺ���               
����ԭ��: bit  ISendStr(UCHAR sla,UCHAR suba,ucahr *s,UCHAR no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba������������sָ������ݣ�����no���ֽڡ�
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
u8 ISendStr(unsigned char  sla,unsigned char  suba,unsigned char  *s,unsigned char  no)
{
   unsigned char i;

   Start_I2c();               /*��������*/
   SendByte(sla);             /*����������ַ*/
   if(ack==0)return(0);
   SendByte(suba);            /*���������ӵ�ַ*/
   if(ack==0)return(0);

   for(i=0;i<no;i++)
   {   
     SendByte(*s);            /*��������*/
     if(ack==0)return(0);
     s++;
   } 
   Stop_I2c();                /*��������*/ 

   return(1);
}

/*******************************************************************
                    �����ӵ�ַ������ȡ���ֽ����ݺ���               
����ԭ��: bit  RecndStr(UCHAR sla,UCHAR suba,ucahr *s,UCHAR no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,�����ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba�����������ݷ���sָ��Ĵ洢������no���ֽڡ�
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
u8 IRcvStr(unsigned char  sla,unsigned char  suba,unsigned char  *s,unsigned char  no)
{
   unsigned char i;

   Start_I2c();                  /*��������*/
   SendByte(sla);                /*����������ַ*/
   if(ack==0)return(0);
   SendByte(suba);               /*���������ӵ�ַ*/
   if(ack==0)return(0);

   Start_I2c();                 /*������������*/
   SendByte(sla+1);
   if(ack==0)return(0);
   for(i=0;i<no-1;i++)
   {   
     *s=RcvByte();               /*��������*/
      Ack_I2c(0);                /*���;ʹ�λ*/  
     s++;
   } 
   *s=RcvByte();
   Ack_I2c(1);                   /*���ͷ�Ӧλ*/
   Stop_I2c();                   /*��������*/ 
   return(1);
}











