#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "common.h"
#include "math.h"
#include "car_init.h"
#include "car_isr.h"
#include "car_general.h"
#include "car_show.h"
#include "debug_sd.h"
#include "data_input.h"
#include "data_process.h"
#include "data_output.h"


#define PIT0_MS 5
#define MAXPWM 9000
#define AXIS_ANGLE                  360/32768 //加速度计归一化常数，归一化后单位为度
#define GRAVITY_ADJUST_TIME         0.06    //Tg融合放大倍数
#define ANGLE_INTEGRAL              0.005    //单位为毫秒，是积分时间
#define SPEED_CONTROL_PERIOD        10
#define DIRECTION_CONTROL_PERIOD    2    
#define SD_BASE                     0

#define FLASH_SAVE_SECTOR 255
#define BLOCK_LEN     2
#define BUFFER_SIZE   512*BLOCK_LEN
#define A  200 
#define Vp 340 
#define PWM_DEADZONE 5670
#define PWM_ACTIVEZONE 3000
#define CAR_SPEED_CONSTANT 0.0390625  //单位转为：转/秒



/*
*  结构体定义
*/

typedef enum STATUS_TRACK
{
  OUT=0,      
  STRAIGHT,    //直道
  CROSS,       //十字
  CROSS_OUT,
  LONGSTRAIGHT,
  RIGHT,  //直角
  CRUVE_SMALL,//小S
  CRUVE_BIG,  //大S
        
  BREAK,      //折角
 
  
}STATUS_TRACK;

typedef enum STATUS_BUTTON
{
  NONE=0,
  PRESS,
  CW,
  CCW,
  UP,
  DOWN,
  PUSH//加不加逗号
}STATUS_BUTTON;

typedef struct STATUS_CAR_STRUCT
{
  uint8 Status;         //小车运行状态 
  uint32 RunTime;       //小车运行时间
  uint32 RaceTime;       //小车运行时间
  uint16 BattVolt;     //电池电压
  uint16 BatteryCount;
  int Speed;
  int Reed;
  int UART;
}STATUS_CAR_STRUCT;

typedef struct indata_STRUCT
{
  MPU6050_DATA_STRUCT mpu6050;  
  int32 Speed;
  int angle;
  int angle_real;
  int last_angle_real;
}indata_STRUCT;

typedef struct Inductor_STRUCT
{
  int32 Ad[8];
  int32 AdReal[8];
  int32 AdOld[8];
}Inductor_STRUCT;

typedef struct setpara_STRUCT
{
  struct 
  {
    int Kp;
    int Ki;
    int Kd;
  }SpeedPID;//未调整
  struct
  {
    int Kp;
    int Kd;
    int Kpchange;
  }SteerPD;//未调整
  struct
  {
    int mid;
    int maxturn;
  }Steer;//
  int TargetSPDMax;
  int TargetSPDMin;
  int SetSpeed;
  int SpeedBANGBANG;  
  int SpeedDutyMax;
  int SpeedDutyMin;
  int SpeedMax;
  int SpeedMin;
  int RightSpeedMax;
  int RightSpeedMin;
  int LStraightMax;
  int LStraightMin;
  int StraightMax;
  int StraightMin;
  
  int diff_slope;
  int SetTime;
  int LoseThreshold;
  int RightThreshold;
  int WeightNear[12];
  int WeightFar[12];
  int FarCal[12];
  int KNear;
  int KFar;
  //int DebugStatus;
  int Status;
  int Ampk;
}setpara_STRUCT;

typedef struct track_STRUCT
{   
  int L;
  int R;
  int LH0;
  int MH0;
  int RH0;
  int AveH0;
  int AveH0Array[5];
  int H0Sum;
  int StrArray[5];
  int StrNum;
  int AmpK;
  int TrackAmpDyc;
  int LH;
  int RH;
  int LF;
  int RF;
  int LV;
  int RV;
  int MH;
  int MV;
  int LockDir;
  int OutLast;
  int Out;
  int NearCal;
  int NearOut;
  int Near[10];
  int NearSum;
  int FarCal;
  int FarOut;
  int Far[10];
  int FarSum;
  int INDSUM[5];
  int SUMSUM;
  int IndSum;
  int WeightNear;
  int WeightFar;
  int8 Segment;
  int LANG;
  int MANG;
  int RANG;
}track_STRUCT;

typedef struct outdata_STRUCT
{
 // int Speed;
  int Steer_PWM;
  int Steer_PWM_Kd;
  int Steer_PWM_Kp;
  int SteerKp_change;
  int Motor_PWM;
}outdata_STRUCT;

typedef struct CONTROL_STRUCT
{
  int BalanceKp;
  int BalanceKd;
  float BalanceAngle;
  int TargetSpeed;
  float SpeedControlIntegral;    //速度控制的积分值
  float SpeedControlOld;      //上一次速度控制输出值
  float SpeedControlNew;      //这一次算出的速度控制值
  uint8 SpeedControlCount;           //中断函数中的速度控制计数值
  uint8 DirectionControlCount;
  float DirectionControlNew;
  float DirectionControlOld;
  int DirP;
  int DirBase;
  int DirKdDiff;
  uint8 CarACCNum;
  uint8 CarACC_F;
  uint8 AngleRecover;
  int DirGyroP;
  struct 
  {
    int Kp;
    int Ki;
  }SpeedPI;
}CONTROL_STRUCT;

typedef struct OLED_STRUCT
{
  uint8 ChangePara;
  uint16 ParaSelect;
  uint16 ParaNum;
  int8 ShowPage;
  int8 ShowPageMax;
  int8 ShowPageMin;
}OLED_STRUCT;

typedef struct PARA_LIST_STRUCT
{
  int* para;
  int8 label[13];
  uint8 precision;
}PARA_LIST_STRUCT;

typedef struct Flag_STRUCT
{
  uint8 ToSendSD;     //发送SD卡中的数据
  uint8 ToSendArray;  //发送可调参数列表中的数据
  uint8 ToScope;
  int STRAIGHT;
  
}Flag_STRUCT;


extern uint32 T;                       //PIT计时器
extern STATUS_BUTTON status_button;    //按钮状态
extern STATUS_CAR_STRUCT mycar;        //小车总状态
extern indata_STRUCT indata;           //输入变量
extern outdata_STRUCT outdata;         //输出变量  
extern setpara_STRUCT setpara;         //设定参数
extern OLED_STRUCT oled;               //屏幕显示
extern track_STRUCT track;             //赛道计算值
extern STATUS_TRACK status_track;      //赛道类型 
extern PARA_LIST_STRUCT paralist[100];
extern CONTROL_STRUCT control;
extern Inductor_STRUCT inductor;
extern Flag_STRUCT flag;
extern int16 OutData[]; 
extern int32 save_ram_no;
extern uint8 RAM_BUFF[50][512];
extern uint8 SEND_BUFF[];
extern int32 TO_SAVE_FLASH_NU;

#endif
