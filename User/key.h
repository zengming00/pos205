#ifndef __MY_KEY_H
#define __MY_KEY_H

#include <stm32f2xx.h>

#define KEY_POWER  10
#define KEY_BT     0
#define KEY_CLEAR  5
#define KEY_1      9
#define KEY_2      4
#define KEY_3      6
#define KEY_4      8
#define KEY_5      13
#define KEY_6      12
#define KEY_7      2
#define KEY_8      7
#define KEY_9      15
#define KEY_CANCEL 14
#define KEY_0      3
#define KEY_OK     1

void keyInit(void);
int keyScan(void);







#endif

