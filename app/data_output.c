#include "data_output.h"


//void PWM_Output(uint8 status)
//{
//    if(outdata.Speed<0)
//    {
//      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5,(int)-outdata.Speed);
//      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4,0);
//    }
//    else
//    {
//      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5,0);
//      LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4,(int)outdata.Speed);
//    }
//    LPLD_FTM_PWM_ChangeDuty(FTM2, FTM_Ch0,outdata.Steer_PWM);
//}

void PWM_Output_TestSteer()
{
  //if(outdata.Steer_PWM>)//PROTECT
    LPLD_FTM_PWM_ChangeDuty(FTM2, FTM_Ch0,outdata.Steer_PWM);
 // LPLD_FTM_PWM_ChangeDuty(FTM2, FTM_Ch0,setpara.Steer.mid);
}

void PWM_Output_TestMotor()
{
     if(outdata.Motor_PWM>=0)
     {
       LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4,outdata.Motor_PWM);
       LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5,0);   
     }
     else
     {
       LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch4,0);
       LPLD_FTM_PWM_ChangeDuty(FTM0, FTM_Ch5,-outdata.Motor_PWM);  
     }
}