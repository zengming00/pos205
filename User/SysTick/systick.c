/*********************************************************************************************************
*
* File                : systick.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "systick.h"
#include "stm32f10x_systick.h"

/* Private variables ---------------------------------------------------------*/	 
static uint8_t  delay_fac_us = 0;
static uint16_t delay_fac_ms = 0;
static FlagStatus  Status;


/*******************************************************************************
* Function Name  : delay_init
* Description    :  
* Input          : None
* Output         : None
* Return         : None
* Attention		 : 
*******************************************************************************/
void delay_init(void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;

    RCC_GetClocksFreq(&RCC_ClocksStatus);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick_ITConfig(DISABLE);
    delay_fac_us = RCC_ClocksStatus.HCLK_Frequency / 8000000;
    delay_fac_ms = RCC_ClocksStatus.HCLK_Frequency / 8000;      
}
					
/*******************************************************************************
* Function Name  : delay_us
* Description    : 
* Input          : - Nus:
* Output         : None
* Return         : None
* Attention		 : 
*******************************************************************************/            
void delay_us(u32 Nus)
{ 
    SysTick_SetReload(delay_fac_us * Nus);
    SysTick_CounterCmd(SysTick_Counter_Clear);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    do
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);
    SysTick_CounterCmd(SysTick_Counter_Disable);
	SysTick_CounterCmd(SysTick_Counter_Clear);
}


/*******************************************************************************
* Function Name  : delay_ms
* Description    : 
* Input          : - nms:
* Output         : None
* Return         : None
* Attention		 : 
*******************************************************************************/  
void _delay_ms(uint16_t nms)
{    
    uint32_t temp = delay_fac_ms * nms;

    if (temp > 0x00ffffff)
    {
        temp = 0x00ffffff;
    }
    SysTick_SetReload(temp);
    SysTick_CounterCmd(SysTick_Counter_Clear);
    SysTick_CounterCmd(SysTick_Counter_Enable);
    do
    {
        Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
    }while (Status != SET);
    SysTick_CounterCmd(SysTick_Counter_Disable);
	SysTick_CounterCmd(SysTick_Counter_Clear);
}

void delayms(uint16_t ms){
	int i;
	for(i=0; i<ms; i++){
		delay_us(1000);
	}
}
////////////////////////////////////////////////////////////////////////////

//__IO static uint32_t TimingDelay;
//
//void delay_1ms(uint32_t delay_conter)
//{
//	SysTick_Config((uint32_t)9000);
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
//
//	TimingDelay = delay_conter;
//	while(TimingDelay!=0);
//}
//
//void delay_10us(uint32_t delay_conter)
//{ 
//	SysTick_Config((uint32_t)90);
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
//
//	TimingDelay = delay_conter;
//	while(TimingDelay!=0);
//}
//
//void TimingDelay_Decrement(void)
//{
//  if (TimingDelay != 0x00)
//  { 
//    TimingDelay--;
//  }
//}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
















