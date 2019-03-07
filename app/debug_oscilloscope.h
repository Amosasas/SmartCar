#ifndef _DEBUG_OSCILLOSCOPE_H_
#define _DEBUG_OSCILLOSCOPE_H_
#include "common.h"
#include "global.h"

unsigned short Crc_Check(unsigned char *Buf, unsigned char CRC_CNT);
void UART_SendData(uint8_t Data);
void OutPut_Data(void);
void Send_UART();

#endif
