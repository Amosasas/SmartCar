#ifndef _CAR_INIT_H_
#define _CAR_INIT_H_

#include "common.h"
#include "global.h"

void Init_All();
void Init_Setpara();  
void Init_Paranum();
void Init_Key(); 
void Init_Motor();
void Init_Steer();
void Init_ADC();
void Init_PIT();
void Init_I2C();
void Init_Flash();
void Init_Sdhc();
void Init_All_Pulse_Counter();
void Init_Pulse_Counter(PortPinsEnum_Type counter_input,
                        uint8 dma_chx,
                        PortPinsEnum_Type dir_input);
void Init_Uart_Interr();
void Init_Temporary();
#endif