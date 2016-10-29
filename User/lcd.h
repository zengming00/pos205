#ifndef LCDDRV_H
#define LCDDRV_H

#include <stm32f2xx.h>

#define LCD_MIRROR_X
#define LCD_MIRROR_Y


#define LCD_CS_PIN GPIO_Pin_6
#define LCD_RS_PIN GPIO_Pin_7
#define LCD_SCLK_PIN GPIO_Pin_8
#define LCD_SDIN_PIN GPIO_Pin_9
#define LCD_RST_PIN GPIO_Pin_14
#define LCD_BL_PIN GPIO_Pin_15

//8位总线，请使能此宏定义
//#define DateBitWidth8

#define LCD_X_start 0
#define LCD_X_end 128
#define LCD_Y_start 0
#define LCD_Y_end 32


#define LCD_DATA_PORT GPIOB
#define LCD_CONTROL_PORT GPIOC


void lcd_wr_cmd8(u8 cmd);
void lcd_wr_data8(u8 dat);
void lcd_wr_CmdData8(u8 cmd,u8 dat);
void LCD_Init(void);													   	//初始化
void LCD_On(void);
void LCD_Off(void);
void LCD_Fill(u8 dat);
void LCD_Clear(void);
void LCD_inverse(u8 On);
void LCD_set_XY(u8 row,u8 colume);
void LCD_draw_tx(void);
void LCD_char(char C);
void LCD_show_string(char *C);
void LCD_show_hanzi(u8 row,u8 colume,u8 *pword);






#endif
