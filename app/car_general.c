#include "car_general.h"
#if 1 //ͨ�����⺯���������������������������������

/*
*  ����ֵ����
*/
int abs(int _X)    /* OVERLOADS */
{       /* return absolute value */
  if(_X>=0)return _X;
  else return -_X;
}

/*
*  ���ź���
*/
int sgn(int _X)    /* OVERLOADS */
{       /* return absolute value */
  if(_X>0)return 1;
  else if(_X<0) return -1;
  else return 0;
}

/*
*  �˷�����
*/
int power(uint8 length)
{
  int power=1;
  while(length--)power*=10;
  return power;
}

/*
*  ��ӡ�ļ����ش���
*/
void die(FRESULT rc)
{
  printf("������� rc=%u.\n", rc);
  //for (;;) ;
}


#endif
