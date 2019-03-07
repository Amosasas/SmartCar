#include "common.h"
#include "global.h"

/*
*  全局变量声明
*/

uint32 T;                       //PIT计时器
STATUS_BUTTON status_button;    //按钮状态
STATUS_CAR_STRUCT mycar;        //小车总状态
indata_STRUCT indata;           //输入变量
outdata_STRUCT outdata;         //输出变量  
setpara_STRUCT setpara;         //设定参数
OLED_STRUCT oled;               //屏幕显示
track_STRUCT track;             //赛道计算值
STATUS_TRACK status_track;      //赛道类型
CONTROL_STRUCT control;
Inductor_STRUCT inductor;
Flag_STRUCT flag;


int16 OutData[4]; 
int32 save_ram_no=0;
uint8 RAM_BUFF[50][512]={0};
uint8 SEND_BUFF[512]={0};
int32 TO_SAVE_FLASH_NU = 0;



PARA_LIST_STRUCT paralist[100]=      //可调参数的调节精度
{
  {&setpara.KFar,"kFar",1},
  {&setpara.KNear,"kNear",1},   
  {&setpara.FarCal[0],"CAL_FAR[0]",10},
  {&setpara.FarCal[1],"CAL_FAR[1]",10},
  {&setpara.FarCal[2],"CAL_FAR[2]",10},
  {&setpara.FarCal[3],"CAL_FAR[3]",10},
  {&setpara.FarCal[4],"CAL_FAR[4]",10},
  {&setpara.FarCal[5],"CAL_FAR[5]",10},
  {&setpara.FarCal[6],"CAL_FAR[6]",10},
  {&setpara.FarCal[7],"CAL_FAR[7]",10},
  {&setpara.FarCal[8],"CAL_FAR[8]",10},
  {&setpara.FarCal[9],"CAL_FAR[9]",10},
  {&setpara.FarCal[10],"CAL_FAR[10]",10},
  {&setpara.FarCal[11],"CAL_FAR[11]",10},
  {&setpara.WeightNear[0],"WEI_NEAR[0]",10},
  {&setpara.WeightNear[1],"WEI_NEAR[1]",10},
  {&setpara.WeightNear[2],"WEI_NEAR[2]",10},
  {&setpara.WeightNear[3],"WEI_NEAR[3]",10},
  {&setpara.WeightNear[4],"WEI_NEAR[4]",10},
  {&setpara.WeightNear[5],"WEI_NEAR[5]",10},
  {&setpara.WeightNear[6],"WEI_NEAR[6]",10},
  {&setpara.WeightNear[7],"WEI_NEAR[7]",10},
  {&setpara.WeightNear[8],"WEI_NEAR[8]",10},
  {&setpara.WeightNear[9],"WEI_NEAR[9]",10},
  {&setpara.WeightNear[10],"WEI_NEAR[10]",10},
  {&setpara.WeightNear[11],"WEI_NEAR[11]",10},
  {&setpara.WeightFar[0],"WEI_FAR[0]",10},
  {&setpara.WeightFar[1],"WEI_FAR[1]",10},
  {&setpara.WeightFar[2],"WEI_FAR[2]",10},
  {&setpara.WeightFar[3],"WEI_FAR[3]",10},
  {&setpara.WeightFar[4],"WEI_FAR[4]",10},
  {&setpara.WeightFar[5],"WEI_FAR[5]",10},
  {&setpara.WeightFar[6],"WEI_FAR[6]",10},
  {&setpara.WeightFar[7],"WEI_FAR[7]",10},
  {&setpara.WeightFar[8],"WEI_FAR[8]",10},
  {&setpara.WeightFar[9],"WEI_FAR[9]",10},    
  {&setpara.WeightFar[10],"WEI_FAR[10]",10},
  {&setpara.WeightFar[11],"WEI_FAR[11]",10},
  {&setpara.SpeedBANGBANG,"BANGBANG",10},
  {&setpara.LoseThreshold,"LoseThreshold",10},
//  {&setpara.DebugStatus,"DebugStatus",1},
  {&setpara.SetTime,"SetTime",100},
  {&setpara.SteerPD.Kp,"SteerKp",1},
  {&setpara.SteerPD.Kd,"SteerKd",1},
  {&setpara.Steer.mid, "Steer.mid",1},
  {&setpara.Steer.maxturn,"Steer.maxturn",1},
  {&setpara.SteerPD.Kpchange,"SPD.Kpchange",1},
  {&setpara.SpeedPID.Ki,"Speed.Ki",1},
  {&setpara.SpeedPID.Kd,"Speed.Kd",1},
  {&setpara.SpeedPID.Kp,"Speed.Kp",1},
  {&setpara.RightSpeedMax,"RSpeedMax",2},
  {&setpara.RightSpeedMin,"RSpeedMin",2},
  {&setpara.LStraightMax,"LStraightMax",2},
  {&setpara.LStraightMin,"LStraightMin",2},
  {&setpara.StraightMax,"StraightMax",2},
  {&setpara.StraightMin,"StraightMin",2},
  {&setpara.Ampk,"AmpK",5},
  {0}
};


#if 1 //主函数★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void main (void)
{
  DisableInterrupts;
  Init_All();           //小车所有的初始化函数  
//  Read_Flash();         //调取上一次设定值
  EnableInterrupts;
 // flag.ToScope = 1;
  
  while(1)              //程序主循环
  {
    Key_Check();        //按键查询与响应
    Show_Oled();        //屏幕显示
  //  Write_SD();
  }
}
#endif 

/*
#if 1 //状态选择★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void Status_Select()
{
  if(mycar.Status == 1)
  {  
    mycar.RunTime += PIT0_MS;
    if(mycar.RunTime > 1000)
    {
      mycar.Status = 2;
      flag.SpeedClear = 0; 
      flag.DirectionClear = 0;
      mycar.RunTime = 0;
//      control.BalanceAngle = setpara.AccAngle/10;
      control.TargetSpeed = (int)(setpara.TargetSpeed * 1.5);
      control.SpeedPI.Ki = 0;
      mycar.RaceTime = 0;
    }
  }  
  else if(mycar.Status == 2)
  {
    mycar.RunTime += PIT0_MS;
    
    if(mycar.RunTime > setpara.SetTime * 1000 || mycar.Reed == 2)
    {
      mycar.Status = 3; 
      flag.SpeedClear = 0; 
      flag.DirectionClear = 0;
      mycar.RunTime = 0;
      mycar.Reed = 0;
    }
    else if(mycar.RunTime > setpara.AccT || (mycar.RunTime <= setpara.AccT && indata.Speed > setpara.TargetSpeed))
    {
      if (control.BalanceAngle * 10  - 2 > (float)setpara.BalanceAngle) 
        control.BalanceAngle -= 0.2;
      else control.BalanceAngle = (float)setpara.BalanceAngle / 10;
      
      if (control.TargetSpeed - 2 > setpara.TargetSpeed)
        control.TargetSpeed -= 2;
      else control.TargetSpeed = setpara.TargetSpeed;
      if(mycar.RunTime > setpara.AccT + 200)
      control.SpeedPI.Ki = setpara.SpeedPI.Ki;
    }
    else
    {
      if (control.BalanceAngle * 10  + 10 < (float)setpara.BalanceAngle+(float)setpara.AccAngle) 
        control.BalanceAngle += 1;
      else control.BalanceAngle = ((float)setpara.BalanceAngle+(float)setpara.AccAngle) / 10;
    }
  }  
}
#endif 
*/

