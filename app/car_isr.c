#include "car_isr.h"

#if 1 //�жϷ������������������������������������
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
  //���PTC2�����жϡ����ɻɹܴ���
    if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin2))
    {
        if(mycar.Reed == 0)
        {
            mycar.Reed = 1;//��һ�μ��
            mycar.RaceTime = T;
        }
        if(mycar.Reed == 1 && (T - mycar.RaceTime) > 2000)
        {
            mycar.RaceTime = T - mycar.RaceTime;
            mycar.Reed = 2;//�ڶ��μ�
        }
    }
    
  //���PTC3�����жϡ�����ť����
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin3))
  {
    status_button = PRESS;
  }
  //���PTC5�����жϡ�����ť��ת,˳ʱ����
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin5))
  {
    if(PTC7_I)  //���C7�˵�ƽ������Ǹߵ�ƽ��������ת
    {
      status_button = CW;
      //��ת��������Ӧ
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
      //��ת��������Ӧ
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
  //���PTC6�����жϡ������ְ���
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin6))
  {
    status_button = PUSH;
  }
  //���PTC4�����жϡ�����������    
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin4))
  {
    status_button = UP;
  }
  //���PTC8�����жϡ�����������
  if(LPLD_GPIO_IsPinxExt(PORTC, GPIO_Pin8))
  {
    status_button = DOWN;
  }
}

void Key_Check()
{
  //��¼����ʱ��
  uint32 pushtime=T;
  
  //��ť���ְ��²�������Ļ��ʼ�������޸�����
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
      oled.ChangePara ^= 1;    //״̬ȡ��
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
    //  Init_Setpara();       //��ʼ�����в���
      mycar.Status = 2;
    }
    break;
  case UP:
    while(!PTC4_I);
    if(T-pushtime<300)
    {
      if(oled.ChangePara)   //�����ı�״̬
      {
        if(oled.ParaSelect >0)
          oled.ParaSelect --;
        else
          oled.ParaSelect = oled.ParaNum-1;
      }
      else                 //����ѡ��״̬
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
      if(oled.ChangePara)   //�����ı�״̬
      {
        if(oled.ParaSelect <oled.ParaNum-1)
          oled.ParaSelect ++;
        else
          oled.ParaSelect = 0;
      }
      else                 //����ѡ��״̬
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
      mycar.Status = 2;  //��ʱ�����°α��2
    }
    break;
  default:
    break;
  }
  //�������״̬
  status_button = NONE;
}

void Pit0_Isr()    //5����һ���ж�
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
