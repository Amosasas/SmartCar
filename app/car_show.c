#include "car_show.h"

void Show_Oled()
{
  if(oled.ShowPage>=0)
  {
    Show_Upperpage(oled.ShowPage);
    Show_Changeable();
  }
  else
  {
    Show_Fullpage(oled.ShowPage);
  } 
}

void Show_Upperpage(int page)
{
  static int lastpage;
  if(lastpage!=page)
  {
    for(int i=0;i<5;LCD_ClearLine(i++));        //换页前清行
    lastpage=page;
  }
  switch(page)
  {
  case  0:
    oledprintf(0,0,"%4d %4d %4d %4d",inductor.Ad[0],inductor.Ad[6],inductor.Ad[5],inductor.Ad[3]);
    oledprintf(1,0,"%4d %4d %4d %4d",inductor.Ad[1],inductor.Ad[7],inductor.Ad[2],inductor.Ad[4]);
    oledprintf(2,0,"%4d %4d %4d %4d",track.LH,track.LV,track.LF,track.MV);
    oledprintf(3,0,"%4d %4d %4d %4d",track.RH,track.RV,track.RF,track.MH);
//    oledprintf(4,0,"SD:%4d Arr:%4d",flag.ToSendSD,flag.ToSendArray);
//    oledprintf(3,0,"Status:%4d",mycar.Status);
    break;
  case 1:    
    //oledprintf(0,0,"T:%4d RunT:%4d",T,mycar.RunTime);
    oledprintf(0,0,"N%4d F%4d O%5d",track.NearOut,track.FarOut,track.Out);
    oledprintf(3,0,"St:%2d Tr:%2d Fa%2d Fd%2d",mycar.Status,(int)status_track,flag.ToSendArray,flag.ToSendSD);
    oledprintf(2,0,"AmpkDyc:%4d",track.TrackAmpDyc);
    oledprintf(4,0,"SumH%4d",(track.LH+track.MH+track.RH) * 2/3);
    // oledprintf(3,0,"indata:%4d",indata.Speed); 
    // oledprintf(4,0,"Motor:%4d",outdata.Motor_PWM);
    
    break;
  case 2:  
//    oledprintf(0,0,"Track:%5d",track.Out);
//    oledprintf(1,0,"angle_real%5d",indata.angle_real);
//    oledprintf(2,0,"%4d %4d %4d",indata.mpu6050.gyr_x,indata.mpu6050.gyr_y,indata.mpu6050.gyr_z);
      oledprintf(1,0,"Speed%4d",indata.Speed);
//      oledprintf(2,0,"CarStatus:%2d",mycar.Status);
      oledprintf(2,0,"OSpeed:%4d",setpara.SetSpeed);
      oledprintf(3,0,"Track_Status:%4d",(int)status_track);
      oledprintf(4,0,"L%2d R%2d Dir%2d",track.L,track.R,track.LockDir);
  default:
    break;
  }
}

void Show_Changeable()
{
  int temp_ParaSelect = oled.ParaSelect;      //由于潜在的中断干扰，必须先存储参数序号以避免危险
  if(temp_ParaSelect>0)
  {
    oledprintf(5,0,"%02d.%-13s",temp_ParaSelect-1,paralist[temp_ParaSelect-1].label);
    oledprintf(5,96,"%5d",*paralist[temp_ParaSelect-1].para);
  }
  else
  {
    LCD_ClearLine(5);
  }
  if(oled.ChangePara)
  {
    oledprintf(6,0,"%02d.%-13s",temp_ParaSelect,paralist[temp_ParaSelect].label);
    oledprintfw(6,96,"%5d",*paralist[temp_ParaSelect].para);
  }
  else
  {  
    oledprintfw(6,0,"%02d.%-13s",temp_ParaSelect,paralist[temp_ParaSelect].label);
    oledprintf(6,96,"%5d",*paralist[temp_ParaSelect].para);
  }
  if(temp_ParaSelect<oled.ParaNum-1)
  {
    oledprintf(7,0,"%02d.%-13s",temp_ParaSelect+1,paralist[temp_ParaSelect+1].label);
    oledprintf(7,96,"%5d",*paralist[temp_ParaSelect+1].para);
  }  
  else
  {
    LCD_ClearLine(7);
  }
}

void Show_Fullpage(int page)
{
  switch(page)
  {
  case -1:
    break;
  case -2:
    break;
  case -3:
    break;
  default:
    break; 
  }
}

