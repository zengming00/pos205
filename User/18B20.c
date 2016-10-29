#include "18B20.h"
#include "systick.h"

/*  
    u8 *p,id[8];
    u16 i,tmp;
  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    Onewire_Enable_GPIO_Port();
	p=readID();
	while(*p){id[i]=*p;p++;i++;}
	printf("DS18B20's ID :");
	i=0;
	while(i!=8)
		{printf("0x%x ",id[i]);i++;}
	printf("\r\n");
  //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
// 		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 
	while(1)
	{
		if(tmp != i)
		{
			printf("Temperture:%d.%dC\r\n ",i/100,i%100);
			tmp=i;
		}  	
 		i = readTemp();
		delay_1ms(500);//²»Õý³££¡
	}
*/

//pb12
#define ONEWIRE_RCC         RCC_AHB1Periph_GPIOB
#define ONEWIRE_GPIO        GPIOB
#define ONEWIRE_GPIO_PIN    GPIO_Pin_12

#define SET_DQ() 		GPIO_SetBits(ONEWIRE_GPIO,ONEWIRE_GPIO_PIN)
#define CLR_DQ() 		GPIO_ResetBits(ONEWIRE_GPIO,ONEWIRE_GPIO_PIN)
#define GET_DQ() 		GPIO_ReadInputDataBit(ONEWIRE_GPIO,ONEWIRE_GPIO_PIN)

void Onewire_Enable_GPIO_Port(void);
void Onewire_OUT_PULL_UP(void);
void Onewire_OUT_FLOATING(void);
void Onewire_IN_FLOATING(void);
void Onewire_IN_PULL_UP(void);
void _delay_us(u8 us);
void resetOnewire(void) ;
u8 rOnewire(void);
void wOnewire(u8 data);

void convertDs18b20(void) 
{ 
    resetOnewire(); 
    wOnewire(0xcc); 
    wOnewire(0x44); 
}


u8* readID(void) 
{ 
	u8 ID[8],i,*p;
	resetOnewire(); 
	wOnewire(0x33);
	for(i=0;i<8;i++)
	{ID[i]=rOnewire();}
	p=ID;
	return p;
}


u16 readTemp(void) 
{ 
	u8 temp1,temp2;
	convertDs18b20();
	resetOnewire(); 
	wOnewire(0xcc); 
	wOnewire(0xbe); 	
	temp1=rOnewire(); 
	temp2=rOnewire(); 
	return (((temp2<<8)|temp1)*6.25); //0.0625=xx, 0.625=xx.x, 6.25=xx.xx
}



void Onewire_Enable_GPIO_Port(void)
{
  RCC_AHB1PeriphClockCmd( ONEWIRE_RCC,ENABLE);
}
//onewire->PE3

void Onewire_OUT_PULL_UP(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = ONEWIRE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ONEWIRE_GPIO, &GPIO_InitStructure);	
}

void Onewire_OUT_FLOATING(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = ONEWIRE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ONEWIRE_GPIO, &GPIO_InitStructure);
}

void Onewire_IN_FLOATING(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = ONEWIRE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ONEWIRE_GPIO, &GPIO_InitStructure);	
}

void Onewire_IN_PULL_UP(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = ONEWIRE_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ONEWIRE_GPIO, &GPIO_InitStructure);

}


/* Reset */
void resetOnewire(void) 
{
	Onewire_OUT_PULL_UP();
	CLR_DQ();
	delay_us(450);
	SET_DQ();
	delay_us(60);
	Onewire_IN_PULL_UP(); 
	delay_us(10);
	while(!(GET_DQ()));
	Onewire_OUT_PULL_UP();
	SET_DQ();
}
/* Read */
u8 rOnewire(void)
{
    u8 data=0,i=0;
    for(i=0;i<8;i++)
    {
		Onewire_OUT_PULL_UP();
		CLR_DQ();
		data=data>>1;
		SET_DQ();
		Onewire_IN_PULL_UP();
		delay_us(8);
		if(GET_DQ())data|=0x80;
		Onewire_OUT_PULL_UP();
		SET_DQ();
		delay_us(60);
    }
    return(data);
}

/* Write */
void wOnewire(u8 data)
{
	u8 i=0;
	Onewire_OUT_PULL_UP();
	SET_DQ();
	delay_us(16);
    	for(i=0;i<8;i++)
    	{
		CLR_DQ();
		if(data&0x01)
		{
			SET_DQ();
		}
   	 	else
   	 	{
			CLR_DQ();
    	}
    	data=data>>1;
		delay_us(40);  //65
		SET_DQ();
    }
}


