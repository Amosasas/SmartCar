#ifndef _CAR_GENERAL_H_
#define _CAR_GENERAL_H_
#include "common.h"
#include "global.h"

/*
*  ����ֵ����
*/
int abs(int _X);    /* OVERLOADS */
float abs_f(float _X);
/*
*  ���ź���
*/
int sgn(int _X);    /* OVERLOADS */

/*
*  �˷�����
*/
int power(uint8 length);

/*
*  ��ӡ�ļ����ش���
*/
void die(FRESULT rc);

/*
*  �û��Զ����ΪFatFsϵͳ�ṩʵʱʱ��ĺ���
*/
DWORD get_fattime (void);
#endif
