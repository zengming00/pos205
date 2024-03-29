/*********************************************************************************************************
*
* File                : syhstick.h
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

#ifndef __SYSTICK_H
#define __SYSTICK_H 

/* Includes ------------------------------------------------------------------*/	   
#include "stm32f2xx.h"

/* Private function prototypes -----------------------------------------------*/
void delay_init(void);
//void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

void delayms(uint16_t ms);

//void delay_1ms(uint32_t delay_conter);
//void delay_10us(uint32_t delay_conter);
//extern void TimingDelay_Decrement(void);

#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/






























