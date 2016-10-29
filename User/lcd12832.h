#ifndef __UC1701X_H
#define __UC1701X_H

#include <stm32f2xx.h>

void LCD_init(void);

//����ʾ
void LCD_on(void);

//�ر���ʾ
void LCD_off(void);

//���������Ļ
void LCD_clear(void);

//����д���ַ
void LCD_setAdress(uint8_t page, uint8_t colume);


#define LCD_WRITE_CMD   0    //д����
#define LCD_WRITE_DATA  1    //д����
void LCD_writeByte(uint8_t data, uint8_t dataOrCmd);

//���¹���ָ��������row = 0-63
void LCD_scroll(uint8_t row);

//��ɫ��ʾ
void LCD_inverseDisplay(uint8_t inverse);

void LCD_setAllPixel(uint8_t data);


//16*16�������֣�ɨ��Ϊ���µ��ϣ������ң�
//row һ�еĸ߶�Ϊ8 ������Ϊ1ʱ�ඥ���ľ���Ϊ8��
//ֻ������0-2������Ϊ1ʱ���ִ�ֱ���У������ú�ҳ��ַ�йأ�
void LCD_showCnChar(uint8_t* arr, uint8_t row, uint8_t colume);

//��ʾָ�������ĺ���
void LCD_showCnString(uint8_t* arr, uint8_t row, int num);






/////////////////////////////////���º��������湦��//////////////////////////////////////

//������ˢ�µ���Ļ
void LCD_refresh(void);

//������ȫ������
void LCD_clearBuffer(void);

//���������㣬�������Ļ
void LCD_clearAll(void);


//��ָ���ֽ���仺��
void LCD_fill(uint8_t chPoint);

/*
����
chPoint   1Ϊ��ɫ��0Ϊ��ɫ
show      1������ʾ����Ļ��0��������ʾ����Ļ
*/
void LCD_drawPoint(uint8_t x, uint8_t y, uint8_t chPoint, uint8_t show);

//���ַ�
void LCD_drawChar(uint8_t x, uint8_t y, uint8_t chChr, uint8_t chSize, uint8_t chMode, uint8_t show);

//���ַ���
void LCD_drawString(uint8_t x, uint8_t y, char* str, uint8_t chSize, uint8_t chMode, uint8_t show);

//ˮƽ��
void LCD_drawLineH(uint8_t x1, uint8_t x2, uint8_t y, uint8_t chPoint, uint8_t show);

//��ֱ��
void LCD_drawLineV(uint8_t y1, uint8_t y2, uint8_t x, uint8_t chPoint, uint8_t show);

//����
void LCD_drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t chPoint, uint8_t show);

//��Բ
void LCD_drawCircle(uint8_t x, uint8_t y, uint8_t r, uint8_t chPoint, uint8_t show);

//������
void LCD_drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t chPoint, uint8_t show);















#endif

