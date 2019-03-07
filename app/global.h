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
#define AXIS_ANGLE                  360/32768 //���ٶȼƹ�һ����������һ����λΪ��
#define GRAVITY_ADJUST_TIME         0.06    //Tg�ںϷŴ���
#define ANGLE_INTEGRAL              0.005    //��λΪ���룬�ǻ���ʱ��
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
#define CAR_SPEED_CONSTANT 0.0390625  //��λתΪ��ת/��



/*
*  �ṹ�嶨��
*/

typedef enum STATUS_TRACK
{
  OUT=0,      
  STRAIGHT,    //ֱ��
  CROSS,       //ʮ��
  CROSS_OUT,
  LONGSTRAIGHT,
  RIGHT,  //ֱ��
  CRUVE_SMALL,//СS
  CRUVE_BIG,  //��S
        
  BREAK,      //�۽�
 
  
}STATUS_TRACK;

typedef enum STATUS_BUTTON
{
  NONE=0,
  PRESS,
  CW,
  CCW,
  UP,
  DOWN,
  PUSH//�Ӳ��Ӷ���
}STATUS_BUTTON;

typedef struct STATUS_CAR_STRUCT
{
  uint8 Status;         //С������״̬ 
  uint32 RunTime;       //С������ʱ��
  uint32 RaceTime;       //С������ʱ��
  uint16 BattVolt;     //��ص�ѹ
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
  }SpeedPID;//δ����
  struct
  {
    int Kp;
    int Kd;
    int Kpchange;
  }SteerPD;//δ����
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
  float SpeedControlIntegral;    //�ٶȿ��ƵĻ���ֵ
  float SpeedControlOld;      //��һ���ٶȿ������ֵ
  float SpeedControlNew;      //��һ��������ٶȿ���ֵ
  uint8 SpeedControlCount;           //�жϺ����е��ٶȿ��Ƽ���ֵ
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
  uint8 ToSendSD;     //����SD���е�����
  uint8 ToSendArray;  //���Ϳɵ������б��е�����
  uint8 ToScope;
  int STRAIGHT;
  
}Flag_STRUCT;


extern uint32 T;                       //PIT��ʱ��
extern STATUS_BUTTON status_button;    //��ť״̬
extern STATUS_CAR_STRUCT mycar;        //С����״̬
extern indata_STRUCT indata;           //�������
extern outdata_STRUCT outdata;         //�������  
extern setpara_STRUCT setpara;         //�趨����
extern OLED_STRUCT oled;               //��Ļ��ʾ
extern track_STRUCT track;             //��������ֵ
extern STATUS_TRACK status_track;      //�������� 
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
