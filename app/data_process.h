#ifndef _DATA_PROCESS_H_
#define _DATA_PROCESS_H_
#include "common.h"
#include "global.h"

void Data_Process();
void Track_Amp();
void Track_Cal();
void Track_Out();
int Weight_Cal(int xvalue,int* setarray);
int Steer_PWM_Cal(int track_pos);
int Motor_PID_Cal(int input_speed,int setspeed);
int Speed_Cal(int track_pos);
#endif