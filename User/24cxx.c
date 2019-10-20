#include "24cxx.h" 
#include "systick.h"

//16页16字节 共256字节


#define SDA_PIN    GPIO_Pin_8
#define SCL_PIN    GPIO_Pin_9

#define WP_PIN     GPIO_Pin_4

#define WP_ON()   GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define WP_OFF()   GPIO_ResetBits(GPIOA, GPIO_Pin_4)  //低电平开启写


//IO方向设置
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

//IO操作函数	 
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

//初始化IIC
void IIC_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE);
//要求开漏输出
  GPIO_InitStructure.GPIO_Pin = SDA_PIN | SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//产生IIC起始信号
void IIC_Start(void)
{
	//当CLK为高时,DATA 从高到低改变
	IIC_SCL(0);
	SDA_OUT();     //sda线输出
	IIC_SDA(1);	delay_us(5);
	
	IIC_SCL(1); delay_us(5);
 	IIC_SDA(0); delay_us(5);
	IIC_SCL(0); delay_us(5);
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	//STOP:when CLK is high DATA change form low to high
	IIC_SCL(0);
	SDA_OUT();//sda线输出
	IIC_SDA(0); delay_us(5);
	
	IIC_SCL(1); delay_us(5);
	IIC_SDA(1);	delay_us(5);							   	
	IIC_SCL(0); delay_us(5);
}
//等待应答信号到来(第九位由从机改变sda）
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ack=0;
	IIC_SCL(0);	 
	SDA_IN();      //SDA设置为输入  
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
//产生ACK应答
void IIC_Ack(u8 ack)
{
	IIC_SCL(0);
	SDA_OUT();
	if(ack){ //不应答
		IIC_SDA(1);
	}else{
		IIC_SDA(0);
	}
	delay_us(4);
	IIC_SCL(1);	delay_us(5);
	IIC_SCL(0);	delay_us(5);
}
					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
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
		IIC_SCL(1); delay_us(5); //发送
		
		IIC_SCL(0); //准备数据
    }
	return IIC_Wait_Ack();	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(u8 ack)
{
	u8 i,receive=0;
	
	IIC_SCL(0);	delay_us(5);//时钟低电平周期大于4.7μs
	SDA_IN();//SDA设置为输入
    for(i=0; i<8; i++){
        IIC_SCL(1); delay_us(5); //读数据
        receive <<= 1;
        if(READ_SDA()){
			receive |= 1;   
		}
        IIC_SCL(0); delay_us(5); //准备数据
    }					 
    IIC_Ack(ack);
    return receive;
}

//初始化IIC接口
void AT24CXX_Init(void)
{
	IIC_Init();
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u8 ReadAddr)
{				  
	u8 temp=0,tmp;	
	WP_OFF();	  	    																 
    IIC_Start();  

	//发送器件地址0XA0,第8位0写数据，1读数据
	tmp = IIC_Send_Byte(0XA0);	   //发送写命令
	tmp = IIC_Send_Byte(ReadAddr);//发送地址
    
	IIC_Start();  	 	   
	tmp = IIC_Send_Byte(0XA1);           //进入接收模式			   
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件
	WP_ON();
	if(0){
		return tmp;
	}	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u8 WriteAddr,u8 DataToWrite)
{
	WP_OFF(); 	    																 
    IIC_Start();  
	IIC_Send_Byte(0XA0);	    //发送写命令
	IIC_Send_Byte(WriteAddr);  //发送地址
	IIC_Send_Byte(DataToWrite);     //发送字节							   
    IIC_Stop();//产生一个停止条件 
	WP_ON();
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
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
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 




/************************************24C02硬件接口******************************/ 
#define    SData  SDA_PIN   //P2^0;     //I2C  时钟 
#define    SCLK  SCL_PIN    //P2^1;     //I2C  数据 

/********************************宏定义*******************************************/
#define SCL(x)	x ? GPIO_SetBits(GPIOB , SCLK)  : GPIO_ResetBits(GPIOB , SCLK)
#define	SDA(x)  x ? GPIO_SetBits(GPIOB , SData) : GPIO_ResetBits(GPIOB , SData)
    
/********************************变量*******************************************/
u8 ack;

//初始化IIC
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

  
  //SDA要求开漏输出
  GPIO_InitStructure.GPIO_Pin = SDA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 	WP_OFF();	  	    																 

}
/*******************************************************************
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:     启动I2C总线,即发送I2C起始条件.  
********************************************************************/
void Start_I2c()
{
  
  SDA(1);  //SDA=1; 发送起始条件的数据信号
  delay_us(1);
  SCL(1);	  //SCL=1;
  delay_us(5);      //起始条件建立时间大于4.7us,延时
  
  SDA(0);  //SDA=0;           /*发送起始信号*/
  delay_us(5);      // 起始条件锁定时间大于4μs
  SCL(0);	  //SCL=0;       /*钳住I2C总线，准备发送或接收数据 */
  delay_us(2);
}

/*******************************************************************
                      结束总线函数               
函数原型: void  Stop_I2c();  
功能:     结束I2C总线,即发送I2C结束条件.  
********************************************************************/
void Stop_I2c()
{
  
  SDA(0);  //SDA=0;   //发送结束条件的数据信号
  delay_us(1);      //发送结束条件的时钟信号
  SCL(1)	; //SCL=1; 结束条件建立时间大于4μs
  delay_us(5);
  SDA(0);  //SDA=1; 发送I2C总线结束信号
  delay_us(4);
}

/*******************************************************************
                 字节数据发送函数               
函数原型: void  SendByte(UCHAR c);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
          此状态位进行操作.(不应答或非应答都使ack=0)     
           发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void  SendByte(unsigned char  c)
{
 unsigned char  BitCnt;
 
 for(BitCnt=0;BitCnt<8;BitCnt++)  //要传送的数据长度为8位
    {
     if((c<<BitCnt)&0x80) SDA(1);  //SDA=1; 判断发送位
     else SDA(0); // SDA=0                
     delay_us(1);
     SCL(1);      //SCL=1 置时钟线为高，通知被控器开始接收数据位
	 delay_us(5);  //保证时钟高电平周期大于4μs
     SCL(0);	     //SCL = 0
    }
    
    delay_us(2);
    SDA(1);   //SDA=1 位发送完后释放数据线，准备接收应答位
	delay_us(2);   
   	SCL(1);   //SCL=1
    delay_us(3);
    if(GPIO_ReadInputDataBit(GPIOB ,SData)==1)ack=0;     
    else ack=1;     //判断是否接收到应答信号
  	SCL(0);   //SCL=0;
    delay_us(3);
}

/*******************************************************************
                 字节数据接收函数               
函数原型: UCHAR  RcvByte();
功能:        用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
          发完后请用应答函数应答从机。  
********************************************************************/    
unsigned char   RcvByte()
{
  unsigned char  retc;
  unsigned char  BitCnt;
  
  retc=0; 
  SDA(1); //SDA=1 置数据线为输入方式
  for(BitCnt=0;BitCnt<8;BitCnt++)
      {
        delay_us(1);           
        SCL(0);    //SCL=0 置时钟线为低，准备接收数据位
		delay_us(5); //时钟低电平周期大于4.7μs
     	
		SCL(1);	 //SCL=1 置时钟线为高使数据线上数据有效
        delay_us(2);
        retc=retc<<1;
        if(GPIO_ReadInputDataBit(GPIOB , SData)== 1)retc=retc+1; // SDA == 1读数据位,接收的数据位放入retc中 
        delay_us(2);
      }
  SCL(0); // SCL=0;
  delay_us(2);
  return(retc);
}

/********************************************************************
                     应答子函数
函数原型:  void Ack_I2c(bit a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void Ack_I2c(u8 a)
{
 
  if(a==0)
   
   SDA(0);  //SDA=0;在此发出应答或非应答信号 
  else 
   SDA(1);  //SDA=1;
  delay_us(3);    
 
  SCL(1);   //SCL=1;	 
  delay_us(4);	//时钟低电平周期大于4μs
 
  SCL(0)	;   //SCL=0; 清时钟线，钳住I2C总线以便继续接收
  delay_us(2); 
 
}



//AT2402的功能函数
/*******************************************************************
                    向有子地址器件发送多字节数据函数               
函数原型: bit  ISendStr(UCHAR sla,UCHAR suba,ucahr *s,UCHAR no);  
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，子地址suba，发送内容是s指向的内容，发送no个字节。
          如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
u8 ISendStr(unsigned char  sla,unsigned char  suba,unsigned char  *s,unsigned char  no)
{
   unsigned char i;

   Start_I2c();               /*启动总线*/
   SendByte(sla);             /*发送器件地址*/
   if(ack==0)return(0);
   SendByte(suba);            /*发送器件子地址*/
   if(ack==0)return(0);

   for(i=0;i<no;i++)
   {   
     SendByte(*s);            /*发送数据*/
     if(ack==0)return(0);
     s++;
   } 
   Stop_I2c();                /*结束总线*/ 

   return(1);
}

/*******************************************************************
                    向有子地址器件读取多字节数据函数               
函数原型: bit  RecndStr(UCHAR sla,UCHAR suba,ucahr *s,UCHAR no);  
功能:     从启动总线到发送地址，子地址,读数据，结束总线的全过程,从器件
          地址sla，子地址suba，读出的内容放入s指向的存储区，读no个字节。
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
u8 IRcvStr(unsigned char  sla,unsigned char  suba,unsigned char  *s,unsigned char  no)
{
   unsigned char i;

   Start_I2c();                  /*启动总线*/
   SendByte(sla);                /*发送器件地址*/
   if(ack==0)return(0);
   SendByte(suba);               /*发送器件子地址*/
   if(ack==0)return(0);

   Start_I2c();                 /*重新启动总线*/
   SendByte(sla+1);
   if(ack==0)return(0);
   for(i=0;i<no-1;i++)
   {   
     *s=RcvByte();               /*发送数据*/
      Ack_I2c(0);                /*发送就答位*/  
     s++;
   } 
   *s=RcvByte();
   Ack_I2c(1);                   /*发送非应位*/
   Stop_I2c();                   /*结束总线*/ 
   return(1);
}











