#ifndef _DATA_OUTPUT_H_
#define _DATA_OUTPUT_H_
#include "common.h"
#include "global.h"

void Car_Protect();
void Output_Control();
void PWM_Output(uint8 Status);
void PWM_Output_TestMotor();
void PWM_Output_TestSteer();

#endif