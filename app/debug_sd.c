#include "debug_sd.h"

#if 1 //数据存储//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
void Send_Array(int* array,char* name)
{
  int i=0;
  printf("int %s",name);
  printf("[11]={");
  for(i=0;i<10;i++)
    printf("%d,",array[i]);
  printf("%d};\n",array[i]);
}

void Send_Int(int value,char* name)
{
  printf("%s",name);//输出字符串
  printf(" = ");
  printf("%d;\n",value);
}

void Send_Tab_All()
{
  printf("#if 1 //Default Para \n");
  //  Send_Array(setpara.weight_near,"WitNear");
  //  Send_Array(setpara.weight_far,"WitFar");
  printf(" void set_para_default()\n ");
  printf("{\n");
  
  Send_Int(setpara.SetTime,"SetTime");  
  
  
  printf("}\n");
  printf("#endif\n");
}
//★★★★★★★★★★★★★★★★
void Save_Flash()
{
  uint32 data_to_write[100];
  for(int32 i=0;i<100;i++)
    data_to_write[i] = *paralist[i].para;
  LPLD_Flash_SectorErase(FLASH_SAVE_SECTOR*0x800);
  LPLD_Flash_ByteProgram(FLASH_SAVE_SECTOR*0x800,data_to_write,oled.ParaNum*4);
  
}

void Read_Flash()
{
  //定义参数读取
  for(int32 i=0;i<oled.ParaNum;i++)
  {
    *paralist[i].para = *(int*)(FLASH_SAVE_SECTOR*0x800+i*4);
  }
}
//★★★★★★★★★★★★★★★★
void Save_RAM()
{
  Save_SD((int)mycar.RunTime,"T");
//  Save_SD((int)mycar.gyr_x,"GX");
//  Save_SD((int)(control.BalanceAngle * 10),"CA");
//  Save_SD((int)(mycar.CarAngle * 10),"Angle");
//  Save_SD((int)outdata.UprightControl,"UpOut");
  Save_SD((int)indata.Speed,"Speed");

  Save_SD((int)track.Out,"Track");
  Save_SD((int)track.NearOut,"NearOut");
  Save_SD((int)track.FarOut,"FarOut");
  Save_SD((int)(outdata.Steer_PWM-setpara.Steer.mid),"Steer");
//  Save_SD((int)(track.Out - track.OutLast),"DTrack");

  
  Save_SD((int)0xAAAA,"");
}
//★★★★★★★★★★★★★★★★
void Save_SD(int value,char* label)
{
  if(flag.ToSendSD==1) //打印标签，每个变量只会执行一次，为了把变量名字打印出来。
  {
    if((uint16)value==0xAAAA)printf("\n");
    else printf("%s\t",label);
  }
  if(mycar.Status==1&&flag.ToSendSD==0)//SD卡中储存的是16位的整型,把整型分成两个字符型变量。
  {
    RAM_BUFF[(2*save_ram_no)/512%50][(2*save_ram_no)%512] = (uint8)(((uint16)value)>>8);
    RAM_BUFF[(2*save_ram_no)/512%50][(2*save_ram_no)%512+1] = (uint8)((uint16)value);
    save_ram_no++;
  }
}
//★★★★★★★★★★★★★★★★
void Data_Save()//本函数放在PIT中，每个周期产生的变化都记录下来。
{
  if(mycar.Status==1&&flag.ToSendSD==0)
    Save_RAM();
}
//★★★★★★★★★★★★★★★★
void Write_SD()//本函数放在主函数中，车运行完通过蓝牙发送到电脑上。
{
  uint16 temp_flash_read;
  //★
  if(flag.ToSendArray==1)
  {
    flag.ToSendArray=0;
    Send_Tab_All();  
  }
 //★ 
  if(mycar.Status==1)
  {
    if(TO_SAVE_FLASH_NU<(2*save_ram_no)/512)//数组一维512byte满了后就会写入。
    {
      int32 i=disk_write(0,RAM_BUFF[TO_SAVE_FLASH_NU%50],TO_SAVE_FLASH_NU+SD_BASE,1);	//0,写入的数据*a（一次512个）,开始写的扇区，要写的扇区数
      if(i==0)
      {
        TO_SAVE_FLASH_NU++;
      }
      else
      {
        for(int32 i=100;i--;);
      }
    }
  }
  
 //★ 
  if(flag.ToSendSD==1)
  {
    DisableInterrupts;
    Save_RAM();       //调用Save_RAM()函数的打印标签功能
    flag.ToSendSD = 0;
    for(int32 i=0;i<TO_SAVE_FLASH_NU;i++)
    {  
      int j;
      while(disk_read(0,SEND_BUFF,i+SD_BASE,1)); //把SD卡中的变量读取出来，通过串口打印函数“printf（）”发送出来。
      for(j=0;j<511;j+=2)                       //0，读出的数据*a（一次512个），开始读的扇区，要写的扇区数
      {       
        temp_flash_read=SEND_BUFF[j]*256+SEND_BUFF[j+1];
        if(temp_flash_read==0xAAAA)
          printf("\n");
        else 
          printf("%d\t",(int16)temp_flash_read);   
      } 
    }
    EnableInterrupts;
  }
}    


#endif
