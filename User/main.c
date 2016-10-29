#include "stm32f2xx.h"
#include "usart.h"
#include "18B20.h"
#include "systick.h"
#include "io.h"
#include "key.h"
#include "temp.h"
#include "lcd12832.h"
#include "24cxx.h"

void Delay(__IO uint32_t nCount);
void testLcd(void);




unsigned char saohui[] =
{

0x00,0x00,0x1F,0x92,0x52,0x32,0x12,0x1F,0x12,0x32,0x52,0x92,0x1F,0x00,0x00,0x00,0x08,0x08,0xC8,0x48,0x48,0x48,0x48,0xFF,0x48,0x48,0x48,0x48,0xC8,0x08,0x08,0x00,/*"单",0*/
0x00,0x00,0x00,0x7F,0x04,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x01,0x06,0xF8,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,0x00,0x00,/*"片",1*/
0x08,0x08,0x0B,0xFF,0x09,0x08,0x00,0x7F,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,0x00,0x20,0xC0,0x00,0xFF,0x00,0xC1,0x06,0xF8,0x00,0x00,0x00,0xFC,0x02,0x02,0x1E,0x00,/*"机",2*/
0x00,0x00,0x47,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x1E,0x00,0x00,/*"已",3*/
0x0F,0x00,0xFF,0x04,0x08,0x00,0x11,0x11,0xF2,0x1A,0x24,0x2A,0x21,0x27,0x00,0x00,0x01,0x0E,0xF0,0x08,0x04,0x41,0x42,0x4C,0x70,0x40,0x40,0x7C,0x42,0x42,0x4E,0x00,/*"烧",4*/
0x7F,0x49,0x89,0x01,0x49,0x49,0x7F,0x00,0x05,0x79,0x41,0x41,0x79,0x05,0x04,0x00,0x22,0x23,0x22,0x3E,0x24,0x24,0x24,0x01,0x01,0xC2,0x34,0x08,0x34,0xC2,0x01,0x00,/*"毁",5*/

};




void showString(char* str){
	LCD_clearAll();
    LCD_drawString(0, 0, str, 16, 1, 1);
}

void printTemp(){
	short temp=DS18B20_Get_Temp();
	char buf[32];
	if(temp<0){
		temp=-temp;	
		sprintf(buf,"18b20: -%d.%dC ",temp/10,temp%10);
	}else{
		sprintf(buf,"18b20: %d.%dC ",temp/10,temp%10);
	}
	printf(buf);
	showString(buf);
}
void show(unsigned char* arr, int num){
	LCD_clearAll();
	LCD_showCnString(arr, 1, num);
}
void showNumber(int num){
	char buf[32];
	sprintf(buf,"%d",num);
	showString(buf);
}

void beep(){ //压电PA8
	int i,j;
	
	IOInit(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_8);
	for(j=0; j<5; j++){
		for(i=0; i<1000; i++){
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
			delayms(1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
			delayms(1);
		}
		delayms(1000);
	}
}
void powerOff(){
	gpioOut(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_2, 0);
}
void powerOn(){
	gpioOut(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_2, 1);
}

void doKey(int key){
	switch(key){
	case 10: //电源
		break;
	case 5: //	清除
		powerOff();
		break;
	case 1://	确认
		show(saohui,6);
		beep();
		break;
	case 0: //	蓝牙键
		printTemp();
		break;
	case 14://	取消
		break;
	case 3://	0				
		break;
	case 9://	1				
		LedRed(0);
		break;
	case 4://	2				
		LedRed(1);
		break;
	case 6://	3
		LedBlue(0);
		break;
	case 8://	4				
		LedBlue(1);
		break;
	case 13://	5				
		LedYellow(0);
		break;
	case 12://	6	
		LedYellow(1);
		break;
	case 2://	7
		LedLcd(0);			
		break;
	case 7://	8	
		LedLcd(1);				
		break;
	case 15://	9
		testLcd();	
		break;
	}
}

void testLcd(){
			int i ;

			//中文字符
			LCD_clearAll();
			LCD_showCnString(saohui, 1, 6);
			delayms(2000);
			
			//向下滚动
			for(i=0; i<=63; i++){
				LCD_scroll(i);
				delayms(100);
			}
			LCD_scroll(0);
			delayms(2000);
			
			//测试直接通过地址写数据
			LCD_clearAll();
			LCD_setAdress(0, 0);
			LCD_writeByte(225, LCD_WRITE_DATA);

			LCD_setAdress(3, 0);
			LCD_writeByte(193, LCD_WRITE_DATA);
			
			for(i=1; i<=126; i++){ //左右都少画一列
				LCD_setAdress(2, i);
				LCD_writeByte(225, LCD_WRITE_DATA);
			}
			
			delayms(3000);	
			
			//测试所有字符
			//LCD_clearAll();
			LCD_fill(0xff);
			LCD_refresh();
			for(i=0; i<95; i++){ //字库只有95个字符,第一个字符是空格，最后一个字符是~
				LCD_drawChar(0,0, i+' ', 24, 0, 1);
				LCD_drawChar(12,0, i+' ', 16, 0, 1);
				LCD_drawChar(20,0, i+' ', 12, 0, 1);				
				delayms(100);
			}
			delayms(3000);
			
			//测试画圆
			for(i=0; i<16; i++){	
				LCD_clearAll();
				LCD_drawCircle(50,16,i,1,1);				
				delayms(100);
			}	
			delayms(2000);

			if(1){//测试画点
				int x=0,y=0;
				LCD_fill(0xff);
				LCD_refresh();
				for(y=1; y<=30; y++){
					for(x=1; x<=126; x++){
						LCD_drawPoint(x, y, 0, 1);						
						delayms(5);
					}
				}
			}
			//测试画线
			LCD_clearAll();
			LCD_drawLineH(1,126,0,1, 1);//上			
			delayms(1000);
			
			LCD_drawLineV(1,30,127,1, 1);//右			
			delayms(1000);
			
			LCD_drawLineH(1,126,31,1, 1);//下			
			delayms(1000);
			
			LCD_drawLineV(1,30,0,1, 1);//左
			
			LCD_drawLine(0,0,127,31,1, 1);//左上到右下
			
			delayms(3000);
		
		if(1){
			int x=0,y=0,xn=5,yn=3;
			int size=10,tmp;
			
			LCD_clearAll();
			for(tmp=0; tmp<50; tmp++){
				if(tmp % 2){
					LCD_drawRect(0,0,10,10,1, 1);
					LCD_drawRect(10,0,20,10,0, 1);					
				}else{
					LCD_drawRect(0,0,10,10,0, 1);
					LCD_drawRect(10,0,20,10,1, 1);					
				}
				
				delayms(100);
			}
			while(1){
				tmp = x+xn;
				if(tmp > 128-size || tmp<0){
					xn = -xn;
				}
				tmp = y+yn;
				if(tmp > 32-size || tmp<0){
					yn = -yn;
				}
				LCD_drawRect(x,y,x+size,y+size,0, 1);
				x += xn;
				y += yn;
				LCD_drawRect(x,y,x+size,y+size,1, 1);				
				delayms(200);
				if(keyScan() != -1){
					break;
				}
			}
		}
}

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f2xx.c file
     */
   if(1){//全闪
   		int i;
   		GPIO_InitTypeDef  GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_Init(GPIOD, &GPIO_InitStructure);
   		while(1){
			  GPIOA->ODR ^= GPIO_Pin_All;
			  GPIOB->ODR ^= GPIO_Pin_All;
			  GPIOC->ODR ^= GPIO_Pin_All;
			  GPIOD->ODR ^= GPIO_Pin_All;
			  //delayms(1000);
			  for(i=0; i<0x00ffffff; i++){
			  }
		}
   }
#if 0
   delay_init();
   USART_Configuration();
   printf("helloworld %s  %s\r\n",__DATE__,__TIME__);
   
   
   if(0){
	   u8 data = 0x12;
	   char buf[32];
	   u8 s;
	   
	   LCD_init();
	   //AT24CXX_Init();
	   IIC2_Init();
	   
	   for(data=0; data<255; data++){
			if(IRcvStr(0xa0, 0, &s, 1)){
//			   sprintf(buf, "r1=%d,%d", AT24CXX_ReadOneByte(data), data);
			   sprintf(buf, "r1=%d,%d", s, data);
				showString(buf);
				delayms(100);
			}
		}
		
	   AT24CXX_WriteOneByte(0,data);

	   sprintf(buf, "r2=%d", AT24CXX_ReadOneByte(0));
		showString(buf);
		delayms(10000);
	
	   while(1);
   }
   
  //开始我们的程序
  if(1){
  	int i=0;
	int key=0,lastKey=0;
	char buf[32];

	LCD_init();
	keyInit();
	LedInit();
	
	showString("helloworld!");

	
	if(keyScan() == 10){//按电源键，电池开机
		powerOn();
	}
		
	do{
		key = DS18B20_Init();//初始化DS18B20,兼检测18B20
		if(key){
			sprintf(buf,"18B20 Failed! %d/10", i++);
			showString(buf);
			delayms(1000);
			if(i > 10) break;
		}
	}while(key);
	
	if(!key){//18B20初始化成功
		while(DS18B20_Get_Temp() == 850){//跳过刚开始的无效值
			delayms(50);
		}
		printTemp();
	} else {
		showString("18B20 Failed!");
	}
	
	while (1) {
		key = keyScan();
		if(key != -1 && key != lastKey) {
			lastKey = key;
		
			switch(key){
				case 5:  sprintf(buf,"clear code:%d",key); break;
				case 1:  sprintf(buf,"ok code:%d",key); break;
				case 0:  sprintf(buf,"BT code:%d",key); break;
				case 10: sprintf(buf,"power code:%d",key); break;
				case 14: sprintf(buf,"cancel code:%d",key); break;
				case 3:  sprintf(buf,"0 code:%d",key);  break;				
				case 9:  sprintf(buf,"1 code:%d",key); break;				
				case 4:  sprintf(buf,"2 code:%d",key); break;				
				case 6:  sprintf(buf,"3 code:%d",key); break;				
				case 8:  sprintf(buf,"4 code:%d",key); break;				
				case 13: sprintf(buf,"5 code:%d",key); break;				
				case 12: sprintf(buf,"6 code:%d",key); break;				
				case 2:  sprintf(buf,"7 code:%d",key); break;				
				case 7:  sprintf(buf,"8 code:%d",key); break;				
				case 15: sprintf(buf,"9 code:%d",key); break;
				default: sprintf(buf,"err code:%d",key); break;			
			}
			showString(buf);
			doKey(key);
		}
	//	delayms(500);
	}
  }
#endif
}
/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {

  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {

  }
}
#endif


