#ifndef __IR__H_
#define __IR__H_


#define PULSETABSIZE 200
#define IR_DATA_SIZE PULSETABSIZE*12 //12¸ö°´¼ü

#define KEY_ADDR_1 (PULSETABSIZE*0)
#define KEY_ADDR_2 (PULSETABSIZE*1)
#define KEY_ADDR_3 (PULSETABSIZE*2)
#define KEY_ADDR_4 (PULSETABSIZE*3)
#define KEY_ADDR_5 (PULSETABSIZE*4)
#define KEY_ADDR_6 (PULSETABSIZE*5)
#define KEY_ADDR_7 (PULSETABSIZE*6)
#define KEY_ADDR_8 (PULSETABSIZE*7)
#define KEY_ADDR_9 (PULSETABSIZE*8)
#define KEY_ADDR_10 (PULSETABSIZE*9)
#define KEY_ADDR_11 (PULSETABSIZE*10)
#define KEY_ADDR_12 (PULSETABSIZE*11)


void IR_init(void);
char* IR_getProto(void);
void IR_clearRecvData(void);
void IR_recvEnable(u8 v);
u8 IR_isStudy(void);
void IR_save(uint32_t WriteAddr);
void IR_read(uint32_t ReadAddr);
void IR_write(void);

#endif

