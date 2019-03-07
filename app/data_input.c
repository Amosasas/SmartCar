#include "data_input.h"

void Data_Input()
{
//  mycar.BattVolt = Battery_Check();
  indata.Speed = Get_Speed();
  indata.Speed=indata.Speed*4/3;
  indata.mpu6050.gyr_x=(int16)((int8)MPU6050_ReadReg(MPU6050_GYRO_XOUT));
  indata.mpu6050.gyr_z=(int16)((int8)MPU6050_ReadReg(MPU6050_GYRO_ZOUT));
  indata.mpu6050.gyr_y=(int16)((int8)MPU6050_ReadReg(MPU6050_GYRO_YOUT));
  Get_Ad_New(inductor.Ad);                                 //获取8路AD转换数值,限幅滤波，为了防止在车运行过程中抖动造成的突变
}

//电池电压
uint16 Battery_Check()
{
  static uint16 Value;
  if(T%500 == 0 && mycar.Status == 0)
        Value = (int)(LPLD_ADC_Get(ADC0,AD23)*(9.9*1.147/256)*100);                   //电池电压检查
  return Value;
}
//编码器测速
int Get_Speed()
{
  static int DMA_CH0_COUNT[2] = {0x7FFF,0x7FFF};//2个数  32767
  DMA_CH0_COUNT[0] = DMA0->TCD[DMA_CH0].CITER_ELINKNO;
  int speed_temp = DMA_CH0_COUNT[1]-DMA_CH0_COUNT[0];
  if(speed_temp<0)speed_temp += 0x7FFF;
  if(PTE6_I)speed_temp=-speed_temp;
  DMA_CH0_COUNT[1]=DMA_CH0_COUNT[0];
  return speed_temp;
}

//DMA计数，不要跟Get_Speed一起用！！
/*int Get_Count()
{
  static int count = 0;
  if(DMA0->TCD[DMA_CH0].CITER_ELINKNO==100)
    count++;
  return count;//统计滚过了多少次到100 从32767开始往下减的 数一下滚动的轮数和当前的剩余数得经过脉冲数
}
*/
int Get_Count()
{
  static int count = 0;
 if(DMA0->TCD[DMA_CH0].CITER_ELINKNO==10)
    count++;
  if(DMA0->TCD[DMA_CH0].CITER_ELINKNO>10)
    return (count+1)*32767+10-DMA0->TCD[DMA_CH0].CITER_ELINKNO;//统计滚过了多少次到100 从32767开始往下减的 数一下滚动的轮数和当前的剩余数得经过脉冲数
  return count*32767+10-DMA0->TCD[DMA_CH0].CITER_ELINKNO;
}//不能保证返回值一直在范围内 别让车跑太远了 先空转看看行不行
//
////电磁传感器
void Get_Ad_Real(int32* ad)
{
  int i;
  inductor.AdReal[0] = (LPLD_ADC_Get(ADC0, AD8)+LPLD_ADC_Get(ADC0, AD8))/2;       //PTB0  平均32次 30us
  inductor.AdReal[1] = (LPLD_ADC_Get(ADC0, AD15)+LPLD_ADC_Get(ADC0, AD15))/2;      //PTC1
  inductor.AdReal[2] = (LPLD_ADC_Get(ADC0, AD14)+LPLD_ADC_Get(ADC0, AD14))/2;      //PTC0
  inductor.AdReal[3] = (LPLD_ADC_Get(ADC0, AD9)+LPLD_ADC_Get(ADC0, AD9))/2;       //PTB1
  inductor.AdReal[4] = (LPLD_ADC_Get(ADC1, AD15)+LPLD_ADC_Get(ADC1, AD15))/2;      //PTB11
  inductor.AdReal[5] = (LPLD_ADC_Get(ADC1, AD14)+LPLD_ADC_Get(ADC1, AD14))/2;      //PTB10  
  inductor.AdReal[6] = (LPLD_ADC_Get(ADC0, AD12)+LPLD_ADC_Get(ADC0, AD12))/2;      //PTB2        
  inductor.AdReal[7] = (LPLD_ADC_Get(ADC0, AD13)+LPLD_ADC_Get(ADC0, AD13))/2;      //PTB3
  for(i=0;i<8;i++)
  {
    ad[i]=inductor.AdReal[i];
  } 
}
void Get_Ad_New(int32* ad)
{
  for(int i=0;i<8;i++)
  {
    inductor.AdOld[i]=inductor.Ad[i];
  }
  Get_Ad_Real(inductor.Ad);
  for(int j=0;j<8;j++)
  {
    if((inductor.Ad[j]-inductor.AdOld[j])>A)
      ad[j] = inductor.AdOld[j]+A;
    else if((inductor.AdOld[j]-inductor.Ad[j])>A)
      ad[j] = inductor.AdOld[j]-A;
    else ad[j] = inductor.Ad[j];
  }
}