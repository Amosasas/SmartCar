#ifndef _DEBUG_SD_H_
#define _DEBUG_SD_H_

#include "common.h"
#include "global.h"

void Send_Int(int value,char* name);
void Send_Array(int* array,char* name);
void Send_Tab_All();
void Save_Flash();
void Read_Flash();
void Save_RAM();
void Save_SD(int value,char* label);
void Data_Save();
void Write_SD();

#endif