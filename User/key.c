#include <stm32f2xx.h>
#include "stdio.h"
#include "systick.h"


void keyInit(void){

  /* Enable the BUTTON Clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); //�رյ��� �˿�����ӳ��  ʹ�÷���������ʱ�������ô���
}

int keyScan(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	int key;
	char a,b,c,d;
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//��Դ��
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) {
		delayms(10);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) {
			return 10;
		}
	}
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	a = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12);
	b = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);
	c = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);
	d = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);
	
	
//printf("a:%d,b:%d,c:%d,d:%d\r\n",a,b,c,d);
//return -1;
		
	if ((a+b+c+d) != 4) { //�жϰ����Ƿ��� �����ť���� ���������е�һ���˿�
		if (!a)	  //������
			key = 0;
		else if (!b)
			key = 4;
		else if (!c)
			key = 8;
		else if (!d)
			key = 12;
		else
			key = -1;


		//��1
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		//��0
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOC, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12);
		GPIO_ResetBits(GPIOA, GPIO_Pin_15);

	//	delay_ms(10);			 //��ʱ


		a = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
		b = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
		c = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
		d = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);


//printf("a:%d,b:%d,c:%d,d:%d\r\n",a,b,c,d);

		if (!a)		  //������
			; //key = key + 0;
		else if (!b)
			key = key + 1;
		else if (!c)
			key = key + 2;
		else if (!d)
			key = key + 3;
		else
			key = -1;

		return key;
	}
	return -1;
}

