#ifndef _DATA_INPUT_H_
#define _DATA_INPUT_H_
#include "common.h"
#include "global.h"

void Data_Input();
uint16 Battery_Check();
int Get_Speed();
int Get_Count();
void Get_Ad_Real(int32* ad);
void Get_Ad_New(int32* ad);

#endif
