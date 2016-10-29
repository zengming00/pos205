#ifndef __UC1701X_H
#define __UC1701X_H

#include <stm32f2xx.h>

void LCD_init(void);

//打开显示
void LCD_on(void);

//关闭显示
void LCD_off(void);

//清除整个屏幕
void LCD_clear(void);

//设置写入地址
void LCD_setAdress(uint8_t page, uint8_t colume);


#define LCD_WRITE_CMD   0    //写命令
#define LCD_WRITE_DATA  1    //写数据
void LCD_writeByte(uint8_t data, uint8_t dataOrCmd);

//向下滚动指定行数，row = 0-63
void LCD_scroll(uint8_t row);

//反色显示
void LCD_inverseDisplay(uint8_t inverse);

void LCD_setAllPixel(uint8_t data);


//16*16单个汉字（扫描为从下到上，从左到右）
//row 一行的高度为8 ，设置为1时距顶部的距离为8，
//只能设置0-2，设置为1时汉字垂直居中（此设置和页地址有关）
void LCD_showCnChar(uint8_t* arr, uint8_t row, uint8_t colume);

//显示指定个数的汉字
void LCD_showCnString(uint8_t* arr, uint8_t row, int num);






/////////////////////////////////以下函数带缓存功能//////////////////////////////////////

//将缓存刷新到屏幕
void LCD_refresh(void);

//将缓存全部清零
void LCD_clearBuffer(void);

//将缓存清零，并清除屏幕
void LCD_clearAll(void);


//用指定字节填充缓存
void LCD_fill(uint8_t chPoint);

/*
画点
chPoint   1为黑色，0为白色
show      1立即显示到屏幕，0不立即显示到屏幕
*/
void LCD_drawPoint(uint8_t x, uint8_t y, uint8_t chPoint, uint8_t show);

//画字符
void LCD_drawChar(uint8_t x, uint8_t y, uint8_t chChr, uint8_t chSize, uint8_t chMode, uint8_t show);

//画字符串
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint8_t chSize, uint8_t chMode, uint8_t show);

//水平线
void LCD_drawLineH(uint8_t x1, uint8_t x2, uint8_t y, uint8_t chPoint, uint8_t show);

//垂直线
void LCD_drawLineV(uint8_t y1, uint8_t y2, uint8_t x, uint8_t chPoint, uint8_t show);

//画线
void LCD_drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t chPoint, uint8_t show);

//画圆
void LCD_drawCircle(uint8_t x, uint8_t y, uint8_t r, uint8_t chPoint, uint8_t show);

//画矩形
void LCD_drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t chPoint, uint8_t show);















#endif

