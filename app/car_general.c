#include "car_general.h"
#if 1 //通用特殊函数★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★

/*
*  绝对值函数
*/
int abs(int _X)    /* OVERLOADS */
{       /* return absolute value */
  if(_X>=0)return _X;
  else return -_X;
}

/*
*  符号函数
*/
int sgn(int _X)    /* OVERLOADS */
{       /* return absolute value */
  if(_X>0)return 1;
  else if(_X<0) return -1;
  else return 0;
}

/*
*  乘方函数
*/
int power(uint8 length)
{
  int power=1;
  while(length--)power*=10;
  return power;
}

/*
*  打印文件返回代码
*/
void die(FRESULT rc)
{
  printf("错误代码 rc=%u.\n", rc);
  //for (;;) ;
}


#endif
