#include "lcd12832.h"
#include "font.h"
#include "systick.h"
#include "io.h"

#define LCD_CS_PIN GPIO_Pin_6   //CS0
#define LCD_RS_PIN GPIO_Pin_7   //CD
#define LCD_SCLK_PIN GPIO_Pin_8 //SDA
#define LCD_SDIN_PIN GPIO_Pin_9 //SCK
#define LCD_DATA_PORT GPIOB

#define LCD_RST_PIN GPIO_Pin_14 //RST
#define LCD_BL_PIN GPIO_Pin_15  //背光
#define LCD_CONTROL_PORT GPIOC

unsigned char LCD_GRAM[128][4];

static void LCD_CS(char v)
{
	BitAction bit = v ? Bit_SET : Bit_RESET;
	GPIO_WriteBit(LCD_DATA_PORT, LCD_CS_PIN, bit);
}
static void LCD_RS(char v)
{
	BitAction bit = v ? Bit_SET : Bit_RESET;
	GPIO_WriteBit(LCD_DATA_PORT, LCD_RS_PIN, bit);
}
static void LCD_SCLK(char v)
{
	BitAction bit = v ? Bit_SET : Bit_RESET;
	GPIO_WriteBit(LCD_DATA_PORT, LCD_SCLK_PIN, bit);
}
static void LCD_SDIN(char v)
{
	BitAction bit = v ? Bit_SET : Bit_RESET;
	GPIO_WriteBit(LCD_DATA_PORT, LCD_SDIN_PIN, bit);
}
static void LCD_RST(char v)
{
	BitAction bit = v ? Bit_SET : Bit_RESET;
	GPIO_WriteBit(LCD_CONTROL_PORT, LCD_RST_PIN, bit);
}
static void LCD_BL(char v)
{
	BitAction bit = v ? Bit_SET : Bit_RESET;
	GPIO_WriteBit(LCD_CONTROL_PORT, LCD_BL_PIN, bit);
}

void LCD_writeByte(uint8_t data, uint8_t dataOrCmd)
{
	uint8_t i;
	LCD_CS(0);
	LCD_RS(dataOrCmd);
	for (i = 0; i < 8; i++)
	{
		LCD_SCLK(0);
		if (data & 0x80)
			LCD_SDIN(1);
		else
			LCD_SDIN(0);
		LCD_SCLK(1);
		data <<= 1;
	}
	LCD_CS(1);
}

void LCD_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);

	IO_OUT_PULL_UP(LCD_DATA_PORT, LCD_CS_PIN | LCD_RS_PIN | LCD_SCLK_PIN | LCD_SDIN_PIN);
	GPIO_SetBits(LCD_DATA_PORT, LCD_CS_PIN | LCD_RS_PIN | LCD_SCLK_PIN | LCD_SDIN_PIN);

	IO_OUT_PULL_UP(LCD_CONTROL_PORT, LCD_RST_PIN | LCD_BL_PIN);
	GPIO_SetBits(LCD_CONTROL_PORT, LCD_RST_PIN | LCD_BL_PIN);

	LCD_CS(1);
	LCD_RS(1);
	LCD_SCLK(1);
	LCD_SDIN(1);
	LCD_BL(0); //点亮背光

	LCD_RST(0);  //低电平复位
	delayms(50); //可以为100ms？
	LCD_RST(1);
	delayms(50); //可以为20ms？

	LCD_writeByte(0xe2, LCD_WRITE_CMD); //软件复位
	delayms(5);
	LCD_writeByte(0x2c, LCD_WRITE_CMD); //升压步骤1
	delayms(5);
	LCD_writeByte(0x2e, LCD_WRITE_CMD); //升压步骤2
	delayms(5);
	LCD_writeByte(0x2f, LCD_WRITE_CMD); //升压步骤3
	delayms(5);

	LCD_writeByte(0x24, LCD_WRITE_CMD); //粗调对比度，可设置范围0x20~0x27
	LCD_writeByte(0x81, LCD_WRITE_CMD); //微调对比度
	LCD_writeByte(0x24, LCD_WRITE_CMD); //微调对比度，可设置范围0x00~0x3f

	LCD_writeByte(0xa2, LCD_WRITE_CMD); //1/9偏压比（bias）

	LCD_writeByte(0xa1, LCD_WRITE_CMD); //列扫描顺序 从右到左
										//	LCD_writeByte(0xa0, LCD_WRITE_CMD);     //列扫描顺序 从左到右
	LCD_writeByte(0xcf, LCD_WRITE_CMD); //行扫描顺序 从下到上
										//	LCD_writeByte(0xc0, LCD_WRITE_CMD);     //行扫描顺序 从上到下

	LCD_writeByte(0x40, LCD_WRITE_CMD); //起始行，从第一行开始
	LCD_writeByte(0xaf, LCD_WRITE_CMD); //开显示
}

void LCD_on()
{
	LCD_writeByte(0xaf, LCD_WRITE_CMD); //开显示
}

void LCD_off()
{
	LCD_writeByte(0xae, LCD_WRITE_CMD); //关显示
}

/*
设置写入地址, 每8行为1页, 32行共4页 0-3
0页在最底，3页在最顶
0列在最左, 127页在最右（因为是128x32的屏，所以最后一列是127）
写入字节时，低位在底，高位在上
例如设置LCD_setAdress(0, 0)再写入225（0b11100001）将在左下角显示
1
1
1
0
0
0
0
1
*/
void LCD_setAdress(uint8_t page, uint8_t colume)
{
	//设置页地址
	LCD_writeByte(0xb0 + page, LCD_WRITE_CMD);

	colume += 4; //实测第4列才开始显示
	//设置列地址的高四位
	LCD_writeByte(0x10 + (colume >> 4 & 0x0f), LCD_WRITE_CMD);
	//设置列地址的低四位
	LCD_writeByte((colume & 0x0f), LCD_WRITE_CMD);
}

void LCD_clear()
{
	uint8_t page, col;
	for (page = 0; page < 4; page++)
	{
		LCD_setAdress(page, 0);
		for (col = 0; col < 128; col++)
		{
			LCD_writeByte(0x00, LCD_WRITE_DATA);
		}
	}
}

//向下滚动指定行数，row = 0-63
void LCD_scroll(uint8_t row)
{
	LCD_writeByte((0x40 + row), LCD_WRITE_CMD);
}

//反色显示
void LCD_inverseDisplay(uint8_t inverse)
{
	if (inverse)
	{
		LCD_writeByte((0xa7), LCD_WRITE_CMD);
	}
	else
	{
		LCD_writeByte((0xa6), LCD_WRITE_CMD);
	}
}

void LCD_setAllPixel(uint8_t data)
{
	if (data)
	{
		LCD_writeByte((0xa5), LCD_WRITE_CMD);
	}
	else
	{
		LCD_writeByte((0xa4), LCD_WRITE_CMD);
	}
}

//16*16单个汉字（扫描为从下到上，从左到右）
void LCD_showCnChar(uint8_t *arr, uint8_t row, uint8_t colume)
{
	unsigned char i;
	if (row > 3)
		return;
	row = 3 - row; //page反转
	LCD_setAdress(row, colume);
	for (i = 0; i < 16; i++)
		LCD_writeByte(*(arr + i), LCD_WRITE_DATA);
	LCD_setAdress(row - 1, colume);
	for (i = 0; i < 16; i++)
	{
		LCD_writeByte(*(arr + 16 + i), LCD_WRITE_DATA);
	}
}

void LCD_showCnString(uint8_t *arr, uint8_t row, int num)
{
	int i;
	for (i = 0; i < num; i++)
	{
		LCD_showCnChar(arr + 32 * i, row, 16 * i);
	}
}

///////////////////////////////////以下是带缓存函数//////////////////////////////

void LCD_clearAll()
{
	LCD_clearBuffer();
	LCD_clear();
}

void LCD_clearBuffer()
{
	LCD_fill(0);
}

void LCD_fill(uint8_t chPoint)
{
	uint8_t page, n;
	for (page = 0; page < 4; page++)
	{
		for (n = 0; n < 128; n++)
			LCD_GRAM[n][page] = chPoint;
	}
}

void LCD_refresh(void)
{
	uint8_t page, n;
	for (page = 0; page < 4; page++)
	{
		LCD_setAdress(page, 0);
		for (n = 0; n < 128; n++)
			LCD_writeByte(LCD_GRAM[n][page], LCD_WRITE_DATA);
	}
}

void LCD_drawPoint(uint8_t x, uint8_t y, uint8_t chPoint, uint8_t show)
{
	uint8_t page, chTemp = 0;
	if (x >= 128 || y >= 32)
		return; //超出范围
	page = 3 - (y / 8);
	chTemp = 1 << (7 - (y % 8));
	if (chPoint)
		LCD_GRAM[x][page] |= chTemp;
	else
		LCD_GRAM[x][page] &= ~chTemp;
	if (show)
	{
		LCD_setAdress(page, x);
		LCD_writeByte(LCD_GRAM[x][page], LCD_WRITE_DATA);
	}
}

/**
chSize取值24、16、12， 24x12（高24宽12） 16x08 12x06 三种ASCII字体
24号字体一行显示10
16号字体一行显示16个字母
12号字体一行显示21个字母
*/
void LCD_drawString(uint8_t x, uint8_t y, char *str, uint8_t chSize, uint8_t chMode, uint8_t show)
{
	int w;
	if (chSize == 24)
	{
		w = 12;
	}
	else if (chSize == 16)
	{
		w = 8;
	}
	else if (chSize == 12)
	{
		w = 6;
	}
	else
	{
		return;
	}
	while (*str)
	{
		LCD_drawChar(x, y, *str, chSize, chMode, show);
		x += w;
		str++;
	}
}

//2412（高24宽12） 1608 1206 三种ASCII字体
void LCD_drawChar(uint8_t x, uint8_t y, uint8_t chChr, uint8_t chSize, uint8_t chMode, uint8_t show)
{
	uint8_t i, j;
	uint8_t chTemp, y0 = y;
	chChr = chChr - ' ';
	if (chSize >= 24)
	{
		for (i = 0; i < 36; i++)
		{ //需要36字节
			if (chMode)
				chTemp = asc2_2412[chChr][i];
			else
				chTemp = ~asc2_2412[chChr][i];

			for (j = 0; j < 8; j++)
			{
				if (chTemp & 0x80)
					LCD_drawPoint(x, y, 1, show);
				else
					LCD_drawPoint(x, y, 0, show);
				chTemp <<= 1;
				y++;
				if ((y - y0) == chSize)
				{
					y = y0;
					x++;
					break;
				}
			}
		}
	}
	else
	{
		for (i = 0; i < chSize; i++)
		{
			if (chSize == 12)
			{
				if (chMode)
					chTemp = asc2_1206[chChr][i];
				else
					chTemp = ~asc2_1206[chChr][i];
			}
			else
			{
				if (chMode)
					chTemp = asc2_1608[chChr][i];
				else
					chTemp = ~asc2_1608[chChr][i];
			}
			for (j = 0; j < 8; j++)
			{
				if (chTemp & 0x80)
					LCD_drawPoint(x, y, 1, show);
				else
					LCD_drawPoint(x, y, 0, show);
				chTemp <<= 1;
				y++;
				if ((y - y0) == chSize)
				{
					y = y0;
					x++;
					break;
				}
			}
		}
	}
}

static void swap(uint8_t *a, uint8_t *b)
{
	uint8_t temp = *a;
	*a = *b;
	*b = temp;
}

void LCD_drawLineH(uint8_t x1, uint8_t x2, uint8_t y, uint8_t chPoint, uint8_t show)
{
	if (x1 > x2)
	{
		swap(&x1, &x2);
	}
	for (; x1 <= x2; x1++)
		LCD_drawPoint(x1, y, chPoint, show);
}

void LCD_drawLineV(uint8_t y1, uint8_t y2, uint8_t x, uint8_t chPoint, uint8_t show)
{
	if (y1 > y2)
	{
		swap(&y1, &y2);
	}
	for (; y1 <= y2; y1++)
		LCD_drawPoint(x, y1, chPoint, show);
}

void LCD_drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t chPoint, uint8_t show)
{
	int t, distance;
	int x = 0, y = 0, delta_x, delta_y;
	int incx, incy;
	delta_x = x2 - x1;
	delta_y = y2 - y1;
	if (delta_x > 0)
		incx = 1;
	else if (delta_x == 0)
	{
		LCD_drawLineV(x1, y1, y2, chPoint, show);
		return;
	}
	else
		incx = -1;
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
	{
		LCD_drawLineH(x1, x2, y1, chPoint, show);
		return;
	}
	else
		incy = -1;
	delta_x = delta_x < 0 ? -delta_x : delta_x;
	delta_y = delta_y < 0 ? -delta_y : delta_y;
	if (delta_x > delta_y)
		distance = delta_x;
	else
		distance = delta_y;
	LCD_drawPoint(x1, y1, chPoint, show);
	for (t = 0; t <= distance + 1; t++)
	{
		LCD_drawPoint(x1, y1, chPoint, show);
		x += delta_x;
		y += delta_y;
		if (x > distance)
		{
			x -= distance;
			x1 += incx;
		}
		if (y > distance)
		{
			y -= distance;
			y1 += incy;
		}
	}
}
void LCD_drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t chPoint, uint8_t show)
{
	uint8_t x;
	if (x1 > x2)
	{
		swap(&x1, &x2);
	}
	if (y1 > y2)
	{
		swap(&y1, &y2);
	}
	for (; y1 <= y2; y1++)
	{
		for (x = x1; x <= x2; x++)
		{
			LCD_drawPoint(x, y1, chPoint, show);
		}
	}
}

void LCD_drawCircle(uint8_t x, uint8_t y, uint8_t r, uint8_t chPoint, uint8_t show)
{
	unsigned char a, b;
	float c;
	a = 0;
	b = r;
	//  c = 1.25 - r;
	c = 3 - 2 * r;
	while (a < b)
	{
		LCD_drawPoint(x + a, y + b, chPoint, show);
		LCD_drawPoint(x - a, y + b, chPoint, show);
		LCD_drawPoint(x + a, y - b, chPoint, show);
		LCD_drawPoint(x - a, y - b, chPoint, show);
		LCD_drawPoint(x + b, y + a, chPoint, show);
		LCD_drawPoint(x - b, y + a, chPoint, show);
		LCD_drawPoint(x + b, y - a, chPoint, show);
		LCD_drawPoint(x - b, y - a, chPoint, show);
		if (c < 0)
			c = c + 4 * a + 6;
		else
		{
			c = c + 4 * (a - b) + 10;
			b -= 1;
		}
		a = a + 1; //控制打点间隔
	}
	if (a == b)
	{
		LCD_drawPoint(x + a, y + b, chPoint, show);
		LCD_drawPoint(x - a, y + b, chPoint, show);
		LCD_drawPoint(x + a, y - b, chPoint, show);
		LCD_drawPoint(x - a, y + b, chPoint, show);
		LCD_drawPoint(x + b, y + a, chPoint, show);
		LCD_drawPoint(x - b, y + a, chPoint, show);
		LCD_drawPoint(x + b, y - a, chPoint, show);
		LCD_drawPoint(x - b, y - a, chPoint, show);
	}
}
