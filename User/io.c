#include "io.h"
/*
PA7蓝色
PA5黄色
PA6红色
液晶背光PC15
*/
void LedInit(){
	IOInit(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_7);
	IOInit(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_6);
	IOInit(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_5);
	
	IOInit(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_15);
}

void LedRed(int v){
	IO(GPIOA, GPIO_Pin_6, !v);//低电平点亮
}

void LedBlue(int v){
	IO(GPIOA, GPIO_Pin_7, !v);
}

void LedYellow(int v){
	IO(GPIOA, GPIO_Pin_5, !v);
}

void LedLcd(int v){
	IO(GPIOC, GPIO_Pin_15, !v);
}


void IOInit(uint32_t RCC_AHB1Periph, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph, ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void IO(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, int v)
{
	if(v){
		GPIOx->BSRRL = GPIO_Pin;
	} else {
		GPIOx->BSRRH = GPIO_Pin; 
	}
}

void IOToggle(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
  GPIOx->ODR ^= GPIO_Pin;
}

void gpioOut(uint32_t RCC_AHB1Periph, GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, char v){
	IOInit(RCC_AHB1Periph, GPIOx, GPIO_Pin);
	IO(GPIOx, GPIO_Pin, v);
}

////////////////////////////////////////////////////////////////////////////

void IO_OUT_PULL_UP(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOx, &GPIO_InitStructure);	
}

void IO_OUT_FLOATING(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void IO_IN_FLOATING(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOx, &GPIO_InitStructure);	
}

void IO_IN_PULL_UP(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOx, &GPIO_InitStructure);

}
