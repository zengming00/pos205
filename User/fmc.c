#include "fmc.h"


//只能进行扇区编程，所以只使用第3扇区（共16K）
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_3  //从第48K开始 
#define FLASH_USER_LEN     (ADDR_FLASH_SECTOR_4 - ADDR_FLASH_SECTOR_3)   


/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */



 
uint32_t GetSector(uint32_t Address);


void FMC_unlock(){
  /* Unlock the Flash to enable the flash control register access *************/ 
  FLASH_Unlock();

  /* Clear pending flags (if any) */  
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 
}

void FMC_lock(){
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  FLASH_Lock(); 
}


int FMC_write(uint8_t* buf, uint32_t len){
  uint32_t i;
  uint32_t addr = FLASH_USER_START_ADDR;
  uint32_t sec = GetSector(FLASH_USER_START_ADDR);

  if(len>FLASH_USER_LEN) return -1;//超出范围
  
  //擦除扇区
  if (FLASH_EraseSector(sec, VoltageRange_3) != FLASH_COMPLETE){ 
    return -1;
  }
  for (i=0; i<len; i++){
    if (FLASH_ProgramByte(addr, *buf) != FLASH_COMPLETE){
	  return -1;
    }
    addr++;
	buf++;
  }
  return 0;
}

int FMC_read(uint8_t* buf, uint32_t len){
  uint32_t i;
  uint8_t* addr = (uint8_t*)FLASH_USER_START_ADDR;
  
  if(len>FLASH_USER_LEN) return -1;
  for (i=0; i<len; i++){
    *buf = *addr;
	buf++;
	addr++;
  }
  return 0;
}

void FMC_test(void)
{  
  uint8_t buf[10] = {11,22,33,4,5,6,7,8,9,10};
  uint8_t buf2[10];
  
  
  FMC_read(buf2, 10);
  
  FMC_unlock();
  FMC_write(buf, 10);
  FMC_lock();
  
  FMC_read(buf2, 10);
 
  while (1)
  {
  }
}

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_Sector_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_Sector_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_Sector_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_Sector_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_Sector_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_Sector_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_Sector_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_Sector_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_Sector_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_Sector_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_Sector_10;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
  {
    sector = FLASH_Sector_11;  
  }

  return sector;
}





































