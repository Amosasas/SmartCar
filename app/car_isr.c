#include "car_isr.h"

#if 1 //中断服务函数★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
//void Uart_Isr()
//{
//  uint8 DeltaV;
//  DeltaV = LPLD_UART_GetChar(UART0);
//  if(DeltaV == (6+48))
//  {
//    mycar.Status = 3; 
//    flag.SpeedClear = 0; 
//    flag.DirectionClear = 0;
//    mycar.RunTime = 0;
//  }
//}
void Uart_Isr()
{  
  if(LPLD_UART_GetChar(UART0)=='1')
  {
  
   }
}

void Portc_Isr()
{
  //如果PTC2产生中断——干簧管触发
    if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin2))
    {
        if(mycar.Reed == 0)
        {
            mycar.Reed = 1;//第一次检测
            mycar.RaceTime = T;
        }
        if(mycar.Reed == 1 && (T - mycar.RaceTime) > 2000)
        {
            mycar.RaceTime = T - mycar.RaceTime;
            mycar.Reed = 2;//第二次检
        }
    }
    
  //如果PTC3产生中断——旋钮按下
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin3))
  {
    status_button = PRESS;
  }
  //如果PTC5产生中断——旋钮旋转,顺时针先
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin5))
  {
    if(PTC7_I)  //检查C7端电平，如果是高电平，则是正转
    {
      status_button = CW;
      //旋转编码器响应
      if(oled.ChangePara)
        *paralist[oled.ParaSelect].para += paralist[oled.ParaSelect].precision;
      else
      {
        if(oled.ParaSelect <oled.ParaNum-1)
          oled.ParaSelect ++;
        else
          oled.ParaSelect = 0;
      }
    }
    else
    {
      status_button = CCW;
      //旋转编码器响应
      if(oled.ChangePara)
        *paralist[oled.ParaSelect].para -= paralist[oled.ParaSelect].precision;
      else
      {
        if(oled.ParaSelect >0)
          oled.ParaSelect --;
        else
          oled.ParaSelect = oled.ParaNum-1;
      }
    }
  }
  //如果PTC6产生中断——拨轮按下
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin6))
  {
    status_button = PUSH;
  }
  //如果PTC4产生中断——拨轮向上    
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin4))
  {
    status_button = UP;
  }
  //如果PTC8产生中断——拨轮向下
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin8))
  {
    status_button = DOWN;
  }
}

void Key_Check()
{
  //记录按键时间
  uint32 pushtime=T;
  
  //旋钮或拨轮按下操作后屏幕初始化，以修复花屏
  if(status_button==PRESS||status_button==PUSH)
  {
    OLED_Init(1);        
    mycar.Status=0;
  }
  switch(status_button)
  {
  case PRESS:
    while(!PTC3_I);
    if(T-pushtime<300)
    {
      oled.ChangePara ^= 1;    //状态取反
    }
    else
    {
      Save_Flash();
      int start_time_1=T;
      while(T-start_time_1<1000);
      mycar.Status = 1;
      mycar.RunTime = 0;
      save_ram_no = 0;
    }
    break;
  case PUSH: 
    while(!PTC6_I);
    if(T-pushtime<800)
    {
      
    }
    else
    {
    //  Init_Setpara();       //初始化所有参数
      mycar.Status = 2;
    }
    break;
  case UP:
    while(!PTC4_I);
    if(T-pushtime<300)
    {
      if(oled.ChangePara)   //参数改变状态
      {
        if(oled.ParaSelect >0)
          oled.ParaSelect --;
        else
          oled.ParaSelect = oled.ParaNum-1;
      }
      else                 //参数选择状态
      {
        if(oled.ShowPage > oled.ShowPageMin)
          oled.ShowPage --;
        else
          oled.ShowPage = oled.ShowPageMax;
      }
    }
    else
    {
      if (mycar.Status == 0) flag.ToSendArray = 1;
      else if(mycar.Status == 2) flag.ToSendSD = 1;
    }
    break;  
  case DOWN:
    while(!PTC8_I);
    if(T-pushtime<300)
    {
      if(oled.ChangePara)   //参数改变状态
      {
        if(oled.ParaSelect <oled.ParaNum-1)
          oled.ParaSelect ++;
        else
          oled.ParaSelect = 0;
      }
      else                 //参数选择状态
      {
        if(oled.ShowPage < oled.ShowPageMax)
          oled.ShowPage ++;
        else
          oled.ShowPage = oled.ShowPageMin;
      }
    }
    else
    {
      Save_Flash();
      int start_time_1=T;
      while(T-start_time_1<1000);
      mycar.Status = 2;  //长时间往下拔变成2
    }
    break;
  default:
    break;
  }
  //清除按键状态
  status_button = NONE;
}

void Pit0_Isr()    //5毫秒一次中断
{  
  T+=PIT0_MS;
  mycar.RunTime+=PIT0_MS;
 // Status_Select();
  Data_Input();
  Data_Process();
  if(mycar.RunTime<setpara.SetTime&&mycar.Status==1&&mycar.Reed!=2)
  {
   PWM_Output_TestMotor();
   PWM_Output_TestSteer();  
  }
  else
 {
   LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5,0);
   LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4,0);
   PWM_Output_TestSteer();
  }
//  if(mycar.RunTime%200==0)
//    Data_Save();
}


#endif
