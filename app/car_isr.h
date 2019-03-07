#ifndef _CAR_ISR_H_
#define _CAR_ISR_H_
#include "common.h"
#include "global.h"

void Portc_Isr();
void Pit0_Isr();
void Key_Check();
void Status_Select();
void Uart_Isr();

#endif
