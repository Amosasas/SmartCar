#include "car_init.h"
 
void Init_All()
{
  Init_Setpara();
  Init_Paranum();       
  Init_Key();           
  OLED_Init(1);          //OLED屏幕初始化
  Init_Motor();
  Init_Steer();
  Init_ADC();
  Init_PIT();           //周期中断初始化
  Init_I2C();           //MPU6050初始化
  Init_Flash();         //FLASH参数存取初始化
  Init_Sdhc();          //SD卡模块初始化
  Init_All_Pulse_Counter();//编码器初始化
  Init_Uart_Interr();
 // Init_Temporary();
}
 
void Init_Temporary()
{
 // mycar.SpeedError              =          0;
}

void Init_Setpara()    //可调参数初始值
{
    track.TrackAmpDyc = 600;
    track.AmpK = 600; 
    setpara.Ampk = 600;
    
  mycar.Status                 =            0;
  
  setpara.Steer.mid             =         895;
  setpara.Steer.maxturn         =         182;
  
  setpara.SpeedDutyMax          =     6000;
  setpara.diff_slope            =     1000;
  
  setpara.SteerPD.Kp              =         110;  
  setpara.SteerPD.Kd              =         200;
  setpara.SteerPD.Kpchange        =          0;
  
  setpara.SpeedPID.Kp           =         38;
  setpara.SpeedPID.Ki           =         13;
  setpara.SpeedPID.Kd           =         11;
  
  setpara.LStraightMax          =        250;
  setpara.LStraightMin          =        200;
  
  setpara.RightSpeedMax         =        230;
  setpara.RightSpeedMin         =        210;
  
  setpara.RightThreshold        =        500;  
  
  setpara.StraightMax           =         240;
  setpara.StraightMin           =         220;
  
  setpara.KFar                  =         95;
  setpara.KNear                 =         50; 
  
  setpara.FarCal[0]	            =       1000;
  setpara.FarCal[1]	            =        950;
  setpara.FarCal[2]	            =        880;
  setpara.FarCal[3]	            =	 790;
  setpara.FarCal[4]	            =	 700;
  setpara.FarCal[5]	            =	 550;
  setpara.FarCal[6]	            =	 440;
  setpara.FarCal[7]	            =	 250;
  setpara.FarCal[8]	            =	 120;
  setpara.FarCal[9]	            =	  40;
  setpara.FarCal[10]         	=      0;
  setpara.FarCal[11]     	    =       0;
  setpara.WeightNear[0]	        =	   0;
  setpara.WeightNear[1]	        =	   0;
  setpara.WeightNear[2]	        =	 140;
  setpara.WeightNear[3]	        =	 360;
  setpara.WeightNear[4] 	    =	 540;
  setpara.WeightNear[5] 	    =	 670;
  setpara.WeightNear[6] 	    =	 780;
  setpara.WeightNear[7] 	    =	 880;
  setpara.WeightNear[8]	        =	 950;
  setpara.WeightNear[9]	        =	 990;
  setpara.WeightNear[10]	    =	1000;
  setpara.WeightNear[11]	    =	1000;
  setpara.WeightFar[0]  	    =	1100;
  setpara.WeightFar[1]  	    =	 960;
  setpara.WeightFar[2]  	    =	 860;
  setpara.WeightFar[3]  	    =	 750;
  setpara.WeightFar[4] 	        =	 620;
  setpara.WeightFar[5] 	        =	 560;
  setpara.WeightFar[6] 	        =	 440;
  setpara.WeightFar[7]  	    =	 380;
  setpara.WeightFar[8]  	    =	 250;
  setpara.WeightFar[9] 	        =	  80;
  setpara.WeightFar[10]	        =	   0;
  setpara.WeightFar[11] 	    =	   0;
  
  setpara.LoseThreshold         =       400;
  setpara.Status   =    1;
  //status = 0;
  
  
  
  mycar.Reed                    =      0;
}

void Init_Paranum()   //可调参数数量计算
{
  while(paralist[oled.ParaNum].precision)
    oled.ParaNum++;
  
  oled.ShowPageMax = 2;
  oled.ShowPageMin = 0;
}

void Init_Key()      //旋钮、拨轮开关等按键初始化
{
  GPIO_InitTypeDef gpio_init_struct={0};
  
  gpio_init_struct.GPIO_PTx = PTC;              //PORTC
  gpio_init_struct.GPIO_Pins = GPIO_Pin2|GPIO_Pin3|GPIO_Pin4|GPIO_Pin5|GPIO_Pin6|GPIO_Pin7|GPIO_Pin8;     //引脚3、4、5、6、7、8
  gpio_init_struct.GPIO_Dir = DIR_INPUT;        //输入
  gpio_init_struct.GPIO_PinControl = INPUT_PF_EN|IRQC_FA;   //输入低通滤波，下降沿中断
  gpio_init_struct.GPIO_Isr = Portc_Isr;        //中断函数
  LPLD_GPIO_Init(gpio_init_struct);
  //使能中断
  LPLD_GPIO_EnableIrq(gpio_init_struct);
}

void Init_Motor()      //电机初始化   
{
  FTM_InitTypeDef ftm0_init_struct={0};
  
  ftm0_init_struct.FTM_Ftmx = FTM0;	        //使能FTM0通道
  ftm0_init_struct.FTM_Mode = FTM_MODE_PWM;	//使能PWM模式
  ftm0_init_struct.FTM_PwmFreq = 15000;	    //PWM频率15000Hz
  
  LPLD_FTM_Init(ftm0_init_struct);
  
  /*****************************************************
  *------------    --------------- -PWM对齐方式
  *--------------- --------------端口号  ↓
  *----------------------初始占空比↓    ↓
  *--------------------通道号  ↓  ↓    ↓
  *-----------------模块号↓   ↓  ↓    ↓
  *                   ↓  ↓   ↓  ↓    ↓       */
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch4,0,PTD4,ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch5,0,PTD5,ALIGN_LEFT);
}

void Init_Steer()
{
  FTM_InitTypeDef ftm_init_struct={0};
  
  ftm_init_struct.FTM_Ftmx = FTM2;	        //使能FTM2通道
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	//使能PWM模式
  ftm_init_struct.FTM_PwmFreq = 50;	        //PWM频率50Hz
  
  LPLD_FTM_Init(ftm_init_struct);
  
  LPLD_FTM_PWM_Enable(FTM2,FTM_Ch0,150,PTB18,ALIGN_LEFT);
}

void Init_ADC()
{
  ADC_InitTypeDef adc_init_struct={0};
  
  adc_init_struct.ADC_Adcx = ADC0;                      //配置ADC采样参数
  adc_init_struct.ADC_BitMode = SE_10BIT;               //单端8位精度
  adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;    //长采样时间
  adc_init_struct.ADC_HwAvgSel = HW_32AVG;               //32次硬件平均
  adc_init_struct.ADC_CalEnable = TRUE;                 //使能初始化校验
  
  LPLD_ADC_Init(adc_init_struct);                       //初始化ADC

  adc_init_struct.ADC_Adcx = ADC1;

  //初始化ADC1
  LPLD_ADC_Init(adc_init_struct);
  //使能相应ADC通道
  LPLD_ADC_Chn_Enable(ADC0, AD23);      //使能相应ADC通道_电池电压

  LPLD_ADC_Chn_Enable(ADC0, AD8);       //PTB0
  LPLD_ADC_Chn_Enable(ADC0, AD9);       //PTB1
  LPLD_ADC_Chn_Enable(ADC0, AD12);      //PTB2
  LPLD_ADC_Chn_Enable(ADC0, AD13);      //PTB3
  LPLD_ADC_Chn_Enable(ADC0, AD14);      //PTC0
  LPLD_ADC_Chn_Enable(ADC0, AD15);      //PTC1
  LPLD_ADC_Chn_Enable(ADC1, AD14);      //PTB10
  LPLD_ADC_Chn_Enable(ADC1, AD15);      //PTB11
  
}

void Init_PIT()         //周期中断初始化
{
  PIT_InitTypeDef pit0_init_struct={PIT0};
  //配置PIT0参数
  pit0_init_struct.PIT_Pitx = PIT0;
  pit0_init_struct.PIT_PeriodMs = PIT0_MS;     //定时周期
  pit0_init_struct.PIT_Isr = Pit0_Isr;  //设置中断函数
  //初始化PIT0
  LPLD_PIT_Init(pit0_init_struct);  
  
  //使能PIT0
  LPLD_PIT_EnableIrq(pit0_init_struct);
}


void Init_I2C()
{
  uint8 device_id = MPU6050_Init();
  //通过设备ID判断设备是否为MPU6050
  if(device_id == MPU6050_ID)
  {
    printf("MPU6050初始化成功!\r\n");
    printf("设备ID: 0x%X\r\n", device_id);
  }
  else
  {
    printf("MPU6050初始化失败!\r\n");
    printf("设备ID: 0x%X\r\n", device_id);
    //while(1);
  }
}

void Init_Flash()
{
  LPLD_Flash_Init();
}

void Init_Sdhc()
{
  int i = disk_initialize(0);
  if(i==0) printf("SD卡初始化成功.\n");
  else printf("SD卡初始化失败.\n");
}

//编码器测速初始化
void Init_All_Pulse_Counter()
{
  Init_Pulse_Counter(PTC17,DMA_CH0,PTE6);
}
void Init_Pulse_Counter(PortPinsEnum_Type counter_port,
                        uint8 dma_chx,
                        PortPinsEnum_Type dir_port)
{
  GPIO_InitTypeDef gpio_init_struct={0};
  uint32 PTx_BASE = PTA_BASE+(counter_port>>5)*0x40u;
  gpio_init_struct.GPIO_PTx = (GPIO_Type *)PTx_BASE;
  gpio_init_struct.GPIO_Pins = 1<<(uint8)(counter_port&0x1Fu);
  gpio_init_struct.GPIO_Dir = DIR_INPUT;
  gpio_init_struct.GPIO_PinControl = IRQC_DMARI | INPUT_PF_EN;
  LPLD_GPIO_Init(gpio_init_struct);
  
  DMA_InitTypeDef dma_init_struct={0};                            //DMA参数配置
  dma_init_struct.DMA_CHx = dma_chx;                              //通道号
  dma_init_struct.DMA_Req = PORTA_DMAREQ+(counter_port>>5);       //DMA请求源
  dma_init_struct.DMA_MajorLoopCnt = 0x7FFF;                      //主循环计数值
  dma_init_struct.DMA_MinorByteCnt = 1;                           //次循环字节计数
  dma_init_struct.DMA_SourceAddr = 0x4004000C;                    //源地址
  dma_init_struct.DMA_DestAddr = 0x4004000C;                      //目的地址
  LPLD_DMA_Init(dma_init_struct);                                 //初始化DMA
  LPLD_DMA_EnableReq(dma_chx);                                    //使能DMA请求
  GPIO(dir_port,DIR_INPUT);
}

void Init_Uart_Interr()
{
  UART_InitTypeDef term_port_structure={0};
  term_port_structure.UART_Uartx = TERM_PORT;
  term_port_structure.UART_BaudRate = TERMINAL_BAUD;
  term_port_structure.UART_RxPin = PTB16;
  term_port_structure.UART_TxPin = PTB17;
  term_port_structure.UART_RxIntEnable = TRUE;    //使能接收中断
  term_port_structure.UART_RxIsr = Uart_Isr;      //设置接收中断函数
  LPLD_UART_Init(term_port_structure);
  LPLD_UART_EnableIrq(term_port_structure);    //Enable uart interrupt
}


