#include "debug_oscilloscope.h"

unsigned short Crc_Check(unsigned char *Buf, unsigned char CRC_CNT)
{
  unsigned short CRC_Temp;
  unsigned char i,j;
  CRC_Temp = 0xffff;
  
  for (i=0;i<CRC_CNT; i++){      
    CRC_Temp ^= Buf[i];
    for (j=0;j<8;j++) {
      if (CRC_Temp & 0x01)
        CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
      else
        CRC_Temp = CRC_Temp >> 1;
    }
  }
  return(CRC_Temp);
}

void UART_SendData(uint8_t Data)
{
  while(!(UART0->S1 & UART_S1_TDRE_MASK));
  UART0->D = (uint8_t)Data;
}

void OutPut_Data(void)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  unsigned char databuf[10] = {0};
  
  unsigned char i;
  unsigned short CRC16 = 0;
  for(i=0;i<4;i++)
  {    
    temp[i]  = (int)OutData[i];
    temp1[i] = (unsigned int)temp[i];   
  }
  
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
  
  CRC16 = Crc_Check(databuf,8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256;
  
  for(i=0;i<10;i++)
    UART_SendData(databuf[i]);
}

void Send_UART()
{
  if(flag.ToScope)
  {
    OutData[0]=(int)(mycar.gyr_x * 100);
    OutData[1]=(int)(mycar.acc_z * 100);
    OutData[2]=(int)(mycar.CarAngle * 100);
    OutData[3]=(int)(mycar.gyr_y * 100);
    
    OutPut_Data();
  }
}


