#ifndef __MY_IO_H
#define __MY_IO_H

#include <stm32f2xx.h>

//Êä³ö
void IOInit(uint32_t RCC_AHB1Periph, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
void IO(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, int v);
void IOToggle(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);

void gpioOut(uint32_t RCC_AHB1Periph, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, char v);



void LedInit(void);

void LedRed(int v);

void LedBlue(int v);

void LedYellow(int v);

void LedLcd(int v);




void IO_OUT_PULL_UP(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
void IO_OUT_FLOATING(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
void IO_IN_FLOATING(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
void IO_IN_PULL_UP(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);











#endif

