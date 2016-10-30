#ifndef __FMC__H_
#define __FMC__H_
#include "stm32f2xx.h"



void FMC_test(void);





void FMC_unlock(void);
void FMC_lock(void);

int FMC_write(uint8_t* buf, uint32_t len);

int FMC_read(uint8_t* buf, uint32_t len);


























#endif
