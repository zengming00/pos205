#include <stdio.h>
#include <stm32f2xx.h>
#include "systick.h"
#include "fmc.h"
#include "ir.h"

#define ENABLE_IR_RECV

// ң����dump�����Ĵ���
// uint16_t rawDataOn2[227] = {3800, 1488, 516, 1260, 516, 1260, 516, 526, 516, 528, 514, 526, 516, 1260, 516, 526, 516, 526, 516, 1260, 516, 1260, 514, 526, 516, 1260, 516, 528, 514, 526, 516, 1260, 516, 1260, 516, 526, 516, 1260, 516, 1260, 516, 526, 516, 526, 516, 1260, 516, 526, 516, 526, 516, 1260, 516, 526, 516, 526, 516, 526, 516, 526, 516, 526, 516, 526, 514, 526, 516, 526, 516, 526, 516, 526, 516, 526, 516, 524, 516, 526, 516, 526, 516, 526, 516, 526, 516, 526, 516, 1260, 516, 524, 516, 526, 514, 1260, 516, 524, 516, 526, 514, 1260, 516, 1260, 514, 528, 514, 526, 514, 526, 516, 524, 516, 524, 516, 526, 516, 1260, 516, 526, 516, 1258, 516, 526, 516, 526, 516, 526, 516, 526, 516, 526, 516, 526, 516, 1258, 516, 526, 516, 1258, 516, 526, 516, 526, 516, 526, 516, 526, 514, 524, 516, 526, 514, 526, 514, 526, 516, 526, 516, 526, 516, 526, 514, 526, 516, 526, 514, 526, 516, 526, 516, 524, 516, 526, 514, 526, 516, 526, 516, 526, 514, 1258, 516, 526, 514, 526, 514, 526, 516, 526, 516, 524, 516, 526, 516, 526, 516, 524, 516, 524, 516, 1258, 516, 526, 516, 526, 516, 526, 516, 526, 514, 526, 516, 526, 516, 526, 516, 524, 516, 526, 514, 1260, 514, 526, 516, 1258, 516, 526, 516};  // UNKNOWN 5795BA90
// uint16_t rawDataOff2[227] = {3804, 1464, 518, 1258, 516, 1258, 518, 524, 518, 522, 518, 524, 516, 1258, 516, 526, 516, 524, 518, 1256, 516, 1260, 516, 526, 516, 1258, 516, 524, 518, 524, 518, 1258, 516, 1258, 516, 526, 516, 1258, 516, 1258, 516, 524, 516, 524, 518, 1258, 516, 524, 516, 524, 516, 1258, 516, 526, 516, 524, 518, 524, 516, 524, 516, 524, 516, 526, 516, 524, 516, 524, 518, 522, 518, 524, 516, 524, 518, 524, 518, 524, 518, 524, 518, 524, 518, 524, 516, 526, 516, 524, 518, 522, 518, 524, 516, 1258, 516, 524, 518, 524, 518, 1258, 516, 1258, 518, 524, 516, 524, 518, 522, 516, 524, 516, 526, 516, 524, 516, 1258, 516, 524, 502, 1272, 518, 526, 516, 524, 516, 524, 518, 524, 518, 524, 516, 524, 518, 1256, 516, 524, 496, 1280, 516, 524, 518, 524, 494, 546, 516, 526, 518, 524, 518, 524, 518, 524, 518, 524, 516, 524, 518, 524, 518, 524, 516, 524, 516, 524, 516, 524, 516, 526, 516, 524, 516, 524, 516, 524, 516, 524, 518, 524, 516, 1258, 516, 524, 518, 524, 518, 524, 516, 524, 518, 524, 518, 524, 518, 524, 516, 526, 516, 524, 520, 1256, 518, 524, 518, 524, 516, 524, 518, 522, 518, 524, 518, 524, 516, 526, 516, 1260, 518, 1258, 518, 524, 516, 524, 518, 1258, 518, 524, 516}; // UNKNOWN EF62489E

/* ������������ */
// u16 PulseTab[PULSETABSIZE] = {9000, 4500, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1680, 560, 560, 560, 1680, 560, 1680, 560, 1680, 560, 1680, 560, 1680, 560, 1680, 560, 560, 560, 1680, 560, 1680, 560, 1680, 560, 560, 560, 1680, 560, 1680, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 560, 1680, 560, 560, 560, 560, 560, 1680, 560, 1680, 560, 1680, 480, 38924, 9033, 2291, 510};
// on
u16 PulseTab[PULSETABSIZE] = {3800, 1488, 516, 1260, 516, 1260, 516, 526, 516, 528, 514, 526, 516, 1260, 516, 526, 516, 526, 516, 1260, 516, 1260, 514, 526, 516, 1260, 516, 528, 514, 526, 516, 1260, 516, 1260, 516, 526, 516, 1260, 516, 1260, 516, 526, 516, 526, 516, 1260, 516, 526, 516, 526, 516, 1260, 516, 526, 516, 526, 516, 526, 516, 526, 516, 526, 516, 526, 514, 526, 516, 526, 516, 526, 516, 526, 516, 526, 516, 524, 516, 526, 516, 526, 516, 526, 516, 526, 516, 526, 516, 1260, 516, 524, 516, 526, 514, 1260, 516, 524, 516, 526, 514, 1260, 516, 1260, 514, 528, 514, 526, 514, 526, 516, 524, 516, 524, 516, 526, 516, 1260, 516, 526, 516, 1258, 516, 526, 516, 526, 516, 526, 516, 526, 516, 526, 516, 526, 516, 1258, 516, 526, 516, 1258, 516, 526, 516, 526, 516, 526, 516, 526, 514, 524, 516, 526, 514, 526, 514, 526, 516, 526, 516, 526, 516, 526, 514, 526, 516, 526, 514, 526, 516, 526, 516, 524, 516, 526, 514, 526, 516, 526, 516, 526, 514, 1258, 516, 526, 514, 526, 514, 526, 516, 526, 516, 524, 516, 526, 516, 526, 516, 524, 516, 524, 516, 1258, 516, 526, 516, 526, 516, 526, 516, 526, 514, 526, 516, 526, 516, 526, 516, 524, 516, 526, 514, 1260, 514, 526, 516, 1258, 516, 526, 516}; // UNKNOWN 5795BA90

//��������
u16 irDatas[IR_DATA_SIZE];
u8 isStudy;
u16 dataLen;

#ifdef ENABLE_IR_RECV

//�жϿ���|�ر�;
void EXTI_Set(uint32_t EXTI_LINE, u8 on)
{
	/* Clear the interrupt bit flag of the selected EXTI lines */
	EXTI->PR = EXTI_LINE;
	if (on)
		EXTI->IMR |= EXTI_LINE;
	else
		EXTI->IMR &= ~EXTI_LINE;
}

//�����ź�����PC9, ������չܿ���PC4
void IR_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//	RCC_APB2PeriphClockCmd(RCC_AHB2Periph_AFIO,ENABLE);	 // ʹ�ܸ��ù���ʱ��
	/* �ж�����ӳ�䵽 PC9 */
	//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9);

	/* Enable GPIOC clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PC9 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOC, GPIO_Pin_9);/* ���ź�,ң�ؽ���ͷ���1 */

	/* Connect EXTI Line9 to PC9 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource9);

	EXTI_InitStructure.EXTI_Line = EXTI_Line9;				// ��Ӧ�ж���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		// �ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	         //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			 // ����Ϊ�ж���1�ϵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // ��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;		 // �����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 // ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		u16 pulseWidth = 0; // ������
		u16 i = 0;

		while (1)
		{
			if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9))
			{ // �и��������
				pulseWidth = 0;
				while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9))
				{
					pulseWidth++;
					delay_us(19); // ��,ʵ���������ʱ��Ϊ:20*pulseWidth
					if (pulseWidth >= 2000)
					{		   // >40ms
						break; // �˳�ѭ��
					}
				}
				if (pulseWidth <= 15 || pulseWidth >= 2000)
				{		   // >40ms || < 300us
					break; // �˳�ѭ��
				}
				PulseTab[i] = pulseWidth * 20;
				i++;
			}
			else
			{
				pulseWidth = 0;
				while (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 0)
				{
					pulseWidth++;
					delay_us(19); // ��,ʵ���������ʱ��Ϊ:20*pulseWidth
				}
				if (pulseWidth <= 15 || pulseWidth >= 2000)
				{		   // >40ms || < 300us
					break; // �˳�ѭ��
				}
				PulseTab[i] = pulseWidth * 20;
				i++;
			}
			//if(i>=PULSETABSIZE) break;
		}
		/* Clear the EXTI line 15 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line9);
		dataLen = i;
	}
}

#endif

// PB1 ��TIM3_CH4
void TIM3_PWM_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	//Ƶ��f = 72M/[(psc+1)*(ar+1)]
	//72000/[(0+1)*(1895+1)] = 37.99K
	u16 ar = 1895; //�Զ���װ����ֵ
	u16 psc = 0;   //ʱ��Ԥ��Ƶ��

	/* Compute the prescaler value */
	psc = (uint16_t)((SystemCoreClock / 2) / 20000000) - 1; //TIM3 counter clock at 20 MHz
	ar = (20000000 / 38000) - 1;							//TIM3 output clock at 38 KHz

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //ʹ��GPIO����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //ʹ�ܶ�ʱ��ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);

	/* Connect TIM3 pins to AF2 */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

	TIM_TimeBaseInitStructure.TIM_Period = ar;						//��һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;					//��ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;				//ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			  //ѡ��ʱ��ģʽ:TIM PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	//TIM_OCInitStructure.TIM_Pulse = (ar+1)/2;	  //ռ�ձ� 50%
	TIM_OCInitStructure.TIM_Pulse = (ar + 1) / 3;			  //ռ�ձ�1:3
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�

	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

//���ⷢ��PC6
void IR_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_4); //�ط��䣬�ؽ���
}

char *IR_getProto(void)
{
	if (PulseTab[1] == 0)
	{
		return "wating..       ";
	}
	if (PulseTab[1] > 3500 && PulseTab[1] < 5500)
	{ /* ����Э���� */
		return "protocol: NEC  ";
	}
	else
	{
		return "protocol: other";
	}
}

u16 IR_getDatalen(void)
{
	return dataLen;
}

void IR_clearRecvData(void)
{
	int i;
	for (i = 0; i < PULSETABSIZE; i++)
	{
		PulseTab[i] = 0;
	}
}

#ifdef ENABLE_IR_RECV
void IR_recvEnable(u8 v)
{
	isStudy = v;
	EXTI_Set(EXTI_Line9, v);
	if (v)
	{
		IR_clearRecvData(); //�򿪽���ʱ���������
		GPIO_SetBits(GPIOC, GPIO_Pin_4);
	}
	else
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	}
}
#endif

u8 IR_isStudy(void)
{
	return isStudy;
}

void IR_write()
{
	FMC_unlock();
	FMC_write((uint8_t *)irDatas, sizeof(irDatas));
	FMC_lock();
}

void IR_save(uint32_t WriteAddr)
{
	int i;
	for (i = 0; i < PULSETABSIZE; i++)
	{
		irDatas[WriteAddr + i] = PulseTab[i];
	}
	IR_clearRecvData();
}

void sendData(u16 arr[])
{
	int i;
	printf("arr=[");
	for (i = 0; i < PULSETABSIZE; i++)
	{
		if (i == 0)
		{
			printf("%d", arr[i]);
		}
		else
		{
			printf(", %d", arr[i]);
		}
	}
	printf("]\r\n");
	for (i = 0; i < PULSETABSIZE / 2; i++)
	{
		if ((arr[2 * i] != 0) && (arr[2 * i + 1] != 0))
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_6);
			delay_us(arr[2 * i]);
			GPIO_ResetBits(GPIOC, GPIO_Pin_6);
			delay_us(arr[2 * i + 1]);
		}
	}
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	delay_us(555);
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);
}

void IR_read(uint32_t ReadAddr)
{
	u16 i, v;

	if (isStudy)
	{
		return;
	}
	printf("irDatas=[");
	for (i = 0; i < PULSETABSIZE; i++)
	{
		if (i == 0)
		{
			printf("%d", irDatas[i + ReadAddr]);
		}
		else
		{
			printf(", %d", irDatas[i + ReadAddr]);
		}
	}
	printf("] %d\r\n", i);

	// sendData(&irDatas[ReadAddr]);
	for (i = 0; i < PULSETABSIZE; i++)
	{
		v = irDatas[i + ReadAddr];
		if (v != 0 && v != 0xffff)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_6);
			delay_us(v);
		}

		i++;

		v = irDatas[i + ReadAddr];
		if (v != 0 && v != 0xffff)
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_6);
			delay_us(v);
		}
	}
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	delay_us(555);
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);
}

void IR_SendData(void)
{
	int i;
	if (isStudy)
	{
		return;
	}

	printf("PulseTab=[");
	for (i = 0; i < PULSETABSIZE; i++)
	{
		if (i == 0)
		{
			printf("%d", PulseTab[i]);
		}
		else
		{
			printf(", %d", PulseTab[i]);
		}
	}
	printf("] %d\r\n", i);

	// for (i = 0; i < PULSETABSIZE; i++)
	// {
	// 	if (PulseTab[i] != 0 && PulseTab[i] != 0xffff)
	// 	{
	// 		GPIO_SetBits(GPIOC, GPIO_Pin_6);
	// 		delay_us(PulseTab[i]);
	// 	}

	// 	i++;

	// 	if (PulseTab[i] != 0 && PulseTab[i] != 0xffff)
	// 	{
	// 		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	// 		delay_us(PulseTab[i]);
	// 	}
	// }
	for (i = 0; i < PULSETABSIZE / 2; i++)
	{
		if ((PulseTab[2 * i] != 0) && (PulseTab[2 * i + 1] != 0))
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_6);
			delay_us(PulseTab[2 * i]);
			GPIO_ResetBits(GPIOC, GPIO_Pin_6);
			delay_us(PulseTab[2 * i + 1]);
		}
	}
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	delay_us(555);
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);
}

//��ʼ��
void IR_init(void)
{
	IR_GPIO_Init();
	TIM3_PWM_Init(); //38K�ز�

#ifdef ENABLE_IR_RECV
	IR_Init(); //�������
	//IR_recvEnable(1);//�򿪺������
	IR_recvEnable(0); //�رպ������
#endif
	//��ȡ������FLASH�еİ�������
	FMC_read((uint8_t *)irDatas, sizeof(irDatas));
}
