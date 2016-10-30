#include <stm32f2xx.h>
#include "systick.h"
#include "fmc.h"
#include "ir.h"

#define ENABLE_IR_RECV

/* ���� ������������ */
u16 PulseTab[PULSETABSIZE] = {9000,4500,	 // Chanel ++
					 560,560,560,560,560,560,560,560,560,560,560,560, 560,1680,560,560,
					 560,1680,560,1680,560,1680,560,1680,560,1680,560,1680,560,560,560,1680,
					 560,1680,560,1680,560,560,560,1680,560,1680,560,560,560,560,560,560,
					 560,560,560,560,560,1680,560,560,560,560,560,1680,560,1680,560,1680,
					 480,38924,9033,2291,510};
u8 isStudy;

#ifdef ENABLE_IR_RECV
//�жϿ���|�ر�;  
void EXTI_Set(uint32_t EXTI_LINE, u8 on)
{
    /* Clear the interrupt bit flag of the selected EXTI lines */
    EXTI->PR = EXTI_LINE; 
    if(on)
		EXTI->IMR |= EXTI_LINE; 
    else 
		EXTI->IMR &= ~EXTI_LINE;
}

//�����ź�����PC9, ������չܿ���PC4
void IR_Init(void)
{	
	EXTI_InitTypeDef  EXTI_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;	
    GPIO_InitTypeDef   GPIO_InitStructure;
	
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
	
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;	  			// ��Ӧ�ж���
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	  	// �ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
	
   // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	         //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;				// ����Ϊ�ж���1�ϵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	// ��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			// �����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  
}

void EXTI9_5_IRQHandler(void) 
{   
  if(EXTI_GetITStatus(EXTI_Line9) != RESET)
  {
		u16 pulseWidth = 0;     	// ������
		u8 i = 0;
		
		while(1)     
		{
			if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)){	// �и��������         		             			
				pulseWidth = 0;  		   
				while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)){
					pulseWidth++;
					delay_us(19);   // ��,ʵ���������ʱ��Ϊ:20*pulseWidth							
					if(pulseWidth >= 2000){	 // >40ms
						break;	// �˳�ѭ��
					}
				}
				if(pulseWidth<=15 || pulseWidth >= 2000){	 // >40ms || < 300us
					break;	// �˳�ѭ��
				}
				PulseTab[i] = pulseWidth*20;	
				i++;
					 		               						
			}else{			
				pulseWidth = 0; 			
				while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 0){
					pulseWidth++;
					delay_us(19);   // ��,ʵ���������ʱ��Ϊ:20*pulseWidth
				}
				if(pulseWidth<=15 || pulseWidth >= 2000){	 // >40ms || < 300us
					break;	// �˳�ѭ��
				}		
				PulseTab[i] = pulseWidth*20;
				i++;
			}
			//if(i>=PULSETABSIZE) break;
		}
    /* Clear the EXTI line 15 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line9);
  }
}

#endif



// PB1 ��TIM3_CH4
void TIM3_PWM_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef 	TIM_OCInitStructure;

	//Ƶ��f = 72M/[(psc+1)*(ar+1)] 
	//72000/[(0+1)*(1895+1)] = 37.99K
	u16 ar=1895;  //�Զ���װ����ֵ	
	u16 psc=0;    //ʱ��Ԥ��Ƶ��
	
	
	/* Compute the prescaler value */
	psc = (uint16_t) ((SystemCoreClock /2) / 20000000) - 1;//TIM3 counter clock at 20 MHz
	ar = (20000000 / 38000) - 1;//TIM3 output clock at 38 KHz

 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   //ʹ��GPIO����ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	
	/* Connect TIM3 pins to AF2 */  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

	TIM_TimeBaseInitStructure.TIM_Period = ar;	 //��һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;	//��ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;  //ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //ѡ��ʱ��ģʽ:TIM PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
	//TIM_OCInitStructure.TIM_Pulse = (ar+1)/2;	  //ռ�ձ� 50%
	TIM_OCInitStructure.TIM_Pulse = (ar+1)/3;	  //ռ�ձ�1:3
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//�������:TIM����Ƚϼ��Ը�

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

	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_6 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_4);//�ط��䣬�ؽ���
}

void IR_SendData()
{
	u8 i;
	if(isStudy) return;
	for(i=0; i<100; i++){
		if((PulseTab[2*i]!=0) && (PulseTab[2*i+1]!=0)){
			GPIO_SetBits(GPIOC, GPIO_Pin_6);
			delay_us(PulseTab[2*i]);
			GPIO_ResetBits(GPIOC, GPIO_Pin_6);
			delay_us(PulseTab[2*i+1]);
		}
	}
	GPIO_SetBits(GPIOC, GPIO_Pin_6); //Ҫ��Ҫע�͵���
	delay_us(555); //Ҫ��Ҫע�͵���
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);
}
/*
#if 0
void SendData(u8 SysNum, u8 KeyNum) 
{ 
	u8 i; 
	u32 data = 0;
//	u32 data = 0xED12BF40; 
//	TIM2_PWM_Init(1895,0);	//72000/(1895+1) = 37.99K
	data = ((~KeyNum & 0xFF)<<24) + ((KeyNum & 0xFF)<<16) + ((~SysNum & 0xff)<< 8) + SysNum;

	LED4 = 0;
//	TIM_Cmd(TIM2, ENABLE); 
	delay_us(9000); 
	LED4 = 1;
//	TIM_Cmd(TIM2, DISABLE); 
	delay_us(4500);
	LED4 = 0;
//	TIM_Cmd(TIM2, ENABLE); 	
	
	for(i=0; i<33; i++) 
	{ 
		delay_us(480);//480
		LED4 = 1;
//		TIM_Cmd(TIM2, DISABLE); 
		
		if(((data >> i) & 0x00000001) == 0) 
		{   
			delay_us(620);   //0.56ms 620
		} 
		else 
		{  
			delay_us(1690); //1.69ms 
		} 
		LED4 = 0;
//		TIM_Cmd(TIM2, ENABLE); 
	} 
//	TIM_Cmd(TIM2, DISABLE); 
	LED4=1;
} 
#endif

*/


char* IR_getProto(void){
	if(PulseTab[1]==0){
		return "wating..       ";
	}
	if(PulseTab[1]>3500 && PulseTab[1]<5500) { /* ����Э���� */
		return "protocol: NEC  ";
	}else{
		return "protocol: other";
	}
}

void IR_clearRecvData(void){
	int i;
	for(i=0; i<PULSETABSIZE; i++){
		PulseTab[i] = 0;
	}
}

#ifdef ENABLE_IR_RECV
void IR_recvEnable(u8 v){
	isStudy = v;
	EXTI_Set(EXTI_Line9, v);
	if(v){
		IR_clearRecvData();//�򿪽���ʱ���������
		GPIO_SetBits(GPIOC, GPIO_Pin_4);
	}else{
		GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	}
}
#endif 

u8 IR_isStudy(void){
	return isStudy;
}

//��������
u16 irDatas[IR_DATA_SIZE];

void IR_write(){
	FMC_unlock();
	FMC_write((uint8_t*)irDatas, sizeof(irDatas));
	FMC_lock();
}

void IR_save(uint32_t WriteAddr){
	int i;
	for(i=0; i<200; i++){
		irDatas[WriteAddr+i] = PulseTab[i];
	}
	IR_clearRecvData();
}

void IR_read(uint32_t ReadAddr){
	int i;
	for(i=0; i<200; i++){
		PulseTab[i] = irDatas[ReadAddr+i];
	}
	IR_SendData();
}

//��ʼ��
void IR_init(void){
	IR_GPIO_Init();
	TIM3_PWM_Init();//38K�ز�
	
#ifdef ENABLE_IR_RECV
	IR_Init();//�������
	//IR_recvEnable(1);//�򿪺������
	IR_recvEnable(0);//�رպ������
#endif
	//��ȡ������FLASH�еİ�������
	FMC_read((uint8_t*)irDatas, sizeof(irDatas));
	
}

