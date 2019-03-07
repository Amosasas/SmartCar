#include "data_process.h"

#define LH track.LH
#define RH track.RH
#define LF track.LF
#define RF track.RF
#define LV track.LV
#define RV track.RV
#define MH track.MH
#define MV track.MV
#define SumH LH+MH+RH //�ı��滻

void Data_Process()
{
  Track_Amp();//��ȡ�Ŵ�����
  Track_Cal();//���㵱ǰƫ��ֵ
  Track_Out();//����״̬����
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
  //0������ֵ ��־λ��H0���ݸ���
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
  //�ۼӺ��ж�
  for(int i=0;i<5;i++)
  {
    track.H0Sum += track.AveH0Array[i];
    if(track.StrArray[i] == 1)
      track.StrNum ++;
  }
  if(track.StrNum >= 5)
  {
    track.AmpK = 1500000/track.H0Sum;   // ����ϴ��ֵ��ʱ�� �Ŵ����ͻ��С
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
  
  //����������ˮƽ���***************************************************************
 if(track.LockDir) //��ǰ����ס��״̬��
  {
    if(SumH * 2/3 > setpara.LoseThreshold * 1.1) //���ڲ��ֵ�Ѿ����� ��������״̬
    {  
      track.LockDir = 0;//���
      track.L = 0;
      track.R = 0;
    }
  }
  else    //��ǰû����ס��״̬��
  {
    if(SumH * 2/3 < setpara.LoseThreshold) //���ֵƫС������� ˵��������뵼�߱Ƚ�Զ �Ƚ�����ֵ��С ȷ������ķ���
      track.LockDir = ((LH + sqrtl(LF * LF + 2 * LV * LV)) > (RH + sqrtl(RF * RF+2 * RV * RV)) ? 1:2);
  }
  
  if(track.LockDir == 1 && LH < RH) //�������ס����� ȷ��һ��ת��ķ��� ���ҵ������ֵ ���
  { //���
    
    MH = LH / 2;
    RH = 0;
   // track.LockDir=-1;
    track.L=1;
  }  
  else if(track.LockDir == 2 && RH < LH) //ͬ��
  {//���
    LH = 0;
    MH = RH / 2;
    //track.LockDir=-2;
    track.R=1;
  }
 //��ֱ������ֱ��track.out100,����Զ�˺�300
  if((LV+MV+RV)<=30&&abs(track.Out)<=100&&track.FarSum<=300&&track.NearSum<=300)
  {
    status_track = LONGSTRAIGHT;//���
  }
  
 //ֱ�ǣ����е�к�300
  track.IndSum=LH+MH+RH+LV+RV+MV+LF+RF;
  for(int i=4;i>0;i--) 
    track.INDSUM[i]=track.INDSUM[i-1];
  track.INDSUM[0]=track.IndSum;
  track.SUMSUM=track.INDSUM[0]+track.INDSUM[1]+track.INDSUM[2]+track.INDSUM[3]+track.INDSUM[4];//���SUMSUM 
  if(track.SUMSUM<=5*setpara.RightThreshold)
  {
    status_track = RIGHT;//���
  }
  //ֱ����track.out300
  if(status_track==RIGHT&&abs(track.Out<300))
  {
    status_track = STRAIGHT;//���
  }
 //ʮ�֣���ֱ
  if((LV>450&&RV>450&&MV>450&&1)||(abs(indata.angle_real)>0&&abs(indata.angle_real)<=180)) 
  { //�����Ҷ���⵽����ֵ�� ���ҽǶ�û���� ˵����ʮ��·�� 
    status_track = CROSS;//���
  }//�ⶨʮ��
  if(status_track == CROSS)
  {
    indata.angle += indata.mpu6050.gyr_z; //�Ǽ��ٶȻ�����Ƕ� 
    indata.angle_real = (int)1.0*indata.angle*360/3400;//�Ƕ�ת��   ��������ת�ǣ�������
    if(abs(indata.angle_real)-abs(indata.last_angle_real)<0)   //��ʮ������
    {
      status_track = STRAIGHT;//���
    }
    indata.last_angle_real = indata.angle_real;//
  }
  if(status_track == CROSS&&abs(indata.angle_real)>180)//ת��180��
  {
    status_track = CROSS_OUT;//���
  }
  if((LV+RV+MV<200)&&((LH+MH+RH)>1100)&&(status_track == CROSS_OUT))//�����ֵ�����м�Ķ��Ҹղű�־��Ҳ��ȥ��
  {
    status_track = STRAIGHT;//�ָ�ֱ��
  }
  if(status_track == STRAIGHT)//ֱ��֮���־�ָ� �ǶȻָ�
  {
    indata.angle = 0;
    indata.angle_real = 0;
  }
 
  
  
  //���߾����㷨***************************************************************
  //��֮ǰ�޹أ�֮ǰֻ�Ǳ����������Ӧ�������ٶ�����
  //���� 
  track.WeightNear = (int)Weight_Cal(SumH * 2/3, setpara.WeightNear); //
  track.NearCal = (int)1000 * abs(RH - LH)/(SumH * 2/3 + 1);   //+1 ��Ϊ�˷�ֹ���� ����0
  track.NearOut = (int)(1.0 * setpara.KNear * track.NearCal * track.WeightNear /100000);
  
  if(track.LockDir == 0) 
     track.NearOut *= sgn(RH-LH);                                  // track.NearOut *= sgn((RH + (int)sqrtl(RF * RF+2 * RV * RV))-(int)(LH + sqrtl(LF * LF+2 * LV * LV)));//�ұ���С  ���
  else if(track.LockDir==1) track.NearOut *= -1;
  
  track.WeightFar = (int)Weight_Cal(SumH * 2/3, setpara.WeightFar);
  track.FarCal = (int)Weight_Cal(SumH * 2/3, setpara.FarCal); 
  track.FarOut = (int)(1.0 * setpara.KFar * track.FarCal * track.WeightFar / 100000);     
  
  if(track.LockDir == 0) 
  track.FarOut *= sgn((RH + (int)sqrtl(RF * RF+2 * RV * RV))-(int)(LH + sqrtl(LF * LF+2 * LV * LV)));
  else if(track.LockDir == 1) track.FarOut *= -1;//�������
  
  track.Out = track.NearOut + track.FarOut;//�� ���
  track.Out=-track.Out;//�� �Ҵ�
  
//  track.LANG = (int)100*LV/(LF+1);
//  track.RANG = (int)100*RV/(RF+1);
//  track.MANG = (int)100*MV/(MH+1);
  
}
//ˮƽ��кͰ�������10������
//����Զ�������һ����
//����� ��Ϊһ��ϵ�����ƶ��KP����ת��ΪPWM��ֱ�ӿ��ƶ��ת�Ǵ�С
//��̬KP��ƫ�����أ�����ϵ����ʵ�ʶ�Ӧ����ʵ���൱�ڻ���
//��̬KP�뵱ǰƫ����أ���̬KD��ƫ��仯���
//��̬KP��������ʱ����̬�Ѳ����Կ�����������Ҫ����Pֵ
//Ȼ���ֻ�����ʱ
int Weight_Cal(int xvalue,int* setarray) //�ֶκ���
{
  track.Segment=xvalue/100;                    //�ֶ�����
  if(track.Segment<11)
  {
    int position=xvalue-track.Segment*100;      //������λ��
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

int Steer_PWM_Cal(int track_pos)//����ĽǶ�
{
  static int last_track_pos;
  //��̬����PD
  outdata.SteerKp_change = setpara.SteerPD.Kpchange*abs(track.FarSum+track.NearSum)/10000;//��̬P��ƫ��ͳ�����
  outdata.SteerKp_change = (outdata.SteerKp_change>setpara.SteerPD.Kpchange?setpara.SteerPD.Kpchange:outdata.SteerKp_change);
 
  outdata.Steer_PWM_Kp = (int)(1.0*(setpara.SteerPD.Kp+outdata.SteerKp_change) * track_pos)/100 ; //���ʵ�ֵ
  outdata.Steer_PWM_Kd = (int)(1.0*setpara.SteerPD.Kd*(track_pos-last_track_pos))/10;   //΢�ֵ�ֵ           
  
 
  int steertemp = outdata.Steer_PWM_Kp + outdata.Steer_PWM_Kd;//
  last_track_pos = track_pos;//
  steertemp = (steertemp>1000?1000:(steertemp<-1000?-1000:steertemp)); //��Ҫ����        ���ʱ���� temp����ʱ���Ҵ�
  return setpara.Steer.mid-setpara.Steer.maxturn * steertemp/1000;//���ؼ���ֵ  
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



//���PID  
int Motor_PID_Cal(int input_speed,int setspeed)
{
  static int powerout,error,last_error,last_last_error;
  error = setspeed - input_speed;
  int d_error = error-last_error;
  int dd_error = -2*last_error+error+last_last_error;
  powerout += (setpara.SpeedPID.Kp * d_error + setpara.SpeedPID.Ki * error + setpara.SpeedPID.Kd * dd_error);
  last_last_error = last_error;
  last_error = error;

  if(powerout>setpara.SpeedDutyMax||error>setpara.SpeedBANGBANG) //���ֵ���� ��Ҫ������������ֵ SpeedBANGBANG=200 һ������ ��֪Ϊ��
    powerout=setpara.SpeedDutyMax; //�������� 
  else if(powerout<-setpara.SpeedDutyMin||error<-setpara.SpeedBANGBANG)
    powerout=-setpara.SpeedDutyMin;
  return powerout;//���������ֵ
}
