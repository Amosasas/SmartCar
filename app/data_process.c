#include "data_process.h"

#define LH track.LH
#define RH track.RH
#define LF track.LF
#define RF track.RF
#define LV track.LV
#define RV track.RV
#define MH track.MH
#define MV track.MV
#define SumH LH+MH+RH //文本替换

void Data_Process()
{
  Track_Amp();//读取磁传感器
  Track_Cal();//计算当前偏离值
  Track_Out();//赛道状态更新
  outdata.Steer_PWM = Steer_PWM_Cal(track.Out);
 // outdata.Steer_PWM = setpara.Steer.mid;
  setpara.SetSpeed =  Speed_Cal(track.Out);
  outdata.Motor_PWM = Motor_PID_Cal(indata.Speed,setpara.SetSpeed);
}


void Track_Amp()
{
  track.LH0 = inductor.Ad[3];//0
  track.MH0 = inductor.Ad[7];//4
  track.RH0 = inductor.Ad[0]; //3
  track.AveH0 = (track.LH0+track.MH0+track.RH0)*2/3;
  if((LV+MV+RV)<=30 && abs(track.Out)<100)
    flag.STRAIGHT = 1;
  else
    flag.STRAIGHT = 0;
  //0是最新值 标志位和H0数据更新
  //flag.STRAIGHT = 1;
  for(int i=3;i>=0;i--)
  {
    track.AveH0Array[i+1] = track.AveH0Array[i];
    track.StrArray[i+1] = track.StrArray[i];
  }
  track.AveH0Array[0] = track.AveH0; 
  track.StrArray[0] = flag.STRAIGHT; 
  
  track.H0Sum = 0;
  track.StrNum = 0;
  //累加和判断
  for(int i=0;i<5;i++)
  {
    track.H0Sum += track.AveH0Array[i];
    if(track.StrArray[i] == 1)
      track.StrNum ++;
  }
  if(track.StrNum >= 5)
  {
    track.AmpK = 1500000/track.H0Sum;   // 测出较大的值的时候 放大倍数就会变小
    track.StrNum = 0;
  }
  if(mycar.Status==0)
  {
    track.AmpK = setpara.Ampk;
    track.TrackAmpDyc = setpara.Ampk;
  }
  else
  {
    track.TrackAmpDyc = track.AmpK;
    if(track.TrackAmpDyc > 2000)
      track.TrackAmpDyc = 2000;
    if(track.TrackAmpDyc < 0)
      track.TrackAmpDyc = 0;
  }
  LH = inductor.Ad[3]*track.TrackAmpDyc/300;//0   3
  RH = inductor.Ad[0]*track.TrackAmpDyc/300;//3   0
  LF = inductor.Ad[5]*track.TrackAmpDyc/300;//6   5
  RF = inductor.Ad[6]*track.TrackAmpDyc/300;//5   6
  MH = inductor.Ad[7]*track.TrackAmpDyc/300;
  MV = inductor.Ad[4]*track.TrackAmpDyc/300;
  LV = inductor.Ad[2]*track.TrackAmpDyc/300;//1   2
  RV = inductor.Ad[1]*track.TrackAmpDyc/300;//2   1
//  RH = inductor.Ad[3]*track.TrackAmpDyc/300;
//  LH = inductor.Ad[0]*track.TrackAmpDyc/300;
//  RF = inductor.Ad[5]*track.TrackAmpDyc/300;
//  LF = inductor.Ad[6]*track.TrackAmpDyc/300;
//  MH = inductor.Ad[7]*track.TrackAmpDyc/300;
//  MV = inductor.Ad[4]*track.TrackAmpDyc/300;
//  RV = inductor.Ad[1]*track.TrackAmpDyc/300;
//  LV = inductor.Ad[2]*track.TrackAmpDyc/300; 
}

void Track_Cal()
{
  status_track = STRAIGHT;
  track.OutLast = track.Out; 
  
  //串道现象处理，水平电感***************************************************************
 if(track.LockDir) //以前有锁住的状态下
  {
    if(SumH * 2/3 > setpara.LoseThreshold * 1.1) //现在测得值已经够大 解除锁弯的状态
    {  
      track.LockDir = 0;//★★
      track.L = 0;
      track.R = 0;
    }
  }
  else    //以前没有锁住的状态下
  {
    if(SumH * 2/3 < setpara.LoseThreshold) //测得值偏小的情况下 说明车体距离导线比较远 比较两端值大小 确认弯道的方向
      track.LockDir = ((LH + sqrtl(LF * LF + 2 * LV * LV)) > (RH + sqrtl(RF * RF+2 * RV * RV)) ? 1:2);
  }
  
  if(track.LockDir == 1 && LH < RH) //如果是锁住的情况 确认一下转弯的方向 并且调整电感值 左拐
  { //★★
    
    MH = LH / 2;
    RH = 0;
   // track.LockDir=-1;
    track.L=1;
  }  
  else if(track.LockDir == 2 && RH < LH) //同上
  {//★★
    LH = 0;
    MH = RH / 2;
    //track.LockDir=-2;
    track.R=1;
  }
 //长直道；垂直，track.out100,近端远端和300
  if((LV+MV+RV)<=30&&abs(track.Out)<=100&&track.FarSum<=300&&track.NearSum<=300)
  {
    status_track = LONGSTRAIGHT;//★★
  }
  
 //直角，所有电感和300
  track.IndSum=LH+MH+RH+LV+RV+MV+LF+RF;
  for(int i=4;i>0;i--) 
    track.INDSUM[i]=track.INDSUM[i-1];
  track.INDSUM[0]=track.IndSum;
  track.SUMSUM=track.INDSUM[0]+track.INDSUM[1]+track.INDSUM[2]+track.INDSUM[3]+track.INDSUM[4];//最后SUMSUM 
  if(track.SUMSUM<=5*setpara.RightThreshold)
  {
    status_track = RIGHT;//★★
  }
  //直道，track.out300
  if(status_track==RIGHT&&abs(track.Out<300))
  {
    status_track = STRAIGHT;//★★
  }
 //十字，垂直
  if((LV>450&&RV>450&&MV>450&&1)||(abs(indata.angle_real)>0&&abs(indata.angle_real)<=180)) 
  { //左中右都检测到赛道值了 并且角度没问题 说明在十字路口 
    status_track = CROSS;//★★
  }//测定十字
  if(status_track == CROSS)
  {
    indata.angle += indata.mpu6050.gyr_z; //角加速度积分求角度 
    indata.angle_real = (int)1.0*indata.angle*360/3400;//角度转换   这个算的是转角！！！！
    if(abs(indata.angle_real)-abs(indata.last_angle_real)<0)   //防十字误判
    {
      status_track = STRAIGHT;//★★
    }
    indata.last_angle_real = indata.angle_real;//
  }
  if(status_track == CROSS&&abs(indata.angle_real)>180)//转过180度
  {
    status_track = CROSS_OUT;//★★
  }
  if((LV+RV+MV<200)&&((LH+MH+RH)>1100)&&(status_track == CROSS_OUT))//当测得值是在中间的而且刚才标志的也出去了
  {
    status_track = STRAIGHT;//恢复直走
  }
  if(status_track == STRAIGHT)//直走之后标志恢复 角度恢复
  {
    indata.angle = 0;
    indata.angle_real = 0;
  }
 
  
  
  //导线距离算法***************************************************************
  //和之前无关，之前只是标记赛道，对应修正的速度区间
  //近端 
  track.WeightNear = (int)Weight_Cal(SumH * 2/3, setpara.WeightNear); //
  track.NearCal = (int)1000 * abs(RH - LH)/(SumH * 2/3 + 1);   //+1 是为了防止出现 除以0
  track.NearOut = (int)(1.0 * setpara.KNear * track.NearCal * track.WeightNear /100000);
  
  if(track.LockDir == 0) 
     track.NearOut *= sgn(RH-LH);                                  // track.NearOut *= sgn((RH + (int)sqrtl(RF * RF+2 * RV * RV))-(int)(LH + sqrtl(LF * LF+2 * LV * LV)));//右比左小  左打
  else if(track.LockDir==1) track.NearOut *= -1;
  
  track.WeightFar = (int)Weight_Cal(SumH * 2/3, setpara.WeightFar);
  track.FarCal = (int)Weight_Cal(SumH * 2/3, setpara.FarCal); 
  track.FarOut = (int)(1.0 * setpara.KFar * track.FarCal * track.WeightFar / 100000);     
  
  if(track.LockDir == 0) 
  track.FarOut *= sgn((RH + (int)sqrtl(RF * RF+2 * RV * RV))-(int)(LH + sqrtl(LF * LF+2 * LV * LV)));
  else if(track.LockDir == 1) track.FarOut *= -1;//左大于右
  
  track.Out = track.NearOut + track.FarOut;//负 左打
  track.Out=-track.Out;//负 右打
  
//  track.LANG = (int)100*LV/(LF+1);
//  track.RANG = (int)100*RV/(RF+1);
//  track.MANG = (int)100*MV/(MH+1);
  
}
//水平电感和把赛道分10个区域
//近端远端制造出一个和
//这个和 作为一个系数控制舵机KP，并转换为PWM，直接控制舵机转角大小
//动态KP与偏差和相关，并用系数与实际对应，其实就相当于积分
//静态KP与当前偏差相关，静态KD与偏差变化相关
//动态KP怕误差过大时，静态已不足以快速修正，需要增大P值
//然而又会有延时
int Weight_Cal(int xvalue,int* setarray) //分段函数
{
  track.Segment=xvalue/100;                    //分段区域
  if(track.Segment<11)
  {
    int position=xvalue-track.Segment*100;      //区域内位置
    if(track.Segment<=0) 
      track.Segment = 0;
    return (setarray[track.Segment]*(100-position)+setarray[track.Segment+1]*position)/100;
  }
  else
    return setarray[11];
}

void Track_Out()
{
  for(int ii=9;ii>=1;ii--)
    track.Near[10-ii]=track.Near[9-ii];
  track.Near[0]=track.NearOut;
  track.NearSum=track.Near[0]+track.Near[1]+track.Near[2]+
    track.Near[3]+track.Near[4]+track.Near[5]+track.Near[6]+
      track.Near[7]+track.Near[8]+track.Near[9];//track.NearSum   track.NearOut
  for(int jj=9;jj>=1;jj--)
    track.Far[10-jj]=track.Far[9-jj];
  track.Far[0]=track.FarOut;
  track.FarSum=track.Far[0]+track.Far[1]+track.Far[2]+
    track.Far[3]+track.Far[4]+track.Far[5]+track.Far[6]+
      track.Far[7]+track.Far[8]+track.Far[9];
}

int Steer_PWM_Cal(int track_pos)//舵机的角度
{
  static int last_track_pos;
  //动态计算PD
  outdata.SteerKp_change = setpara.SteerPD.Kpchange*abs(track.FarSum+track.NearSum)/10000;//动态P与偏差和成正比
  outdata.SteerKp_change = (outdata.SteerKp_change>setpara.SteerPD.Kpchange?setpara.SteerPD.Kpchange:outdata.SteerKp_change);
 
  outdata.Steer_PWM_Kp = (int)(1.0*(setpara.SteerPD.Kp+outdata.SteerKp_change) * track_pos)/100 ; //比率的值
  outdata.Steer_PWM_Kd = (int)(1.0*setpara.SteerPD.Kd*(track_pos-last_track_pos))/10;   //微分的值           
  
 
  int steertemp = outdata.Steer_PWM_Kp + outdata.Steer_PWM_Kd;//
  last_track_pos = track_pos;//
  steertemp = (steertemp>1000?1000:(steertemp<-1000?-1000:steertemp)); //需要整定        大的时候右 temp负的时候右打
  return setpara.Steer.mid-setpara.Steer.maxturn * steertemp/1000;//返回计算值  
}

 // setpara.SpeedDutyMax          =       4000;  
  

int Speed_Cal(int track_pos)
{
  static int speed;
  if(status_track==RIGHT)
  {
    setpara.SpeedMax = setpara.RightSpeedMax;
    setpara.SpeedMin = setpara.RightSpeedMin;
  }
  else if(status_track==LONGSTRAIGHT)
  {
    setpara.SpeedMax = setpara.LStraightMax;
    setpara.SpeedMin = setpara.LStraightMin;
  }
  else
  {
    setpara.SpeedMax = setpara.StraightMax;
    setpara.SpeedMin = setpara.StraightMin;
  }
  
  speed = abs(track_pos)*(setpara.SpeedMax-setpara.SpeedMin)/setpara.diff_slope;
  speed = setpara.SpeedMax - speed;
  speed = (speed<setpara.SpeedMin?setpara.SpeedMin:speed);
  return speed;
}



//电机PID  
int Motor_PID_Cal(int input_speed,int setspeed)
{
  static int powerout,error,last_error,last_last_error;
  error = setspeed - input_speed;
  int d_error = error-last_error;
  int dd_error = -2*last_error+error+last_last_error;
  powerout += (setpara.SpeedPID.Kp * d_error + setpara.SpeedPID.Ki * error + setpara.SpeedPID.Kd * dd_error);
  last_last_error = last_error;
  last_error = error;

  if(powerout>setpara.SpeedDutyMax||error>setpara.SpeedBANGBANG) //输出值调节 不要输出不能输出的值 SpeedBANGBANG=200 一个常数 不知为何
    powerout=setpara.SpeedDutyMax; //棒棒控制 
  else if(powerout<-setpara.SpeedDutyMin||error<-setpara.SpeedBANGBANG)
    powerout=-setpara.SpeedDutyMin;
  return powerout;//返回输出的值
}
